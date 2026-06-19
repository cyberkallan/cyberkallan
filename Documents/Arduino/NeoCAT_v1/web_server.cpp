/**
 * @file web_server.cpp
 * @brief NeoCAT V1 — Full async web-server implementation.
 *
 * Captive-portal redirects, REST-style JSON API for WiFi / BLE / LED /
 * system control, and a WebSocket channel that pushes live telemetry
 * every 500 ms.
 *
 * All POST endpoints that accept a body use AsyncCallbackJsonWebHandler
 * so the JSON is already parsed when the handler fires.
 */

#include "web_server.h"
#include "web_dashboard.h"
#include "config_manager.h"
#include "chat_system.h"
#include "chat_ui.h"

// ── module pointers (set once in setupWebServer) ─────────────────────────
static WiFiModule*     _wifi = nullptr;
static BLEModule*      _ble  = nullptr;
static LEDController*  _led  = nullptr;
static AsyncWebSocket* _ws   = nullptr;

static unsigned long _lastBroadcast = 0;
static const unsigned long BROADCAST_INTERVAL_MS = 500;

// ── Chat mode deferred AP switching ──────────────────────────────────
static volatile bool _chatStartPending = false;
static volatile bool _chatStopPending = false;
static bool _chatModeActive = false;

bool isChatMode() { return _chatModeActive; }

// ── CORS helper ──────────────────────────────────────────────────────────
static void addCorsHeaders(AsyncWebServerResponse* r) {
    r->addHeader("Access-Control-Allow-Origin", "*");
    r->addHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
    r->addHeader("Access-Control-Allow-Headers", "Content-Type");
}

static void sendJson(AsyncWebServerRequest* req, int code, const String& json) {
    AsyncWebServerResponse* r = req->beginResponse(code, "application/json", json);
    addCorsHeaders(r);
    req->send(r);
}

static void redirectToDashboard(AsyncWebServerRequest* req) {
    req->redirect("/");
}

// ── WebSocket event handler ──────────────────────────────────────────────
static void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                      AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT: {
            Serial.printf("[WS] Client #%u connected from %s\n",
                          client->id(), client->remoteIP().toString().c_str());

            // Send a welcome / system-info frame immediately
            JsonDocument doc;
            doc["type"]            = "system_info";
            doc["chipModel"]       = ESP.getChipModel();
            doc["chipRevision"]    = ESP.getChipRevision();
            doc["cpuFreqMHz"]      = ESP.getCpuFreqMHz();
            doc["freeHeap"]        = ESP.getFreeHeap();
            doc["totalHeap"]       = ESP.getHeapSize();
            doc["flashSize"]       = ESP.getFlashChipSize();
            doc["sdkVersion"]      = ESP.getSdkVersion();
            doc["firmwareVersion"] = "NeoCAT V1.0.0";
            doc["uptime"]          = millis() / 1000;

            String out;
            serializeJson(doc, out);
            client->text(out);
            break;
        }

        case WS_EVT_DISCONNECT:
            Serial.printf("[WS] Client #%u disconnected\n", client->id());
            break;

        case WS_EVT_DATA: {
            AwsFrameInfo* info = (AwsFrameInfo*)arg;
            if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
                data[len] = '\0';
                String msg = (char*)data;
                Serial.printf("[WS] Msg from #%u: %s\n", client->id(), msg.c_str());

                // Minimal command parsing over WS (most work goes through REST)
                JsonDocument cmdDoc;
                DeserializationError err = deserializeJson(cmdDoc, msg);
                if (!err) {
                    const char* cmd = cmdDoc["cmd"] | "";
                    if (strcmp(cmd, "ping") == 0) {
                        client->text("{\"type\":\"pong\"}");
                    } else if (strcmp(cmd, "status") == 0) {
                        // force an immediate status push
                        _lastBroadcast = 0;
                    }
                }
            }
            break;
        }

        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

