/**
 * led_controller.h - NeoCAT v1 LED Controller
 * 
 * Drives a single WS2812 NeoPixel on GPIO 7 with non-blocking
 * animations for system status feedback.
 * 
 * Modes:
 *   BOOT      – one-time blocking startup animation
 *   IDLE      – slow cyan breathing
 *   SCANNING  – fast rainbow cycle
 *   ATTACKING – fast red breathing
 *   BLE_ACTIVE – purple breathing
 *   SUCCESS   – solid green (2 s then revert)
 *   ERROR     – solid red   (2 s then revert)
 *   CUSTOM    – user-defined solid color
 * 
 * Part of the NeoCAT v1 firmware.
 */

#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   7
#define LED_COUNT 1

enum LEDMode {
  LED_OFF = 0,
  LED_BOOT,
  LED_IDLE,
  LED_SCANNING,
  LED_ATTACKING,
  LED_BLE_ACTIVE,
  LED_SUCCESS,
  LED_ERROR,
  LED_CUSTOM
};

class LEDController {
public:
  void begin();
  void update();                                         ///< Call every loop iteration
  void setMode(LEDMode mode);
  void setCustomColor(uint8_t r, uint8_t g, uint8_t b);
  void bootAnimation();                                  ///< Blocking ~2 s boot sequence
  LEDMode getMode();
  String getStatusJSON();

private:
  Adafruit_NeoPixel _strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

  LEDMode  _mode        = LED_OFF;
  LEDMode  _prevMode    = LED_IDLE;   ///< Restored after SUCCESS / ERROR
  uint8_t  _customR     = 0;
  uint8_t  _customG     = 0;
  uint8_t  _customB     = 0;
  unsigned long _lastUpdate   = 0;
  unsigned long _modeSetTime  = 0;    ///< Timestamp when current mode was set
  float    _breathPhase = 0;
  float    _hue         = 0;

  /// Returns color with brightness modulated by a sin-wave
  uint32_t _breathe(uint8_t r, uint8_t g, uint8_t b, float speed);

  /// Returns an HSV color that cycles through the hue wheel
  uint32_t _rainbow(float speed);

  /// Write a single color to the strip and show
  void _setPixel(uint32_t color);
};
