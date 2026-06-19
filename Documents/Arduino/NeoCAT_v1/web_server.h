#pragma once
/**
 * @file web_server.h
 * @brief NeoCAT V1 — Async Web Server & WebSocket Interface
 * 
 * Declares the HTTP/WS API surface used by the cyberpunk dashboard.
 * All endpoints are CORS-enabled and JSON-based.
 */

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include "wifi_module.h"
#include "ble_module.h"
#include "led_controller.h"

// ---------------------------------------------------------------------------
// Global Battery State (updated by NeoCAT_v1.ino loop)
// ---------------------------------------------------------------------------
extern float g_batteryVoltage;
extern int   g_batteryPercent;
extern bool  g_isCharging;

// ---------------------------------------------------------------------------
// Global WebSocket reference (shared with other modules for push messages)
// ---------------------------------------------------------------------------
extern AsyncWebSocket ws;

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

/**
 * @brief Wire every HTTP route and the WebSocket handler onto `server`.
 *
 * Must be called once in setup() after WiFi AP is up.
 */
void setupWebServer(AsyncWebServer& server, AsyncWebSocket& ws, 
                    WiFiModule& wifiMod, BLEModule& bleMod, LEDController& ledCtrl);
void setupTerminalRoutes(AsyncWebServer& server);

/**
 * @brief Wire all chat HTTP routes and /chat-ws WebSocket onto `server`.
 */
void setupChatRoutes(AsyncWebServer& server);

/**
 * @brief Wire clone AP HTTP routes and captive portal onto `server`.
 */
void setupCloneRoutes(AsyncWebServer& server);

/**
 * @brief Push a plain-text message to every connected WebSocket client.
 */
void notifyClients(String message);

/**
 * @brief Broadcast a JSON status frame to all WS clients.
 *
 * Called from loop(); internally rate-limited to every 500 ms.
 */
void broadcastStatus();

/**
 * @brief Handle deferred WiFi AP switching for Chat Mode
 *
 * Must be called in loop()
 */
void handleChatSwitch();

/**
 * @brief Check if the device is currently in chat mode
 */
bool isChatMode();

/**
 * @brief Handle deferred WiFi AP switching for Clone Mode
 *
 * Must be called in loop()
 */
void handleCloneSwitch();

/**
 * @brief Check if the device is currently in clone AP mode
 */
bool isCloneMode();