// ══════════════════════════════════════════════════════════════════════════
// setupWebServer — the one-stop wiring function
// ══════════════════════════════════════════════════════════════════════════
void setupWebServer(AsyncWebServer& server, AsyncWebSocket& wsock,
                    WiFiModule& wifiMod, BLEModule& bleMod, LEDController& ledCtrl) {
    _wifi = &wifiMod;
    _ble  = &bleMod;
    _led  = &ledCtrl;
    _ws   = &wsock;

    // ── WebSocket ────────────────────────────────────────────────────────
    wsock.onEvent(onWsEvent);
    server.addHandler(&wsock);

    // ══════════════════════════════════════════════════════════════════════
    //  CAPTIVE-PORTAL REDIRECTS
    // ══════════════════════════════════════════════════════════════════════

    // Main dashboard (served from PROGMEM)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* req) {
        AsyncWebServerResponse* r =
            req->beginResponse_P(200, "text/html", (const uint8_t*)DASHBOARD_HTML, strlen_P(DASHBOARD_HTML));
        addCorsHeaders(r);
        req->send(r);
    });

    // PWA manifest
    server.on("/manifest.json", HTTP_GET, [](AsyncWebServerRequest* req) {
        AsyncWebServerResponse* r =
            req->beginResponse_P(200, "application/json", (const uint8_t*)PWA_MANIFEST, strlen_P(PWA_MANIFEST));
        addCorsHeaders(r);
        req->send(r);
    });

    // iOS
    server.on("/hotspot-detect.html", HTTP_GET, redirectToDashboard);
    // Android
    server.on("/generate_204",       HTTP_GET, redirectToDashboard);
    // Windows
    server.on("/redirect",           HTTP_GET, redirectToDashboard);
    server.on("/connecttest.txt",    HTTP_GET, redirectToDashboard);
    // Microsoft
    server.on("/fwlink",             HTTP_GET, redirectToDashboard);

    // ══════════════════════════════════════════════════════════════════════
    //  SYSTEM API
    // ══════════════════════════════════════════════════════════════════════

    server.on("/api/system/info", HTTP_GET, [](AsyncWebServerRequest* req) {
        JsonDocument doc;
        doc["chipModel"]       = ESP.getChipModel();
        doc["chipRevision"]    = ESP.getChipRevision();
        doc["cpuFreqMHz"]      = ESP.getCpuFreqMHz();
        doc["freeHeap"]        = ESP.getFreeHeap();
        doc["totalHeap"]       = ESP.getHeapSize();
        doc["uptime"]          = millis() / 1000;
        doc["flashSize"]       = ESP.getFlashChipSize();
        doc["sdkVersion"]      = ESP.getSdkVersion();
        doc["firmwareVersion"] = "NeoCAT V1.0.0";
        String out;
        serializeJson(doc, out);
        sendJson(req, 200, out);
    });

    // Reboot -----------------------------------------------------------------
    server.on("/api/system/reboot", HTTP_POST, [](AsyncWebServerRequest* req) {
        sendJson(req, 200, "{\"status\":\"rebooting\"}");
        // Wait 1 second so the response gets sent, then reboot
        delay(1000);
        ESP.restart();
    });

    // ══════════════════════════════════════════════════════════════════════
    //  CONFIG API
    // ══════════════════════════════════════════════════════════════════════

    server.on("/api/config", HTTP_GET, [](AsyncWebServerRequest* req) {
        sendJson(req, 200, config.getSettingsJSON());
    });

    AsyncCallbackJsonWebHandler* configHandler = new AsyncCallbackJsonWebHandler(
        "/api/config",
        [](AsyncWebServerRequest* req, JsonVariant& json) {
            JsonObject obj = json.as<JsonObject>();
            if(obj.containsKey("ssid")) config.setSSID(obj["ssid"].as<String>());
            if(obj.containsKey("pass")) config.setPassword(obj["pass"].as<String>());
            if(obj.containsKey("user")) config.setUserName(obj["user"].as<String>());
            if(obj.containsKey("insta")) config.setInstaID(obj["insta"].as<String>());
            sendJson(req, 200, "{\"status\":\"ok\"}");
        });
    server.addHandler(configHandler);

    // ══════════════════════════════════════════════════════════════════════
    //  WIFI API
    // ══════════════════════════════════════════════════════════════════════

    // Start scan -------------------------------------------------------------
    server.on("/api/wifi/scan", HTTP_GET, [](AsyncWebServerRequest* req) {
        if (_wifi) _wifi->startScan();
        sendJson(req, 200, "{\"status\":\"scanning\"}");
    });

    // Get scan results -------------------------------------------------------
    server.on("/api/wifi/results", HTTP_GET, [](AsyncWebServerRequest* req) {
        if (!_wifi) { sendJson(req, 500, "{\"error\":\"no wifi module\"}"); return; }
        sendJson(req, 200, _wifi->getScanResultsJSON());
    });

    // Deauth -----------------------------------------------------------------
    AsyncCallbackJsonWebHandler* deauthHandler =
        new AsyncCallbackJsonWebHandler("/api/wifi/deauth",
            [](AsyncWebServerRequest* req, JsonVariant& json) {
                if (!_wifi) { sendJson(req, 500, "{\"error\":\"no wifi module\"}"); return; }

                JsonObject obj = json.as<JsonObject>();
                if (obj["target"].is<const char*>()) {
                    String t = obj["target"].as<String>();
                    if (t == "all") {
                        _wifi->startDeauthAll();
                        sendJson(req, 200, "{\"status\":\"deauth_all_started\"}");
                    } else {
                        sendJson(req, 400, "{\"error\":\"invalid target\"}");
                    }
                } else if (obj["target"].is<int>()) {
                    int idx = obj["target"].as<int>();
                    _wifi->startDeauth(idx);
                    String r = "{\"status\":\"deauth_started\",\"target\":" + String(idx) + "}";
                    sendJson(req, 200, r);
                } else {
                    sendJson(req, 400, "{\"error\":\"missing target\"}");
                }
            });
    deauthHandler->setMethod(HTTP_POST);
    server.addHandler(deauthHandler);

    // Beacon flood -----------------------------------------------------------
    AsyncCallbackJsonWebHandler* beaconHandler =
        new AsyncCallbackJsonWebHandler("/api/wifi/beacon",
            [](AsyncWebServerRequest* req, JsonVariant& json) {
                if (!_wifi) { sendJson(req, 500, "{\"error\":\"no wifi module\"}"); return; }
                String ssids = json["ssids"].as<String>();
                _wifi->startBeaconFlood(ssids);
                sendJson(req, 200, "{\"status\":\"beacon_flood_started\"}");
            });
    beaconHandler->setMethod(HTTP_POST);
    server.addHandler(beaconHandler);

    // Rickroll ---------------------------------------------------------------
    AsyncCallbackJsonWebHandler* rickrollHandler =
        new AsyncCallbackJsonWebHandler("/api/wifi/rickroll",
            [](AsyncWebServerRequest* req, JsonVariant& json) {
                if (!_wifi) { sendJson(req, 500, "{\"error\":\"no wifi module\"}"); return; }
                String lyrics = json["lyrics"].as<String>();
                _wifi->startBeaconRickroll(lyrics);
                sendJson(req, 200, "{\"status\":\"rickroll_started\"}");
            });
    rickrollHandler->setMethod(HTTP_POST);
    server.addHandler(rickrollHandler);

    // Probe flood ------------------------------------------------------------
    server.on("/api/wifi/probe", HTTP_POST, [](AsyncWebServerRequest* req) {
        if (!_wifi) { sendJson(req, 500, "{\"error\":\"no wifi module\"}"); return; }
        _wifi->startProbeFlood();
        sendJson(req, 200, "{\"status\":\"probe_flood_started\"}");
    });

    // Stop all WiFi attacks --------------------------------------------------
    server.on("/api/wifi/stop", HTTP_POST, [](AsyncWebServerRequest* req) {
        if (_wifi) _wifi->stopAttack();
        sendJson(req, 200, "{\"status\":\"stopped\"}");
    });

    // WiFi status ------------------------------------------------------------
    server.on("/api/wifi/status", HTTP_GET, [](AsyncWebServerRequest* req) {
        if (!_wifi) { sendJson(req, 500, "{\"error\":\"no wifi module\"}"); return; }
        JsonDocument doc;
        doc["attacking"]   = _wifi->isAttacking();
        doc["attackType"]  = _wifi->getAttackType();
        doc["packetsSent"] = _wifi->getPacketsSent();
        doc["scanActive"]  = _wifi->isScanning();
        doc["networkCount"]= _wifi->getScanCount();
        String out;
        serializeJson(doc, out);
        sendJson(req, 200, out);
    });

    // ══════════════════════════════════════════════════════════════════════
    //  BLE API
    // ══════════════════════════════════════════════════════════════════════

    // Start BLE scan ---------------------------------------------------------
    server.on("/api/ble/scan", HTTP_GET, [](AsyncWebServerRequest* req) {
        if (_ble) _ble->startScan();
        sendJson(req, 200, "{\"status\":\"ble_scanning\"}");
    });

    // BLE scan results -------------------------------------------------------
    server.on("/api/ble/results", HTTP_GET, [](AsyncWebServerRequest* req) {
        if (!_ble) { sendJson(req, 500, "{\"error\":\"no ble module\"}"); return; }
        sendJson(req, 200, _ble->getScanResultsJSON());
    });

    // BLE spam ---------------------------------------------------------------
    AsyncCallbackJsonWebHandler* bleSpamHandler =
        new AsyncCallbackJsonWebHandler("/api/ble/spam",
            [](AsyncWebServerRequest* req, JsonVariant& json) {
                if (!_ble) { sendJson(req, 500, "{\"error\":\"no ble module\"}"); return; }
                String spamType = json["type"].as<String>();
                BLESpamType st = BLE_SPAM_ALL;
                if (spamType == "apple") st = BLE_SPAM_APPLE;
                else if (spamType == "samsung") st = BLE_SPAM_SAMSUNG;
                else if (spamType == "windows") st = BLE_SPAM_WINDOWS;
                _ble->startSpam(st);
                sendJson(req, 200, "{\"status\":\"ble_spam_started\",\"type\":\"" + spamType + "\"}");
            });
    bleSpamHandler->setMethod(HTTP_POST);
    server.addHandler(bleSpamHandler);

    // BLE stop ---------------------------------------------------------------
    server.on("/api/ble/stop", HTTP_POST, [](AsyncWebServerRequest* req) {
        if (_ble) _ble->stopSpam();
        sendJson(req, 200, "{\"status\":\"ble_stopped\"}");
    });

    // BLE status -------------------------------------------------------------
    server.on("/api/ble/status", HTTP_GET, [](AsyncWebServerRequest* req) {
        if (!_ble) { sendJson(req, 500, "{\"error\":\"no ble module\"}"); return; }
        JsonDocument doc;
        doc["spamming"]    = _ble->isSpamming();
        doc["spamType"]    = _ble->getSpamType();
        doc["packetsSent"] = _ble->getSpamCount();
        doc["deviceCount"] = _ble->getDeviceCount();
        String out;
        serializeJson(doc, out);
        sendJson(req, 200, out);
    });

    // ══════════════════════════════════════════════════════════════════════
    //  LED API
    // ══════════════════════════════════════════════════════════════════════

    // LED mode ---------------------------------------------------------------
    AsyncCallbackJsonWebHandler* ledModeHandler =
        new AsyncCallbackJsonWebHandler("/api/led/mode",
            [](AsyncWebServerRequest* req, JsonVariant& json) {
                if (!_led) { sendJson(req, 500, "{\"error\":\"no led\"}"); return; }
                String mode = json["mode"].as<String>();
                LEDMode m = LED_IDLE;
                if (mode == "off") m = LED_OFF;
                else if (mode == "idle") m = LED_IDLE;
                else if (mode == "scanning") m = LED_SCANNING;
                else if (mode == "attacking") m = LED_ATTACKING;
                else if (mode == "ble") m = LED_BLE_ACTIVE;
                else if (mode == "custom") m = LED_CUSTOM;
                _led->setMode(m);
                sendJson(req, 200, "{\"status\":\"led_mode_set\",\"mode\":\"" + mode + "\"}");
            });
    ledModeHandler->setMethod(HTTP_POST);
    server.addHandler(ledModeHandler);

    // LED color --------------------------------------------------------------
    AsyncCallbackJsonWebHandler* ledColorHandler =
        new AsyncCallbackJsonWebHandler("/api/led/color",
            [](AsyncWebServerRequest* req, JsonVariant& json) {
                if (!_led) { sendJson(req, 500, "{\"error\":\"no led\"}"); return; }
                uint8_t r = json["r"] | 0;
                uint8_t g = json["g"] | 0;
                uint8_t b = json["b"] | 0;
                _led->setCustomColor(r, g, b);
                sendJson(req, 200, "{\"status\":\"color_set\"}");
            });
    ledColorHandler->setMethod(HTTP_POST);
    server.addHandler(ledColorHandler);

    // ══════════════════════════════════════════════════════════════════════
    //  OPTIONS pre-flight (CORS)
    // ══════════════════════════════════════════════════════════════════════
    server.on("/api/*", HTTP_OPTIONS, [](AsyncWebServerRequest* req) {
        AsyncWebServerResponse* r = req->beginResponse(204);
        addCorsHeaders(r);
        req->send(r);
    });

    // ══════════════════════════════════════════════════════════════════════
    //  404 catch-all → captive-portal redirect
    // ══════════════════════════════════════════════════════════════════════
    server.onNotFound(redirectToDashboard);

    Serial.println("[WEB] All routes registered");
}

