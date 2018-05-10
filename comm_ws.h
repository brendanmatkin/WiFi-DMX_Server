#ifndef COMM_WS_H
#define COMM_WS_H

#include <ArduinoJson.h>
#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);

extern const bool SERIAL_DEBUG;
extern void setZygoteDMX( uint8_t f, uint8_t r, uint8_t g, uint8_t b, uint8_t w );



/*--------- make JSON object from serial websocket data -----------*/
bool deserializeJSON(uint8_t * json) {
//  StaticJsonBuffer<512> jsonBuffer;     // actually only needs to be about 240 bytes, extra reserved for future use (or just because I have the space..). (http://arduinojson.org/assistant/)
//  JsonObject& root = jsonBuffer.parseObject(json);
//  if (!root.success()) {
//    if (SERIAL_DEBUG) Serial.printf("[ws] parseObject() failed\n");
//    return false;
//  }
//  else {
//    if (SERIAL_DEBUG) Serial.printf("[ws] parseObject() success!\n");
//
//  }
//  return root.success();
  return true;
}



/*--------- make serialized JSON to send over websockets -----------*/
const uint16_t jsonSendSize = 256;
void serializeJSON_connected(char * json) {
//  StaticJsonBuffer<jsonSendSize> jsonBuffer;
//  JsonObject& root = jsonBuffer.createObject();
//  //    root["type"] = MSG_TYPE_CONNECT_INFO;
//  //    // JsonObject& data = root.createNestedObject("data");
//  //    // data["id"] = address;
//  //    // data["firmwareVersion"] = FW_VERSION;
//  //    root["id"] = address;
//  //    root["firmwareVersion"] = FW_VERSION;
//  root["id"] = address;
//  root["battery"] = sensorState.voltage;
//  //if (SERIAL_DEBUG) Serial.println(json);
//  root.printTo(json, jsonSendSize);
//  if (SERIAL_DEBUG) root.printTo(Serial); Serial.println();
}


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

        //webSocket.sendTXT(num, LEDON, strlen(LEDON)); // client listens for LEDON which matches id of button
        //      char connJSON[jsonSendSize];
        //      serializeJSON_connected(connJSON);
        //      webSocket.sendTXT(connJSON);    // send to server
      }
      break;
    case WStype_TEXT: {
        if (SERIAL_DEBUG) Serial.printf("[ws] [%u] got text: %s\n", num, payload);
        if (payload[0] == '#') {            // we get RGB data
          uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);   // decode rgb data
//          int r = ((rgb >> 20) & 0x3FF);                     // 10 bits per color, so R: bits 20-29
//          int g = ((rgb >> 10) & 0x3FF);                     // G: bits 10-19
//          int b =          rgb & 0x3FF;                      // B: bits  0-9
//          //int w =          rgb & 0x3FF;                      // B: bits  0-9
//          int r = ((rgb >> 24) & 0xFF);
//          int g = ((rgb >> 16) & 0xFF);
//          int b = ((rgb >> 8)  & 0xFF);
//          int w =          rgb & 0xFF;
          int r = ((rgb >> 16) & 0xFF);
          int g = ((rgb >> 8)  & 0xFF);
          int b =          rgb & 0xFF;
          
          //setZygoteDMX(0, r/4, g/4, b/4, 0);                 // set Zygote #1 RGB. 
          setZygoteDMX(0, r, g, b, 0);                 // set Zygote #1 RGB. 
          
          if (SERIAL_DEBUG) Serial.printf("[ws] R: %u, G: %u, B: %u\n", r, g, b);
        } else if (payload[0] == 'R') {                      // the browser sends an R when the rainbow effect is enabled
          //rainbow = true;
        } else if (payload[0] == 'N') {                      // the browser sends an N when the rainbow effect is disabled
          //rainbow = false;
        } else if (payload[0] == 'W') {
          uint32_t _raw = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
          int w = _raw & 0xFF;
          if (SERIAL_DEBUG) Serial.printf("[ws] W: %u\n", w);
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
