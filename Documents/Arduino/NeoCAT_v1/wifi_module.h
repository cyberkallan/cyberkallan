/*
 * ============================================================================
 *  wifi_module.h — WiFi Pentesting Module for NeoCAT V1
 * ============================================================================
 *  Provides WiFi network scanning, deauthentication, beacon flooding,
 *  probe request flooding, and Rickroll beacon attacks using raw 802.11
 *  frame injection via the ESP32-S3 WiFi peripheral.
 *
 *  DISCLAIMER: For authorized security testing only.
 * ============================================================================
 */

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <vector>
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_err.h"

// ========================= Data Structures ==================================

struct ScanResult {
  String ssid;
  String bssid;
  uint8_t bssidRaw[6];   // Raw BSSID bytes for frame construction
  int32_t rssi;
  uint8_t channel;
  wifi_auth_mode_t encryption;
  bool selected;          // Marked for targeting

  ScanResult() : rssi(0), channel(0), encryption(WIFI_AUTH_OPEN), selected(false) {
    memset(bssidRaw, 0, sizeof(bssidRaw));
  }
};

enum WiFiAttackType {
  ATTACK_NONE = 0,
  ATTACK_DEAUTH,
  ATTACK_BEACON,
  ATTACK_PROBE,
  ATTACK_RICKROLL       // Beacon flood with Rick Astley lyrics as SSIDs
};

// ========================= WiFi Module Class ================================

class WiFiModule {
public:
  WiFiModule();
  ~WiFiModule();

  /**
   * Initialize the WiFi module. Enables promiscuous mode for raw TX.
   * Must be called after WiFi.softAP() has been started.
   */
  void begin();

  /**
   * Main update loop — call frequently from loop() or a FreeRTOS task.
   * Handles async scan completion and attack packet transmission.
   */
  void update();

  // -------------------- Scanning --------------------

  /** Start an asynchronous WiFi network scan */
  void startScan();

  /** Returns true if a scan is currently in progress */
  bool isScanning() const;

  /** Get scan results as a JSON string array */
  String getScanResultsJSON();

  /** Get the number of networks found in the last scan */
  int getScanCount() const;

  /** Get a pointer to a specific scan result (bounds-checked) */
  const ScanResult* getScanResult(int index) const;

  /** Toggle selection state of a scan result by index */
  void toggleTarget(int index);

  /** Select all scan results */
  void selectAll();

  /** Deselect all scan results */
  void deselectAll();

  // -------------------- Attacks ---------------------

  /** Start deauthentication attack on a specific target by scan index */
  void startDeauth(int targetIndex);

  /** Start deauthentication against ALL scanned networks */
  void startDeauthAll();

  /** Start beacon flood with newline-separated custom SSIDs */
  void startBeaconFlood(String ssidList);

  /** Start Rickroll beacon flood (lyrics as SSIDs) */
  void startBeaconRickroll(String lyrics);

  /** Start probe request flood */
  void startProbeFlood();

  /** Stop any running attack immediately */
  void stopAttack();

  // -------------------- Status ----------------------

  /** Get the currently active attack type */
  WiFiAttackType getAttackType() const;

  /** Returns true if any attack is currently running */
  bool isAttacking() const;

  /** Get total packets sent during current attack */
  uint32_t getPacketsSent() const;

  /** Get full status as a JSON string */
  String getStatusJSON();

private:
  // -------------------- State -----------------------
  std::vector<ScanResult> _scanResults;
  WiFiAttackType _currentAttack;
  bool _scanning;
  uint32_t _packetsSent;
  uint32_t _attackStartTime;
  int _deauthTarget;            // -1 = all, >=0 = specific index
  String _beaconSSIDs;
  uint8_t _currentChannel;
  unsigned long _lastPacketTime;

  // For cycling through targets/SSIDs during attacks
  int _currentTargetIndex;
  int _currentSSIDIndex;
  std::vector<String> _ssidList;

  // -------------------- Frame TX --------------------

  /**
   * Send a deauthentication frame targeting a specific BSSID.
   * Broadcasts deauth to all clients of that AP.
   */
  void _sendDeauthFrame(uint8_t* bssid, uint8_t channel);
  void _sendDeauthBurst(uint8_t* bssid, uint8_t channel);

  /**
   * Send a beacon frame advertising the given SSID on the specified channel.
   * Uses a random locally-administered MAC address.
   */
  void _sendBeaconFrame(const char* ssid, uint8_t channel);

  /**
   * Send a probe request frame with the given SSID.
   * Uses a random locally-administered MAC address.
   */
  void _sendProbeFrame(const char* ssid, uint8_t channel);

  /**
   * Fast variants — skip channel switching (caller manages it for batching).
   */
  void _sendBeaconFrameNoChannelSwitch(const char* ssid, uint8_t channel);
  void _sendProbeFrameNoChannelSwitch(const char* ssid, uint8_t channel);

  /**
   * Generate a random locally-administered unicast MAC address.
   * Sets the locally-administered bit and clears the multicast bit.
   */
  void _generateRandomMAC(uint8_t* mac);

  /**
   * Parse a BSSID string "AA:BB:CC:DD:EE:FF" into a 6-byte array.
   */
  static void _parseBSSID(const String& bssidStr, uint8_t* out);

  /**
   * Convert wifi_auth_mode_t to a human-readable string.
   */
  static const char* _authModeToString(wifi_auth_mode_t mode);

  /**
   * Split a newline-separated string into a vector of strings.
   */
  void _parseSSIDList(const String& list);
};
