/**
 * ble_module.h - NeoCAT v1 BLE Pentesting Module
 * 
 * Provides BLE scanning and advertisement spam capabilities
 * using the NimBLE stack for ESP32-S3.
 * 
 * Supported spam types:
 *   - Apple Proximity Pairing (AirPods popups)
 *   - Samsung BLE advertisements
 *   - Windows Swift Pair notifications
 * 
 * Part of the NeoCAT v1 firmware.
 */

#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <ArduinoJson.h>

/// Discovered BLE device metadata
struct BLEDeviceInfo {
  String name;
  String address;
  int rssi;
  String manufacturer;
  bool isPublic;
  uint32_t lastSeen;
};

/// BLE spam target platform
enum BLESpamType {
  BLE_SPAM_NONE = 0,
  BLE_SPAM_APPLE,
  BLE_SPAM_SAMSUNG,
  BLE_SPAM_WINDOWS,
  BLE_SPAM_ALL
};

class BLEModule {
public:
  /// Initialize BLE stack and advertising
  void begin();

  /// Main loop update — drives scan completion and spam cycling
  void update();

  // ── Scanning ──────────────────────────────────────────────
  void startScan(int duration = 5);   ///< Start async scan (seconds)
  bool isScanning();
  String getScanResultsJSON();        ///< JSON array of discovered devices
  int getDeviceCount();
  const BLEDeviceInfo* getDevice(int index) const;

  // ── Spam ──────────────────────────────────────────────────
  void startSpam(BLESpamType type);
  void stopSpam();
  bool isSpamming();
  BLESpamType getSpamType();
  uint32_t getSpamCount();            ///< Total advertisements sent

  // ── Status ────────────────────────────────────────────────
  String getStatusJSON();

private:
  std::vector<BLEDeviceInfo> _devices;
  BLESpamType _spamType      = BLE_SPAM_NONE;
  bool        _scanning      = false;
  bool        _spamming      = false;
  uint32_t    _spamCount     = 0;
  unsigned long _lastSpamTime  = 0;
  unsigned long _scanStartTime = 0;
  int         _scanDuration    = 5;
  NimBLEAdvertising* _pAdvertising = nullptr;

  // Spam payload generators
  void _sendAppleSpam();
  void _sendSamsungSpam();
  void _sendWindowsSpam();

  /// Fill addr[6] with a valid random BLE address
  void _generateRandomAddr(uint8_t* addr);
};
