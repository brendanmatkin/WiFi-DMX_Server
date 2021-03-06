#ifndef SETTINGS_H
#define SETTINGS_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <FastLED.h>

#include "wificonfig.h"   
// to track and stop tracking config.h: 
// git update-index --assume-unchanged FILE_NAME 
// git update-index --no-assume-unchanged FILE_NAME



//#define DBG_OUTPUT_PORT Serial
const bool SERIAL_DEBUG = true;
#define DMX_AP true



/* -- led settings -- */
//#define DEFAULT_HUE       160
#define DMX_PERIOD        25.0f               // 25ms = 40 fps
#define MAX_FIXTURES      40                  // maximum number of pixels to control (currently considering each fixture 1 pixel because zygotes).
//uint8_t currentFixtures = 8;
CHSV leds[MAX_FIXTURES];
CRGB whiteLeds[MAX_FIXTURES];




/* -- firmware settings -- */
#define HW_VERSION "0.0.0"
#define HW_PHASE   ""
const int FW_VERSION = 101;                   // 0.1.00, convention: 1.2.10 = 1210, 0.5.9 = 509, no leading 0s or it is interpreted as octal.. learned that the hard way!
#define FW_PHASE   "-alpha"
float fwCheckButtonTime = 2000.0f;            // how long to hold button down.
//const char* fwUrlBase = "http://10.0.3.100/";
//const char* fwName = "field_fw";
//bool sendFirmwareVersion = false;             // send firmware via OSC to 8888 for testing



/* -- network settings -- */
uint8_t address   = 99;                       // set by DIPs (if you see 99 then it's wrong!)
String deviceName = "DMX_Server";             // used for DHCP
const char* host  = "dmx";                    // used for mDNS
const char* ssid  = "your-ssid";              // unused for now
const char* password = "your-password";       // unused for now
const char* ssid_AP  = WIFI_SSID;             // see wificonfig.h
const char* password_AP = WIFI_PASSWORD;      // see wificonigh.h - **should be at least 8 characters!**

IPAddress local_IP(10, 0, 0, 1);              // static IP  - use staticIP[index] to get & set individual octets
IPAddress gateway(10, 0, 0, 1);               // gateway (for static)
IPAddress subnet(255, 255, 255, 0);           // subnet (for static)


/* global variables */
float sdc_speed = DMX_PERIOD;
uint8_t sdc_delay = 0;
int hueCycleSpeed = 0;
uint32_t hueTimer;
//unsigned long frameCounter = 0;
uint8_t brightness = 255;

enum Mode {
  SDC,
  MANUAL,
  AUTO
} mode = SDC;

struct State {
    uint8_t speed       = 127;
    uint8_t delay       = 0;
    uint8_t hue         = 160;
    Mode    mode        = SDC;
    uint8_t hue_cycle   = 0;
    uint8_t brightness  = 255;
    uint8_t white       = 0;
    uint8_t preset      = 0;
    uint8_t num_devices = 8;
} state;

#define NUM_PRESETS 3
struct State presets[NUM_PRESETS];
bool savePresetFlag = false;

#endif /* SETTINGS_H */