// ══════════════════════════════════════════════════════════════════════════
// Public helpers
// ══════════════════════════════════════════════════════════════════════════

void notifyClients(String message) {
    if (_ws) _ws->textAll(message);
}

void broadcastStatus() {
    if (!_ws || _ws->count() == 0) return;

    unsigned long now = millis();
    if (now - _lastBroadcast < BROADCAST_INTERVAL_MS) return;
    _lastBroadcast = now;

    // Clean up disconnected clients periodically
    _ws->cleanupClients();

    JsonDocument doc;
    doc["type"]     = "status";
    doc["uptime"]   = now / 1000;
    doc["freeHeap"] = ESP.getFreeHeap();

    // CPU temperature
#if CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32
    doc["cpuTemp"]  = temperatureRead();
#endif
    doc["cpuFreq"]  = ESP.getCpuFreqMHz();

    // Chat status
    doc["chatActive"]  = chatSys.isRoomActive();
    doc["chatUsers"]   = chatSys.getUserCount();
    doc["chatRoom"]    = chatSys.getRoomName();

    // WiFi telemetry
    if (_wifi) {
        doc["wifiAttacking"]  = _wifi->isAttacking();
        // Send attack type as human-readable string for dashboard
        switch (_wifi->getAttackType()) {
            case ATTACK_DEAUTH:   doc["wifiAttackType"] = "Deauth"; break;
            case ATTACK_BEACON:   doc["wifiAttackType"] = "Beacon Flood"; break;
            case ATTACK_PROBE:    doc["wifiAttackType"] = "Probe Flood"; break;
            case ATTACK_RICKROLL: doc["wifiAttackType"] = "Rickroll"; break;
            default:              doc["wifiAttackType"] = "None"; break;
        }
        doc["wifiPackets"]    = _wifi->getPacketsSent();
        doc["wifiScanActive"] = _wifi->isScanning();
        doc["wifiNetworks"]   = _wifi->getScanCount();
    }

    // BLE telemetry
    if (_ble) {
        doc["bleSpamming"]  = _ble->isSpamming();
        switch (_ble->getSpamType()) {
            case BLE_SPAM_APPLE:   doc["bleSpamType"] = "Apple"; break;
            case BLE_SPAM_SAMSUNG: doc["bleSpamType"] = "Samsung"; break;
            case BLE_SPAM_WINDOWS: doc["bleSpamType"] = "Windows"; break;
            case BLE_SPAM_ALL:     doc["bleSpamType"] = "All"; break;
            default:               doc["bleSpamType"] = "None"; break;
        }
        doc["blePackets"]   = _ble->getSpamCount();
        doc["bleDevices"]   = _ble->getDeviceCount();
    }

    String out;
    serializeJson(doc, out);
    _ws->textAll(out);
}

