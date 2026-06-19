#include "chat_system.h"

ChatSystem chatSys;

// ---------------------------------------------------------------------------
// Initialization
// ---------------------------------------------------------------------------

void ChatSystem::begin() {
  // Default room settings
  strncpy(_roomName, "CATroom", CHAT_MAX_ROOM_NAME);
  _roomName[CHAT_MAX_ROOM_NAME] = '\0';

  _roomPassword[0] = '\0';

  // Default emoji: cat face 🐱 (UTF-8: F0 9F 90 B1)
  strncpy(_roomEmoji, "\xF0\x9F\x90\xB1", sizeof(_roomEmoji) - 1);
  _roomEmoji[sizeof(_roomEmoji) - 1] = '\0';

  _roomActive = false;
  _roomStartTime = 0;

  // Zero out all user slots
  memset(_users, 0, sizeof(_users));
  for (int i = 0; i < CHAT_MAX_USERS; i++) {
    _users[i].active = false;
  }

  // Zero out all message slots
  memset(_messages, 0, sizeof(_messages));
  _msgHead = 0;
  _msgCount = 0;
  _totalMsgCount = 0;
  _userCount = 0;
}

// ---------------------------------------------------------------------------
// Room management
// ---------------------------------------------------------------------------

bool ChatSystem::createRoom(const String& name, const String& password) {
  if (name.length() == 0) return false;

  strncpy(_roomName, name.c_str(), CHAT_MAX_ROOM_NAME);
  _roomName[CHAT_MAX_ROOM_NAME] = '\0';

  strncpy(_roomPassword, password.c_str(), CHAT_MAX_ROOM_PASS);
  _roomPassword[CHAT_MAX_ROOM_PASS] = '\0';

  // Room is not marked active here — the main firmware activates it
  // when the WiFi AP is configured and ready.
  _roomActive = false;
  _roomStartTime = millis();
  return true;
}

void ChatSystem::startRoom() {
  _roomActive = true;
  _roomStartTime = millis();
  clearMessages();
  clearUsers();
  addSystemMessage("Chat room started");
}

void ChatSystem::stopRoom() {
  addSystemMessage("Chat room closed by admin");
  _roomActive = false;
  clearUsers();
}

void ChatSystem::setRoomActive(bool active) {
  _roomActive = active;
  if (active && _roomStartTime == 0) {
    _roomStartTime = millis();
  }
}

bool ChatSystem::isRoomActive() {
  return _roomActive;
}

String ChatSystem::getRoomName() {
  return String(_roomName);
}

String ChatSystem::getRoomPassword() {
  return String(_roomPassword);
}

void ChatSystem::setRoomEmoji(const String& emoji) {
  strncpy(_roomEmoji, emoji.c_str(), sizeof(_roomEmoji) - 1);
  _roomEmoji[sizeof(_roomEmoji) - 1] = '\0';
}

String ChatSystem::getRoomEmoji() {
  return String(_roomEmoji);
}

// ---------------------------------------------------------------------------
// User management
// ---------------------------------------------------------------------------

bool ChatSystem::addUser(uint32_t clientId, const String& name, uint8_t avatar, IPAddress ip) {
  // Reject if already present
  if (getUser(clientId) != nullptr) return false;

  // Find an empty slot
  int slot = -1;
  for (int i = 0; i < CHAT_MAX_USERS; i++) {
    if (!_users[i].active) {
      slot = i;
      break;
    }
  }
  if (slot < 0) return false;  // Room full

  ChatUser& u = _users[slot];
  u.clientId = clientId;

  strncpy(u.name, name.c_str(), CHAT_MAX_NAME_LEN);
  u.name[CHAT_MAX_NAME_LEN] = '\0';

  u.avatar = (avatar > 3) ? 3 : avatar;  // Clamp 0-3
  u.ip = ip;
  u.joinTime = millis();
  u.isAdmin = (_userCount == 0);  // First user becomes admin
  u.active = true;

  _userCount++;

  // System announcement
  char sysBuf[CHAT_MAX_NAME_LEN + 24];
  snprintf(sysBuf, sizeof(sysBuf), "%s joined the room", u.name);
  addSystemMessage(String(sysBuf));

  return true;
}

