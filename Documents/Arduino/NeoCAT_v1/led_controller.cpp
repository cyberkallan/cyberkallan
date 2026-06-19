/**
 * led_controller.cpp - NeoCAT v1 LED Controller Implementation
 * 
 * Non-blocking NeoPixel animations for system status indication.
 * Runs at ~50 fps (20 ms update interval) using millis() timing.
 * 
 * Part of the NeoCAT v1 firmware.
 */

#include "led_controller.h"
#include <ArduinoJson.h>
#include <math.h>

// Animation timing constants
static const unsigned long UPDATE_INTERVAL_MS = 20;   // 50 fps
static const unsigned long STATUS_DURATION_MS = 2000;  // SUCCESS / ERROR display time

// Breathing speeds (radians per update tick)
static const float BREATH_SLOW    = 0.042f;   // ~3 s period  (2π / (3000/20))
static const float BREATH_FAST    = 0.251f;   // ~0.5 s period (2π / (500/20))
static const float BREATH_MEDIUM  = 0.084f;   // ~1.5 s period
static const float RAINBOW_SPEED  = 512.0f;   // Hue increment per tick (out of 65535)

// ═══════════════════════════════════════════════════════════════
//  Public API
// ═══════════════════════════════════════════════════════════════

/**
 * Initialize the NeoPixel hardware.
 */
void LEDController::begin() {
  _strip.begin();
  _strip.setBrightness(50);
  _strip.clear();
  _strip.show();
  _mode = LED_OFF;
  Serial.println("[LED] Controller initialized.");
}

/**
 * Blocking boot animation (~2 seconds total).
 * 
 * Sequence:
 *   1. Fade from black → cyan over 500 ms
 *   2. Flash white 3×  (100 ms on / 100 ms off each)
 *   3. Fade from white → idle cyan over 200 ms
 */
void LEDController::bootAnimation() {
  Serial.println("[LED] Boot animation starting...");

  // Phase 1: Fade black → cyan (500 ms, ~50 steps)
  for (int i = 0; i <= 50; i++) {
    float t = (float)i / 50.0f;
    uint8_t g = (uint8_t)(255.0f * t);
    uint8_t b = (uint8_t)(255.0f * t);
    _setPixel(_strip.Color(0, g, b));
    delay(10);  // 50 × 10 ms = 500 ms
  }

  // Phase 2: Flash white 3 times (3 × 200 ms = 600 ms)
  for (int flash = 0; flash < 3; flash++) {
    _setPixel(_strip.Color(255, 255, 255));
    delay(100);
    _setPixel(_strip.Color(0, 0, 0));
    delay(100);
  }

  // Phase 3: Fade back to idle cyan (200 ms, ~20 steps)
  for (int i = 0; i <= 20; i++) {
    float t = (float)i / 20.0f;
    uint8_t g = (uint8_t)(255.0f * t);
    uint8_t b = (uint8_t)(255.0f * t);
    _setPixel(_strip.Color(0, g, b));
    delay(10);  // 20 × 10 ms = 200 ms
  }

  // Settle into idle mode
  _mode       = LED_IDLE;
  _prevMode   = LED_IDLE;
  _breathPhase = 0;

  Serial.println("[LED] Boot animation complete.");
}

/**
 * Set the current LED mode.
 * SUCCESS and ERROR are temporary — they auto-revert after 2 s.
 */
void LEDController::setMode(LEDMode mode) {
  // For transient modes, remember what to revert to
  if (mode == LED_SUCCESS || mode == LED_ERROR) {
    if (_mode != LED_SUCCESS && _mode != LED_ERROR) {
      _prevMode = _mode;  // Save current non-transient mode
    }
  }

  _mode        = mode;
  _modeSetTime = millis();
  _breathPhase = 0;
  _hue         = 0;
}

LEDMode LEDController::getMode() {
  return _mode;
}

/**
 * Set a user-defined solid color for LED_CUSTOM mode.
 */
void LEDController::setCustomColor(uint8_t r, uint8_t g, uint8_t b) {
  _customR = r;
  _customG = g;
  _customB = b;
}

/**
 * Return LED status as JSON:
 * { mode, brightness }
 */
