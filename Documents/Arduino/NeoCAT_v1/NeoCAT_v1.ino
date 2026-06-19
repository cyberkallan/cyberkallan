/*
 * ============================================================================
 *  NeoCAT V1 — ESP32-S3 WiFi/BLE Pentesting Firmware
 *  Target Board: PCBCupid Glyph S3 (8MB Flash, No PSRAM)
 *  Hardware: WS2812B on GPIO7, BOOT button on GPIO9
 * ============================================================================
 *
 *  Captive-portal based web UI for WiFi and BLE reconnaissance & attacks.
 *  All traffic is redirected to 192.168.4.1 via DNS hijacking.
 *
 *  Partition: custom 8MB layout — see partitions.csv
 *  Board config: ESP32-S3 Dev Module, Flash Size 8MB, Partition Scheme Custom
 *
 *  DISCLAIMER: This firmware is provided for EDUCATIONAL and AUTHORIZED
 *  SECURITY TESTING purposes only. Unauthorized use against networks you
 *  do not own or have explicit permission to test is illegal.
 * ============================================================================
 */

#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "wifi_module.h"
#include "ble_module.h"
#include "led_controller.h"
#include "web_server.h"
#include "terminal_module.h"
#include "config_manager.h"
#include "chat_system.h"

// =========================== Configuration ==================================
// Settings loaded from ConfigManager

static const uint8_t AP_CHANNEL   = 1;
static const uint8_t AP_MAX_CONN  = 8;

static const IPAddress AP_IP(192, 168, 4, 1);
static const IPAddress AP_GW(192, 168, 4, 1);
static const IPAddress AP_MASK(255, 255, 255, 0);

static const uint16_t DNS_PORT    = 53;
static const uint16_t HTTP_PORT   = 80;

// =========================== Battery Config =================================
static const uint8_t BATTERY_ADC_PIN = 0;  // GPIO0 (A0) on PCBCupid Glyph S3
// ESP32 ADC is 12-bit (0-4095). Reference voltage is 3.3V, but it usually
// measures up to ~3.1V without attenuation.
// Assuming a standard voltage divider (e.g. 100k/100k -> 0.5 ratio):
// Vbat = (adc_val / 4095.0) * 3.3 * 2.0
// We'll use a calibration multiplier. Adjust if needed.
static const float ADC_MULTIPLIER = 0.00171f; // (3.3 / 4095) * 2.12 (approx calibration)

float g_batteryVoltage = 0.0f;
int   g_batteryPercent = 0;
bool  g_isCharging     = false;

// =========================== Global Objects =================================
DNSServer       dnsServer;
AsyncWebServer  server(HTTP_PORT);
AsyncWebSocket  ws("/ws");

WiFiModule      wifiMod;
BLEModule       bleMod;
LEDController   ledCtrl;

// =========================== FreeRTOS Handles ===============================
static TaskHandle_t ledTaskHandle   = NULL;
static TaskHandle_t coreTaskHandle  = NULL;

// Forward declarations for FreeRTOS tasks
void ledTask(void* pvParameters);
void coreLoopTask(void* pvParameters);
void updateBatteryStatus();