// -----------------------------------------------------------------------------
// Terminal Web Socket Setup
// -----------------------------------------------------------------------------
#include "terminal_module.h"

AsyncWebSocket termWs("/term-ws");

void onTermWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("[Term] Client connected: %u\n", client->id());
    String welcome = termMod.getWelcomeBanner();
    String prompt = termMod.getPrompt();
    
    JsonDocument doc;
    doc["output"] = welcome;
    doc["prompt"] = prompt;
    String out; serializeJson(doc, out);
    client->text(out);
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("[Term] Client disconnected: %u\n", client->id());
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      data[len] = 0;
      String payload = (char*)data;
      
      JsonDocument doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (!err && doc.containsKey("cmd")) {
        String cmd = doc["cmd"].as<String>();
        String outMsg = termMod.execute(cmd);
        String prompt = termMod.getPrompt();
        
        JsonDocument resp;
        resp["output"] = outMsg;
        resp["prompt"] = prompt;
        String respStr; serializeJson(resp, respStr);
        client->text(respStr);
      }
    }
  }
}

void setupTerminalRoutes(AsyncWebServer& server) {
  termWs.onEvent(onTermWsEvent);
  server.addHandler(&termWs);
  Serial.println(F("[HTTP] WebSocket /term-ws registered"));
}