void ChatSystem::removeUser(uint32_t clientId) {
  for (int i = 0; i < CHAT_MAX_USERS; i++) {
    if (_users[i].active && _users[i].clientId == clientId) {
      char sysBuf[CHAT_MAX_NAME_LEN + 20];
      snprintf(sysBuf, sizeof(sysBuf), "%s left the room", _users[i].name);

      _users[i].active = false;
      if (_userCount > 0) _userCount--;

      addSystemMessage(String(sysBuf));
      return;
    }
  }
}

ChatUser* ChatSystem::getUser(uint32_t clientId) {
  for (int i = 0; i < CHAT_MAX_USERS; i++) {
    if (_users[i].active && _users[i].clientId == clientId) {
      return &_users[i];
    }
  }
  return nullptr;
}

int ChatSystem::getUserCount() {
  return _userCount;
}

bool ChatSystem::isAdmin(uint32_t clientId) {
  ChatUser* u = getUser(clientId);
  return (u != nullptr) && u->isAdmin;
}

void ChatSystem::setAdmin(uint32_t clientId) {
  ChatUser* u = getUser(clientId);
  if (u != nullptr) {
    u->isAdmin = true;
  }
}

// ---------------------------------------------------------------------------
// Messages (circular buffer)
// ---------------------------------------------------------------------------

void ChatSystem::addMessage(uint32_t clientId, const String& text) {
  ChatUser* u = getUser(clientId);
  if (u == nullptr) return;

  ChatMessage& msg = _messages[_msgHead];

  strncpy(msg.sender, u->name, CHAT_MAX_NAME_LEN);
  msg.sender[CHAT_MAX_NAME_LEN] = '\0';

  msg.avatar = u->avatar;

  strncpy(msg.text, text.c_str(), CHAT_MAX_MSG_LEN);
  msg.text[CHAT_MAX_MSG_LEN] = '\0';

  msg.timestamp = millis() / 1000;
  msg.type = 0;  // normal message
  msg.senderId = clientId;
  msg.msgId = (uint32_t)_totalMsgCount;

  _msgHead = (_msgHead + 1) % CHAT_MAX_MESSAGES;
  _totalMsgCount++;
  if (_msgCount < CHAT_MAX_MESSAGES) {
    _msgCount++;
  }
}

void ChatSystem::addSystemMessage(const String& text) {
  ChatMessage& msg = _messages[_msgHead];

  strncpy(msg.sender, "System", CHAT_MAX_NAME_LEN);
  msg.sender[CHAT_MAX_NAME_LEN] = '\0';

  msg.avatar = 255;

  strncpy(msg.text, text.c_str(), CHAT_MAX_MSG_LEN);
  msg.text[CHAT_MAX_MSG_LEN] = '\0';

  msg.timestamp = millis() / 1000;
  msg.type = 1;  // system message
  msg.senderId = 0;
  msg.msgId = (uint32_t)_totalMsgCount;

  _msgHead = (_msgHead + 1) % CHAT_MAX_MESSAGES;
  _totalMsgCount++;
  if (_msgCount < CHAT_MAX_MESSAGES) {
    _msgCount++;
  }
}

int ChatSystem::getMessageCount() {
  return _totalMsgCount;
}

// ---------------------------------------------------------------------------
// JSON output — ArduinoJson v7 (JsonDocument, not StaticJsonDocument)
// ---------------------------------------------------------------------------

String ChatSystem::getUsersJSON() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  for (int i = 0; i < CHAT_MAX_USERS; i++) {
    if (!_users[i].active) continue;

    JsonObject obj = arr.add<JsonObject>();
    obj["id"] = _users[i].clientId;
    obj["name"] = _users[i].name;
    obj["avatar"] = _users[i].avatar;
    obj["ip"] = _users[i].ip.toString();
    obj["joinTime"] = _users[i].joinTime;
    obj["isAdmin"] = _users[i].isAdmin;
  }

  String out;
  serializeJson(doc, out);
  return out;
}

