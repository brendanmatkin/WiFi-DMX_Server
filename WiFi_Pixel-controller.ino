

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
//File fsUploadFile;                      // holds the current upload




/* format bytes helper function */
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}




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

  /* init LED PIN */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);           // low is on

  spiffsInit();
  networkInit();                            // connect to wifi, init osc and websockets, print status.
  websocketInit();
  mdnsInit();
  serverInit();
  dmx.init(512);

  //EEPROM.begin(512);
  //EEPROM.get(addr, val);

  //sendTimer = millis();
  yield();
}


bool autoControl = false;

void loop() {
  webSocket.loop();
  server.handleClient();

  /* button */
  static uint32_t buttonTimer;
  if (digitalRead(BOOTLOAD_PIN) == HIGH) {
    buttonTimer = millis();         // reset timer
  }
  else if (digitalRead(BOOTLOAD_PIN) == LOW && millis() - buttonTimer >= 2000) {
    buttonTimer = millis();
    //autoControl = !autoControl;
    if (mode == AUTO) mode = MANUAL;
    else mode = AUTO;
    if (SERIAL_DEBUG) Serial.printf("[button] autoControl now %s\n", autoControl ? "true" : "false");
    if (!autoControl) setZygoteDMX(0, 0, 0, 0, 0);          // fixture # 0 to off.
  }

  /* DMX */
  static uint32_t timer;
  if (millis() - timer > DMX_PERIOD) {
    timer = millis();
    switch (mode) {
      case AUTO:
        setZygoteDMX(0, 0, 0, 0, 127);          // fixture # 0 to half white.
        break;
      case SDC:
        sdcZygotes();
        break;
      case MANUAL:
        for (int i = 0; i < currentFixtures; i++) {
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
}
