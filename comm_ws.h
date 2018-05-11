#ifndef COMM_WS_H
#define COMM_WS_H

#include <ArduinoJson.h>
#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);

extern const bool SERIAL_DEBUG;
extern void setZygoteDMX( uint8_t f, uint8_t r, uint8_t g, uint8_t b, uint8_t w );


/*---------- Websocket Event ------------*/
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_ERROR:
      if (SERIAL_DEBUG) Serial.printf("[ws] ERROR!\n");
      break;
    case WStype_DISCONNECTED:
      if (SERIAL_DEBUG) Serial.printf("[ws] [%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        if (SERIAL_DEBUG) Serial.printf("[ws] [%u] Connected from url: %u.%u.%u.%u; url: %s replying...\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        String _init = 'I' + String(currentFixtures);
        webSocket.sendTXT(num, _init);
      }
      break;
    case WStype_TEXT: {
        if (SERIAL_DEBUG) Serial.printf("[ws] [%u] got text: %s\n", num, payload);
        if (payload[0] == '#') {                    // get RGB data
          uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);   // decode rgb data
          int r = ((rgb >> 16) & 0xFF);
          int g = ((rgb >> 8)  & 0xFF);
          int b =          rgb & 0xFF;
          //leds[0].setRGB(r, g, b);
          for (int i = 0; i < currentFixtures; i++) {
            leds[i] = rgb2hsv_approximate(CRGB(r, g, b));
          }
          if (SERIAL_DEBUG) Serial.printf("[ws] R: %u, G: %u, B: %u\n", r, g, b);
        } 
        else if (payload[0] == '&') {             // get SDC data
          uint32_t sdc = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
          int s = (sdc >> 16) & 0xFF;               // speed
          int d = (sdc >> 8)  & 0xFF;               // delay
          int c =         sdc & 0xFF;               // color/hue
          sdc_speed = (256 - s)/5.0f;
          //leds[0].setHue(c);
          for (int i = 0; i < currentFixtures; i++) {
            leds[0].h=c;
          }
          if (SERIAL_DEBUG) Serial.printf("[ws] S: %u, D: %u, C: %u, spd:%.2f\n", s, d, c, sdc_speed);
        } 
        else if (payload[0] == 'N') {             // number of devices
          uint32_t _num = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
          currentFixtures = _num & 0xFF;
          if (SERIAL_DEBUG) Serial.printf("[ws] currentFixtures: %u\n", currentFixtures);
        } 
        else if (payload[0] == 'W') {
          uint32_t _raw = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
          int w = _raw & 0xFF;
          for (int i = 0; i < currentFixtures; i++) {
            whiteLeds[i].r = w;
          }
          if (SERIAL_DEBUG) Serial.printf("[ws] White: %u\n", w);
        }
        else if (payload[0] == 'M') {
          mode = MANUAL;
          if (SERIAL_DEBUG) Serial.printf("[ws] Mode: %d\n", mode);
        }
        else if (payload[0] == 'S') {
          mode = SDC;
          if (SERIAL_DEBUG) Serial.printf("[ws] Mode: %d\n", mode);
        }
        else if (payload[0] == 'A') {
          mode = AUTO;
          if (SERIAL_DEBUG) Serial.printf("[ws] Mode: %d\n", mode);
        }
      }
      break;
    case WStype_BIN:
      if (SERIAL_DEBUG) Serial.printf("[ws] got binary (length): %zu\n", length);
      hexdump(payload, length);
      break;
    default:
      break;
  }
}

#endif /* COMM_WS_H */
