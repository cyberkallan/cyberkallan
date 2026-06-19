#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

#define CHAT_MAX_USERS 8
#define CHAT_MAX_MESSAGES 50
#define CHAT_MAX_NAME_LEN 20
#define CHAT_MAX_MSG_LEN 200
#define CHAT_MAX_ROOM_NAME 24
#define CHAT_MAX_ROOM_PASS 24

struct ChatUser {
  uint32_t clientId;       // WebSocket client ID
  char name[CHAT_MAX_NAME_LEN + 1];
  uint8_t avatar;          // 0-3
  IPAddress ip;
  uint32_t joinTime;       // millis()
  bool isAdmin;            // first user or device owner
  bool active;
};

struct ChatMessage {
  char sender[CHAT_MAX_NAME_LEN + 1];
  uint8_t avatar;
  char text[CHAT_MAX_MSG_LEN + 1];
  uint32_t timestamp;      // millis() / 1000 (uptime seconds)
  uint8_t type;            // 0=message, 1=system, 2=admin
  uint32_t senderId;       // client ID of sender
  uint32_t msgId;          // global message ID at time of insert
};

class ChatSystem {
public:
  void begin();

  // Room management
  bool createRoom(const String& name, const String& password);
  void startRoom();
  void stopRoom();
  void setRoomActive(bool active);
  bool isRoomActive();
  String getRoomName();
  String getRoomPassword();
  void setRoomEmoji(const String& emoji);
  String getRoomEmoji();

  // User management
  bool addUser(uint32_t clientId, const String& name, uint8_t avatar, IPAddress ip);
  void removeUser(uint32_t clientId);
  ChatUser* getUser(uint32_t clientId);
  int getUserCount();
  bool isAdmin(uint32_t clientId);
  void setAdmin(uint32_t clientId);

  // Messages
  void addMessage(uint32_t clientId, const String& text);
  void addSystemMessage(const String& text);
  int getMessageCount();

  // JSON output
  String getUsersJSON();
  String getMessagesJSON(int sinceIndex = 0);
  String getRoomJSON();
  String getStatsJSON();

  // Cleanup
  void clearMessages();
  void clearUsers();
  void reset();

private:
  char _roomName[CHAT_MAX_ROOM_NAME + 1];
  char _roomPassword[CHAT_MAX_ROOM_PASS + 1];
  char _roomEmoji[8];  // emoji character(s)
  bool _roomActive = false;
  uint32_t _roomStartTime = 0;

  ChatUser _users[CHAT_MAX_USERS];
  ChatMessage _messages[CHAT_MAX_MESSAGES];
  int _msgHead = 0;      // circular buffer write position
  int _msgCount = 0;     // total messages stored
  int _totalMsgCount = 0; // total messages ever sent
  int _userCount = 0;
};

extern ChatSystem chatSys;