String LEDController::getStatusJSON() {
  JsonDocument doc;

  const char* modeStr = "off";
  switch (_mode) {
    case LED_OFF:        modeStr = "off";        break;
    case LED_BOOT:       modeStr = "boot";       break;
    case LED_IDLE:       modeStr = "idle";        break;
    case LED_SCANNING:   modeStr = "scanning";    break;
    case LED_ATTACKING:  modeStr = "attacking";   break;
    case LED_BLE_ACTIVE: modeStr = "ble_active";  break;
    case LED_SUCCESS:    modeStr = "success";     break;
    case LED_ERROR:      modeStr = "error";       break;
    case LED_CUSTOM:     modeStr = "custom";      break;
  }

  doc["mode"]       = modeStr;
  doc["brightness"] = _strip.getBrightness();

  String out;
  serializeJson(doc, out);
  return out;
}

// ═══════════════════════════════════════════════════════════════
//  Main update loop — non-blocking
// ═══════════════════════════════════════════════════════════════

void LEDController::update() {
  unsigned long now = millis();

  // Throttle to ~50 fps
  if (now - _lastUpdate < UPDATE_INTERVAL_MS) return;
  _lastUpdate = now;

  switch (_mode) {
    case LED_OFF:
      _setPixel(0);
      break;

    case LED_BOOT:
      // Boot animation is blocking — this shouldn't be reached,
      // but show cyan as a fallback.
      _setPixel(_strip.Color(0, 255, 255));
      break;

    case LED_IDLE:
      // Slow cyan breathing (~3 s period)
      _setPixel(_breathe(0, 255, 255, BREATH_SLOW));
      break;

    case LED_SCANNING:
      // Fast rainbow hue cycle
      _setPixel(_rainbow(RAINBOW_SPEED));
      break;

    case LED_ATTACKING:
      // Fast red breathing (~0.5 s period)
      _setPixel(_breathe(255, 0, 0, BREATH_FAST));
      break;

    case LED_BLE_ACTIVE:
      // Medium purple breathing
      _setPixel(_breathe(138, 43, 226, BREATH_MEDIUM));
      break;

    case LED_SUCCESS:
      // Solid green for 2 s, then revert
      _setPixel(_strip.Color(0, 255, 0));
      if (now - _modeSetTime >= STATUS_DURATION_MS) {
        _mode = _prevMode;
        _breathPhase = 0;
      }
      break;

    case LED_ERROR:
      // Solid red for 2 s, then revert
      _setPixel(_strip.Color(255, 0, 0));
      if (now - _modeSetTime >= STATUS_DURATION_MS) {
        _mode = _prevMode;
        _breathPhase = 0;
      }
      break;

    case LED_CUSTOM:
      _setPixel(_strip.Color(_customR, _customG, _customB));
      break;
  }
}

// ═══════════════════════════════════════════════════════════════
//  Private helpers
// ═══════════════════════════════════════════════════════════════

/**
 * Compute a breathing (pulsing) color using a sine wave.
 * 
 * @param r,g,b   Base color at full brightness.
 * @param speed   Phase increment per update tick (radians).
 * @return        NeoPixel packed color with modulated brightness.
 */
uint32_t LEDController::_breathe(uint8_t r, uint8_t g, uint8_t b, float speed) {
  _breathPhase += speed;
  if (_breathPhase > 6.2831853f) {  // 2π
    _breathPhase -= 6.2831853f;
  }

  // Map sin() output from [-1, 1] → [0, 1]
  float brightness = (sinf(_breathPhase) + 1.0f) / 2.0f;

  // Apply a minimum floor so the LED never fully turns off
  brightness = 0.05f + brightness * 0.95f;

  return _strip.Color(
    (uint8_t)(r * brightness),
    (uint8_t)(g * brightness),
    (uint8_t)(b * brightness)
  );
}

/**
 * Cycle through the HSV hue wheel at the given speed.
 * 
 * @param speed   Hue increment per tick (0–65535 range).
 * @return        NeoPixel packed color at full saturation / value.
 */
uint32_t LEDController::_rainbow(float speed) {
  _hue += speed;
  if (_hue >= 65536.0f) {
    _hue -= 65536.0f;
  }
  return _strip.ColorHSV((uint16_t)_hue, 255, 255);
}

/**
 * Write a single color to pixel 0 and push to hardware.
 */
void LEDController::_setPixel(uint32_t color) {
  _strip.setPixelColor(0, color);
  _strip.show();
}