// ============================================================================
//  LED Animation Task — Runs on Core 0 to keep animations smooth
// ============================================================================
void ledTask(void* pvParameters) {
  (void)pvParameters;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(16);  // ~60 FPS

  for (;;) {
    ledCtrl.update();
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}

// ============================================================================
//  Core Processing Task — Handles attacks/scanning on Core 1
// ============================================================================
void coreLoopTask(void* pvParameters) {
  (void)pvParameters;

  for (;;) {
    // Process WiFi attacks / scanning at maximum rate
    wifiMod.update();

    // Process BLE operations
    bleMod.update();

    // Minimal yield — allow other tasks, but keep attack rate high
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

// ============================================================================
//  SETUP
// ============================================================================
void setup() {
  // ---------- Serial ----------
  Serial.begin(115200);
  delay(500);  // Allow USB-CDC to stabilize on S3
  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("  NeoCAT V1 — ESP32-S3 Pentesting FW"));
  Serial.println(F("  PCBCupid Glyph S3 Board"));
  Serial.println(F("========================================"));
  Serial.printf("  ESP-IDF: %s\n", esp_get_idf_version());
  Serial.printf("  Free heap: %u bytes\n", ESP.getFreeHeap());
  Serial.printf("  Flash size: %u MB\n", ESP.getFlashChipSize() / (1024 * 1024));
  Serial.printf("  CPU freq: %u MHz\n", getCpuFrequencyMhz());
  Serial.println();

  // ---------- LED Controller ----------
  Serial.println(F("[INIT] LED controller..."));
  ledCtrl.begin();

  // ---------- Hardware Factory Reset ----------
  pinMode(0, INPUT_PULLUP);
  if (digitalRead(0) == LOW) {
    Serial.println(F("[INIT] BOOT button held. Waiting 5s for Factory Reset..."));
    ledCtrl.setMode(LED_ERROR); // Red breathing to indicate danger
    uint32_t t = millis();
    bool doReset = true;
    while(millis() - t < 5000) {
      if(digitalRead(0) == HIGH) { doReset = false; break; }
      ledCtrl.update();
      delay(10);
    }
    if(doReset) {
      Serial.println(F("[INIT] FACTORY RESET TRIGGERED. Erasing settings..."));
      config.begin();
      config.factoryReset();
      ledCtrl.setMode(LED_SUCCESS); // Green to indicate success
      for(int i=0;i<100;i++){ ledCtrl.update(); delay(20); } // 2 seconds
      Serial.println(F("Rebooting..."));
      ESP.restart();
    } else {
      Serial.println(F("[INIT] Factory reset aborted."));
    }
  }
  
  ledCtrl.setMode(LED_BOOT);  // Resume Boot animation

  // ---------- Config Manager ----------
  Serial.println(F("[INIT] Configuration manager..."));
  config.begin();

  // ---------- WiFi Access Point ----------
  Serial.println(F("[INIT] WiFi Access Point..."));
  WiFi.mode(WIFI_AP_STA);  // AP + STA for scanning capability
  WiFi.softAPConfig(AP_IP, AP_GW, AP_MASK);

  String ssid = config.getSSID();
  String pass = config.getPassword();
  const char* pass_cstr = pass.length() > 0 ? pass.c_str() : NULL;

  if (!WiFi.softAP(ssid.c_str(), pass_cstr, AP_CHANNEL, 0, AP_MAX_CONN)) {
    Serial.println(F("[ERROR] Failed to start WiFi AP!"));
    ledCtrl.setMode(LED_ERROR);
    while (true) { delay(1000); }  // Halt
  }

  Serial.printf("[INIT] AP SSID: %s\n", ssid.c_str());
  Serial.printf("[INIT] AP IP:   %s\n", WiFi.softAPIP().toString().c_str());

  // ---------- DNS Server (Captive Portal) ----------
  Serial.println(F("[INIT] DNS captive portal..."));
  // Redirect ALL DNS queries to our AP IP
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  if (!dnsServer.start(DNS_PORT, "*", AP_IP)) {
    Serial.println(F("[WARN] DNS server failed to start"));
  }

  // ---------- WebSocket ----------
  Serial.println(F("[INIT] WebSocket on /ws..."));
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // ---------- Web Server ----------
  Serial.println(F("[INIT] Web Server..."));
  setupWebServer(server, ws, wifiMod, bleMod, ledCtrl);

  // ---------- Terminal System ----------
  Serial.println(F("[INIT] Terminal subsystem & LittleFS..."));
  termMod.begin(&wifiMod, &bleMod, &ledCtrl);
  setupTerminalRoutes(server);

  // ---------- Chat Routes ----------
  Serial.println(F("[INIT] Chat routes & WebSocket..."));
  setupChatRoutes(server);

  // ---------- Clone AP Routes ----------
  Serial.println(F("[INIT] Clone AP routes..."));
  setupCloneRoutes(server);

  // ---------- Start Services ----------

  // Handle captive portal detection endpoints
  server.onNotFound([](AsyncWebServerRequest* request) {
    if (isCloneMode()) {
      request->redirect("http://192.168.4.1/portal");
    } else if (isChatMode()) {
      request->redirect("http://192.168.4.1/chat");
    } else {
      // Redirect all unknown requests to root (captive portal)
      request->redirect("http://" + WiFi.softAPIP().toString() + "/");
    }
  });

  server.begin();
  Serial.println(F("[INIT] Web server started"));

  // ---------- WiFi Module ----------
  Serial.println(F("[INIT] WiFi pentesting module..."));
  wifiMod.begin();

  // ---------- BLE Module ----------
  Serial.println(F("[INIT] BLE module..."));
  bleMod.begin();

  // ---------- Chat System ----------
  Serial.println(F("[INIT] Chat system..."));
  chatSys.begin();

  // ---------- FreeRTOS Tasks ----------
  Serial.println(F("[INIT] Creating FreeRTOS tasks..."));

  // LED task on Core 0 — low priority, smooth animations
  xTaskCreatePinnedToCore(
    ledTask,           // Task function
    "LED_Task",        // Name
    2048,              // Stack size (bytes)
    NULL,              // Parameters
    1,                 // Priority (low)
    &ledTaskHandle,    // Task handle
    0                  // Core 0
  );

  // Core processing task on Core 1 — high priority for attack timing
  xTaskCreatePinnedToCore(
    coreLoopTask,      // Task function
    "Core_Task",       // Name
    8192,              // Stack size (bytes)
    NULL,              // Parameters
    2,                 // Priority (above normal)
    &coreTaskHandle,   // Task handle
    1                  // Core 1
  );

  // ---------- Ready ----------
  ledCtrl.setMode(LED_IDLE);

  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("  NeoCAT V1 READY"));
  Serial.printf("  Connect to WiFi: %s\n", config.getSSID().c_str());
  Serial.printf("  Open browser:    http://%s/\n", WiFi.softAPIP().toString().c_str());
  Serial.printf("  Free heap:       %u bytes\n", ESP.getFreeHeap());
  Serial.println(F("========================================"));
  Serial.println();
}

// ============================================================================
//  LOOP — Runs on Core 1 (Arduino default)
//  Handles DNS, WebSocket cleanup, and periodic status
// ============================================================================
void loop() {
  // Process DNS requests for captive portal
  dnsServer.processNextRequest();

  // Handle deferred chat mode AP switches
  handleChatSwitch();

  // Handle deferred clone mode AP switches
  handleCloneSwitch();

  // Update battery telemetry
  updateBatteryStatus();

  // Periodic status broadcast over WebSocket (uses broadcastStatus from web_server.h)
  broadcastStatus();

  // Heap monitoring (warn if low)
  static unsigned long lastHeapCheck = 0;
  if (millis() - lastHeapCheck > 10000) {
    uint32_t freeHeap = ESP.getFreeHeap();
    if (freeHeap < 20000) {
      Serial.printf("[WARN] Low heap: %u bytes\n", freeHeap);
      ledCtrl.setMode(LED_ERROR);
    }
    lastHeapCheck = millis();
  }

  // Small yield
  delay(5);
}

// ============================================================================
//  BATTERY MONITORING TASK (runs periodically in loop)
// ============================================================================
void updateBatteryStatus() {
  static unsigned long lastBatteryRead = 0;
  if (millis() - lastBatteryRead > 2000) {  // Update every 2 seconds
    lastBatteryRead = millis();
    
    // Read raw ADC (take average of 5 samples for stability)
    uint32_t adcSum = 0;
    for (int i = 0; i < 5; i++) {
      adcSum += analogRead(BATTERY_ADC_PIN);
      delay(2);
    }
    float avgAdc = adcSum / 5.0f;
    
    // Calculate Voltage
    g_batteryVoltage = avgAdc * ADC_MULTIPLIER;
    
    // Determine charging status (approximate based on voltage threshold)
    g_isCharging = (g_batteryVoltage >= 4.25f);
    
    // Calculate Percentage (LiPo standard: 3.2V = 0%, 4.2V = 100%)
    if (g_batteryVoltage >= 4.20f) {
      g_batteryPercent = 100;
    } else if (g_batteryVoltage <= 3.20f) {
      g_batteryPercent = 0;
    } else {
      g_batteryPercent = (int)(((g_batteryVoltage - 3.2f) / (4.2f - 3.2f)) * 100.0f);
    }
  }
}

// ============================================================================
//  WebSocket Event Handler
// ============================================================================
void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
               AwsEventType type, void* arg, uint8_t* data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("[WS] Client #%u connected from %s\n",
                     client->id(), client->remoteIP().toString().c_str());
      // Send initial state
      {
        JsonDocument doc;
        doc["type"] = "hello";
        doc["version"] = "1.0.0";
        doc["freeHeap"] = ESP.getFreeHeap();
        doc["uptime"] = millis() / 1000;
        doc["userName"] = config.getUserName();
        doc["instaId"] = config.getInstaID();
        String msg;
        serializeJson(doc, msg);
        client->text(msg);
      }
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("[WS] Client #%u disconnected\n", client->id());
      break;

    case WS_EVT_DATA:
      {
        AwsFrameInfo* info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
          // Null-terminate the data
          char* msgBuf = (char*)malloc(len + 1);
          if (msgBuf) {
            memcpy(msgBuf, data, len);
            msgBuf[len] = '\0';
            // Websocket processing delegated to API routes directly for now
            // handleWsMessage(client, String(msgBuf), wifiMod, bleMod, ledCtrl);
            free(msgBuf);
          }
        }
      }
      break;

    case WS_EVT_PONG:
      break;

    case WS_EVT_ERROR:
      Serial.printf("[WS] Client #%u error(%u): %s\n",
                     client->id(), *((uint16_t*)arg), (char*)data);
      break;
  }
}