String ChatSystem::getMessagesJSON(int sinceIndex) {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  if (_msgCount == 0) {
    String out;
    serializeJson(doc, out);
    return out;
  }

  // Calculate the starting position in the circular buffer.
  // sinceIndex is a global message ID; we want messages with msgId >= sinceIndex.
  //
  // The oldest stored message sits at:
  //   - If buffer not yet full: index 0
  //   - If buffer full: _msgHead (the next write position wraps over the oldest)
  int startSlot;
  if (_msgCount < CHAT_MAX_MESSAGES) {
    startSlot = 0;
  } else {
    startSlot = _msgHead;  // oldest message in a full circular buffer
  }

  for (int i = 0; i < _msgCount; i++) {
    int idx = (startSlot + i) % CHAT_MAX_MESSAGES;
    const ChatMessage& m = _messages[idx];

    // Skip messages older than the requested index
    if ((int)m.msgId < sinceIndex) continue;

    JsonObject obj = arr.add<JsonObject>();
    obj["sender"] = m.sender;
    obj["avatar"] = m.avatar;
    obj["text"] = m.text;
    obj["time"] = m.timestamp;
    obj["type"] = m.type;
    obj["id"] = m.msgId;
  }

  String out;
  serializeJson(doc, out);
  return out;
}

String ChatSystem::getRoomJSON() {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  obj["name"] = _roomName;
  obj["active"] = _roomActive;
  obj["userCount"] = _userCount;
  obj["maxUsers"] = CHAT_MAX_USERS;
  obj["messageCount"] = _totalMsgCount;
  obj["emoji"] = _roomEmoji;

  // Uptime in seconds since room was created (or 0 if never created)
  if (_roomStartTime > 0) {
    obj["uptime"] = (millis() - _roomStartTime) / 1000;
  } else {
    obj["uptime"] = 0;
  }

  String out;
  serializeJson(doc, out);
  return out;
}

String ChatSystem::getStatsJSON() {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  obj["totalMessages"] = _totalMsgCount;
  obj["activeUsers"] = _userCount;
  obj["maxUsers"] = CHAT_MAX_USERS;

  if (_roomStartTime > 0) {
    obj["roomUptime"] = (millis() - _roomStartTime) / 1000;
  } else {
    obj["roomUptime"] = 0;
  }

  obj["freeHeap"] = ESP.getFreeHeap();
  obj["cpuFreq"] = ESP.getCpuFreqMHz();

#if CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32
  obj["cpuTemp"] = temperatureRead();
#else
  obj["cpuTemp"] = 0;
#endif

  String out;
  serializeJson(doc, out);
  return out;
}

// ---------------------------------------------------------------------------
// Cleanup
// ---------------------------------------------------------------------------

void ChatSystem::clearMessages() {
  memset(_messages, 0, sizeof(_messages));
  _msgHead = 0;
  _msgCount = 0;
  _totalMsgCount = 0;
}

void ChatSystem::clearUsers() {
  for (int i = 0; i < CHAT_MAX_USERS; i++) {
    _users[i].active = false;
  }
  _userCount = 0;
}

void ChatSystem::reset() {
  clearMessages();
  clearUsers();

  // Restore defaults
  strncpy(_roomName, "CATroom", CHAT_MAX_ROOM_NAME);
  _roomName[CHAT_MAX_ROOM_NAME] = '\0';

  _roomPassword[0] = '\0';

  strncpy(_roomEmoji, "\xF0\x9F\x90\xB1", sizeof(_roomEmoji) - 1);
  _roomEmoji[sizeof(_roomEmoji) - 1] = '\0';

  _roomActive = false;
  _roomStartTime = 0;
}