// ══════════════════════════════════════════════════════════════════════════
//  CHAT WEBSOCKET  — Dedicated /chat-ws endpoint for real-time messaging
// ══════════════════════════════════════════════════════════════════════════

static AsyncWebSocket _chatWs("/chat-ws");

// ══════════════════════════════════════════════════════════════════════════
//  handleChatSwitch — Execute deferred WiFi AP changes safely from loop()
// ══════════════════════════════════════════════════════════════════════════

void handleChatSwitch() {
    if (_chatStartPending) {
        _chatStartPending = false;
        _chatModeActive = true;
        
        String roomName = chatSys.getRoomName();
        String roomPass = chatSys.getRoomPassword();
        const char* passCstr = roomPass.length() > 0 ? roomPass.c_str() : NULL;

        Serial.printf("\n[CHAT] Switching AP to Room: %s\n", roomName.c_str());
        WiFi.softAPdisconnect(true);
        delay(500); // Give hardware time to settle
        WiFi.softAP(roomName.c_str(), passCstr, 1, 0, CHAT_MAX_USERS);

        chatSys.startRoom();
        Serial.println("[CHAT] Room is now ACTIVE.");
    }
    
    if (_chatStopPending) {
        _chatStopPending = false;
        _chatModeActive = false;
        
        Serial.println("\n[CHAT] Stopping Room and reverting to main AP...");
        chatSys.stopRoom();
        _chatWs.closeAll();

        String ssid = config.getSSID();
        String pass = config.getPassword();
        const char* passCstr = pass.length() > 0 ? pass.c_str() : NULL;

        WiFi.softAPdisconnect(true);
        delay(500); // Give hardware time to settle
        WiFi.softAP(ssid.c_str(), passCstr, 1, 0, 8);

        Serial.printf("[CHAT] AP reverted to: %s\n", ssid.c_str());
    }
}

