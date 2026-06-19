#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <ArduinoJson.h>

class ConfigManager {
public:
  void begin();
  
  // Getters
  String getSSID();
  String getPassword();
  String getUserName();
  String getInstaID();
  String getSettingsJSON();

  // Setters
  void setSSID(const String& ssid);
  void setPassword(const String& password);
  void setUserName(const String& userName);
  void setInstaID(const String& instaId);
  bool updateFromJson(const String& jsonStr);

  // System
  void factoryReset();

private:
  Preferences _prefs;
  
  String _ssid;
  String _password;
  String _userName;
  String _instaId;
};

extern ConfigManager config;
