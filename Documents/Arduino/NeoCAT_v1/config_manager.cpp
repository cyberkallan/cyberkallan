#include "config_manager.h"

ConfigManager config;

void ConfigManager::begin() {
  _prefs.begin("neocat", false);
  
  _ssid = _prefs.getString("ssid", "NeoCAT V1");
  _password = _prefs.getString("pass", "");
  _userName = _prefs.getString("user", "Guest");
  _instaId = _prefs.getString("insta", "");
}

String ConfigManager::getSSID() { return _ssid; }
String ConfigManager::getPassword() { return _password; }
String ConfigManager::getUserName() { return _userName; }
String ConfigManager::getInstaID() { return _instaId; }

void ConfigManager::setSSID(const String& ssid) {
  _ssid = ssid;
  _prefs.putString("ssid", ssid);
}

void ConfigManager::setPassword(const String& password) {
  _password = password;
  _prefs.putString("pass", password);
}

void ConfigManager::setUserName(const String& userName) {
  _userName = userName;
  _prefs.putString("user", userName);
}

void ConfigManager::setInstaID(const String& instaId) {
  _instaId = instaId;
  _prefs.putString("insta", instaId);
}

String ConfigManager::getSettingsJSON() {
  JsonDocument doc;
  doc["ssid"] = _ssid;
  doc["pass"] = _password;
  doc["user"] = _userName;
  doc["insta"] = _instaId;
  
  String out;
  serializeJson(doc, out);
  return out;
}

bool ConfigManager::updateFromJson(const String& jsonStr) {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, jsonStr);
  if (err) return false;

  if (doc.containsKey("ssid")) setSSID(doc["ssid"].as<String>());
  if (doc.containsKey("pass")) setPassword(doc["pass"].as<String>());
  if (doc.containsKey("user")) setUserName(doc["user"].as<String>());
  if (doc.containsKey("insta")) setInstaID(doc["insta"].as<String>());

  return true;
}

void ConfigManager::factoryReset() {
  _prefs.clear();
  begin(); // Reload defaults
}