static void broadcastChatUsers() {
    String usersJson = "{\"type\":\"users\",\"list\":" + chatSys.getUsersJSON() + "}";
    _chatWs.textAll(usersJson);
}

static void broadcastChatMessage(const String& sender, uint8_t avatar,
                                  const String& text, uint32_t time,
                                  uint8_t type, uint32_t msgId, uint32_t senderId) {
    JsonDocument doc;
    doc["type"]   = (type == 1) ? "system" : "message";
    doc["sender"] = sender;
    doc["avatar"] = avatar;
    doc["text"]   = text;
    doc["time"]   = time;
    doc["id"]     = msgId;
    doc["senderId"] = senderId;
    String out;
    serializeJson(doc, out);
    _chatWs.textAll(out);
}

static void onChatWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                           AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("[CHAT-WS] Client #%u connected\n", client->id());
            break;

        case WS_EVT_DISCONNECT: {
            Serial.printf("[CHAT-WS] Client #%u disconnected\n", client->id());
            ChatUser* u = chatSys.getUser(client->id());
            if (u) {
                String leftName = String(u->name);
                chatSys.removeUser(client->id());
                // Broadcast system message about user leaving
                int mc = chatSys.getMessageCount();
                broadcastChatMessage("System", 255, leftName + " left the room",
                                      millis() / 1000, 1, mc - 1, 0);
                broadcastChatUsers();
            }
            break;
        }

        case WS_EVT_DATA: {
            AwsFrameInfo* info = (AwsFrameInfo*)arg;
            if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
                data[len] = '\0';
                JsonDocument cmdDoc;
                DeserializationError err = deserializeJson(cmdDoc, (char*)data);
                if (err) break;

                const char* cmd = cmdDoc["cmd"] | "";

                // ── JOIN ──────────────────────────────────────────────
                if (strcmp(cmd, "join") == 0) {
                    String name = cmdDoc["name"] | "Anon";
                    uint8_t avatar = cmdDoc["avatar"] | 0;

                    if (name.length() == 0) name = "Anon";
                    if (name.length() > CHAT_MAX_NAME_LEN) name = name.substring(0, CHAT_MAX_NAME_LEN);

                    bool added = chatSys.addUser(client->id(), name, avatar, client->remoteIP());
                    if (!added) {
                        client->text("{\"type\":\"error\",\"text\":\"Room is full\"}");
                        break;
                    }

                    // Send welcome with message history and user list
                    JsonDocument welcome;
                    welcome["type"]    = "welcome";
                    welcome["room"]    = chatSys.getRoomName();
                    welcome["emoji"]   = chatSys.getRoomEmoji();
                    welcome["myId"]    = client->id();
                    welcome["isAdmin"] = chatSys.isAdmin(client->id());

                    // Embed messages and users as raw JSON
                    String msgJson = chatSys.getMessagesJSON(0);
                    String usrJson = chatSys.getUsersJSON();

                    String welcomeStr;
                    serializeJson(welcome, welcomeStr);
                    // Inject arrays manually to avoid double-serialization
                    welcomeStr.remove(welcomeStr.length() - 1); // remove trailing }
                    welcomeStr += ",\"messages\":" + msgJson + ",\"users\":" + usrJson + "}";

                    client->text(welcomeStr);

                    // Broadcast updated user list and join system message to everyone
                    int mc = chatSys.getMessageCount();
                    broadcastChatMessage("System", 255, name + " joined the room",
                                          millis() / 1000, 1, mc - 1, 0);
                    broadcastChatUsers();
                }

                // ── MESSAGE ───────────────────────────────────────────
                else if (strcmp(cmd, "msg") == 0) {
                    String text = cmdDoc["text"] | "";
                    if (text.length() == 0) break;
                    if (text.length() > CHAT_MAX_MSG_LEN) text = text.substring(0, CHAT_MAX_MSG_LEN);

                    ChatUser* u = chatSys.getUser(client->id());
                    if (!u) break;

                    chatSys.addMessage(client->id(), text);
                    int mc = chatSys.getMessageCount();
                    broadcastChatMessage(String(u->name), u->avatar, text,
                                          millis() / 1000, 0, mc - 1, client->id());
                }

                // ── PING ──────────────────────────────────────────────
                else if (strcmp(cmd, "ping") == 0) {
                    client->text("{\"type\":\"pong\"}");
                }
            }
            break;
        }

        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

