/**
 * Defaults for different boards
 */
#include <Arduino.h>

#include "Version.h"

#ifndef SONOFF_BASIC
#include <Wire.h>
#include <SPI.h>
#endif

#ifndef SCREEN_ADDRESS
#define SCREEN_ADDRESS 0x3C
#endif

#ifdef ARDUINO_ESP8266_HELTEC_WIFI_KIT_8
// Serial.println("ARDUINO_ESP8266_HELTEC_WIFI_KIT_8");
//  0.91" OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 16
//  HELTEC WiFi Kit 8 does not have a built-in LED, so we define it as GPIO 2 (D4)
#define LED_BUILTIN 2
#else
// Generic 0.96" OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1
#endif

#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#endif

// Button is on GPIO 0 by default
#ifndef DEFAULT_BUTTON_PIN
#define DEFAULT_BUTTON_PIN 0
#endif
// Long Press delay - to reset WiFi settings [s]
#define DEFAULT_BUTTON_LONG_PRESS 5
// default PWM frequency for dimmers
#define DEFAULT_PWM_FREQ 600
// ===

#ifdef ESP32
#define ESP_getChipId() ((uint32_t)ESP.getEfuseMac())
#define CHIP_ARC "ESP32"
#else
#define ESP_getChipId() (ESP.getChipId())
#define CHIP_ARC "ESP8266"
#endif
#define CHIP_ID (String(ESP_getChipId(), HEX))

// ESP32 DMX pins
// it is recommended to put these definitions into platformio.ini so they could be board specific
#ifndef DMX_TX_PIN
#define DMX_TX_PIN 17
#endif
// RX pin is not used in this project but it is required by the library
#ifndef DMX_RX_PIN
#define DMX_RX_PIN 16
#endif
// Enable pin is not used in this project but it is required by the library
#ifndef DMX_ENABLE_PIN
#define DMX_ENABLE_PIN 21
#endif
#ifndef DMX_PORT
#define DMX_PORT 1
#endif
