

// esp8266 2.4.0-rc2
// https://tttapa.github.io/ESP8266/Chap14%20-%20WebSocket.html

extern "C" {
#include "user_interface.h"   // allows wifi_set_sleep_type
}

#include "settings.h"
#include "comm_ws.h"
#include <FS.h>
#include <ESPDMX.h>                     //https://github.com/Rickgg/ESP-Dmx      // dmx.write(ch 1-512, val 0-255), dmx.update()
DMXESPSerial dmx;                       // dmx.write(ch 1-512, val 0-255), dmx.update()

#define LED_BUILTIN   2                 // not correctly mapped for ESP-12x
#define BOOTLOAD_PIN  0                 // BOOTLOAD button

ESP8266WebServer server(80);            // http server




void setZygoteDMX( uint8_t fixture, uint8_t red, uint8_t green, uint8_t blue, uint8_t white ) {
  int _offset = fixture * 5;
  dmx.write( _offset + 1, red );
  dmx.write( _offset + 2, green );
  dmx.write( _offset + 3, blue );
  dmx.write( _offset + 4, white );
  //dmx.write( _offset + 5, strobe );
  //  dmx.update();
  //if (SERIAL_DEBUG) Serial.printf("[dmx] Off: %u, R: %u, G: %u, B: %u, W: %u\n", _offset, red, green, blue, white);
  //if (SERIAL_DEBUG && fixture == 0) Serial.printf("[dmx] Off: %u, R: %u, G: %u, B: %u, W: %u\n", _offset, red, green, blue, white);
}



void setup() {
  wifi_set_sleep_type(NONE_SLEEP_T);    // disable sleep (added this when trying to fix analogRead issue.. )

  Serial.begin(115200);
  Serial.printf("\n");
  delay(500);
  Serial.printf("\n\n\nWiFi to DMX\n");
  Serial.printf("FW: %u%s   ", FW_VERSION, FW_PHASE);
  Serial.printf("HW: %s%s\n", HW_VERSION, HW_PHASE);
  Serial.printf("(c)2018 Brendan Matkin\n\n");    // pick open source license
  Serial.flush();

  /* init preset states */
  // do this so we don't have any empty structs.. 
  for (int i = 0; i < NUM_PRESETS; i++) {
    presets[i] = state;                     // copy default state into presets for now (store in EEPROM or SPIFFS later)
    presets[i].preset = i+1;
  }

  /* init LED PIN */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);           // low is on

  spiffsInit();
  networkInit();                            // connect to wifi, init osc and websockets, print status.
  websocketInit();
  //mdnsInit();
  serverInit();
  dmx.init(512);

  // now set the state to the (now loaded from FS) 'Preset 1'
  state = presets[0];

  //sendTimer = millis();
  for (int i = 0; i < MAX_FIXTURES; i++) {
    leds[i].h = state.hue;
    leds[i].s = 255;
  }
  yield();
}



bool autoControl = false;
void loop() {
  webSocket.loop();
  server.handleClient();
  
  if (savePresetFlag) {
    savePresets();
    savePresetFlag = false;
  }

  /* button */
  static uint32_t buttonTimer;
  if (digitalRead(BOOTLOAD_PIN) == HIGH) {
    buttonTimer = millis();         // reset timer
  }
  else if (digitalRead(BOOTLOAD_PIN) == LOW && millis() - buttonTimer >= 2000) {
    buttonTimer = millis();
    //autoControl = !autoControl;
    if (state.mode == AUTO) state.mode = MANUAL;
    else state.mode = AUTO;
    if (SERIAL_DEBUG) Serial.printf("[button] autoControl now %s\n", autoControl ? "true" : "false");
    if (!autoControl) setZygoteDMX(0, 0, 0, 0, 0);          // fixture # 0 to off.
  }

  /* DMX */
  static uint32_t timer;
  if (millis() - timer > DMX_PERIOD) {                  // how often to send a DMX frame
    timer = millis();
    switch (state.mode) {
      case AUTO:      // currently off
        //frameCounter = 0;                             // helps to start fades from 0 I think?
        for (int i = 0; i < state.num_devices; i++) {
          //          float _temp = leds[i].v;
          //          _temp*=0.99;
          //          leds[i].v=(uint8_t
          leds[i].v *= 0.9;
          CRGB _temp = leds[i];
          setZygoteDMX( i, _temp.r, _temp.g, _temp.b, whiteLeds[i].r);
        }
        break;
      case SDC:
        sdcZygotes();
        break;
      case MANUAL:
        if (millis() - hueTimer > hueCycleSpeed && hueCycleSpeed != 0) {
          hueTimer = millis();
          for (int i = 0; i < state.num_devices; i++) {
            leds[i].h++;
          }
        }
        for (int i = 0; i < state.num_devices; i++) {
          leds[i].s = 255;
          leds[i].v = state.brightness;
          CRGB _temp = leds[i];
          setZygoteDMX( i, _temp.r, _temp.g, _temp.b, whiteLeds[i].r);
          //setZygoteDMX(i, leds[i].r, leds[i].g, leds[i].b, whiteLeds[i].r);
        }
        break;
      default:
        break;
    }
    dmx.update();
  }
  yield();
}