// ══════════════════════════════════════════════════════════════════════════
//  setupChatRoutes — Wire chat HTTP + WS endpoints onto the server
// ══════════════════════════════════════════════════════════════════════════

void setupChatRoutes(AsyncWebServer& server) {
    // ── Chat WebSocket ────────────────────────────────────────────────
    _chatWs.onEvent(onChatWsEvent);
    server.addHandler(&_chatWs);

    // ── Chat page ─────────────────────────────────────────────────────
    server.on("/chat", HTTP_GET, [](AsyncWebServerRequest* req) {
        AsyncWebServerResponse* r =
            req->beginResponse_P(200, "text/html", (const uint8_t*)CHAT_HTML, strlen_P(CHAT_HTML));
        addCorsHeaders(r);
        req->send(r);
    });

    // ── GET /api/chat/room ────────────────────────────────────────────
    server.on("/api/chat/room", HTTP_GET, [](AsyncWebServerRequest* req) {
        sendJson(req, 200, chatSys.getRoomJSON());
    });

    // ── POST /api/chat/room — Create/update room ──────────────────────
    AsyncCallbackJsonWebHandler* roomHandler = new AsyncCallbackJsonWebHandler(
        "/api/chat/room",
        [](AsyncWebServerRequest* req, JsonVariant& json) {
            JsonObject obj = json.as<JsonObject>();
            String name = obj["name"] | chatSys.getRoomName();
            String pass = obj["password"] | chatSys.getRoomPassword();
            if (obj.containsKey("emoji")) {
                chatSys.setRoomEmoji(obj["emoji"].as<String>());
            }
            chatSys.createRoom(name, pass);
            sendJson(req, 200, chatSys.getRoomJSON());
        });
    server.addHandler(roomHandler);

    // ── POST /api/chat/room/start ─────────────────────────────────────
    server.on("/api/chat/room/start", HTTP_POST, [](AsyncWebServerRequest* req) {
        _chatStartPending = true;  // Deferred — handled in handleChatSwitch()
        sendJson(req, 200, "{\"status\":\"starting\"}");
    });

    // ── POST /api/chat/room/stop ──────────────────────────────────────
    server.on("/api/chat/room/stop", HTTP_POST, [](AsyncWebServerRequest* req) {
        _chatStopPending = true;  // Deferred — handled in handleChatSwitch()
        sendJson(req, 200, "{\"status\":\"stopping\"}");
    });

    // ── GET /api/chat/users ───────────────────────────────────────────
    server.on("/api/chat/users", HTTP_GET, [](AsyncWebServerRequest* req) {
        sendJson(req, 200, chatSys.getUsersJSON());
    });

    // ── POST /api/chat/kick ───────────────────────────────────────────
    AsyncCallbackJsonWebHandler* kickHandler = new AsyncCallbackJsonWebHandler(
        "/api/chat/kick",
        [](AsyncWebServerRequest* req, JsonVariant& json) {
            uint32_t clientId = json["clientId"] | 0;
            if (clientId == 0) {
                sendJson(req, 400, "{\"error\":\"missing clientId\"}");
                return;
            }

            ChatUser* u = chatSys.getUser(clientId);
            if (!u) {
                sendJson(req, 404, "{\"error\":\"user not found\"}");
                return;
            }

            String kickedName = String(u->name);
            chatSys.removeUser(clientId);

            // Close the kicked user's WS connection
            AsyncWebSocketClient* wsClient = _chatWs.client(clientId);
            if (wsClient) {
                wsClient->text("{\"type\":\"kicked\",\"reason\":\"Removed by admin\"}");
                wsClient->close();
            }

            // Broadcast
            int mc = chatSys.getMessageCount();
            broadcastChatMessage("System", 255, kickedName + " was kicked",
                                  millis() / 1000, 1, mc - 1, 0);
            broadcastChatUsers();

            sendJson(req, 200, "{\"status\":\"kicked\"}");
        });
    server.addHandler(kickHandler);

    // ── POST /api/chat/clear ──────────────────────────────────────────
    server.on("/api/chat/clear", HTTP_POST, [](AsyncWebServerRequest* req) {
        chatSys.clearMessages();
        _chatWs.textAll("{\"type\":\"system\",\"sender\":\"System\",\"avatar\":255,\"text\":\"Messages cleared by admin\",\"time\":" + String(millis() / 1000) + ",\"id\":0,\"senderId\":0}");
        sendJson(req, 200, "{\"status\":\"cleared\"}");
    });

    // ── GET /api/chat/stats ───────────────────────────────────────────
    server.on("/api/chat/stats", HTTP_GET, [](AsyncWebServerRequest* req) {
        sendJson(req, 200, chatSys.getStatsJSON());
    });
}

