#ifndef COMM_WS_H
#define COMM_WS_H

#include <ArduinoJson.h>
#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);

extern const bool SERIAL_DEBUG;
extern void setZygoteDMX( uint8_t f, uint8_t r, uint8_t g, uint8_t b, uint8_t w );
extern bool savePresets();
extern bool loadPresets();

/*
JSON object:
  let state = {
    speed: 127,
    delay: 0,
    hue: 160,
    mode: 0,
    hue_cycle: 0,
    brightness: 255,
    white: 0,
    preset: 0,
    num_devices: 8
  };

local struct: 
  struct State = {
    uint8_t speed       = 127;
    uint8_t delay       = 0;
    uint8_t hue         = 160;
    uint8_t mode        = 0;
    uint8_t hue_cycle   = 0;
    uint8_t brightness  = 255;
    uint8_t white       = 0;
    uint8_t preset      = 0;
    uint8_t num_devices = 8;
  } state;
*/


/*---------- JSON serialize/deserialize ----------*/
const uint16_t jsonReceiveSize = 256;
bool deserializeJSON(uint8_t * json) {
  StaticJsonBuffer<jsonReceiveSize*3> jsonBuffer;     // actually only needs to be about 240 bytes, extra reserved for future use (or just because I have the space..). (http://arduinojson.org/assistant/)
  JsonVariant variant = jsonBuffer.parse(json);
  if (variant.is<JsonArray>()) {
    JsonArray& array = variant;
    if (!array.success()) {
      if (SERIAL_DEBUG) Serial.printf("[JSON] parseArray() failed\n");
      return false;
    }
    else {
      if (SERIAL_DEBUG) Serial.printf("[JSON] JSON Array test values: ");
      for (int i = 0; i < NUM_PRESETS; i++) {
//        StaticJsonBuffer<jsonReceiveSize> jsonBuffer2;
//        JsonObject& root = jsonBuffer2.createObject();
        presets[i].speed = array[i]["speed"];
        presets[i].delay = array[i]["delay"];
        presets[i].hue   = array[i]["hue"];
        uint8_t _mode = array[i]["mode"];
        presets[i].mode = (Mode) _mode;
        presets[i].hue_cycle = array[i]["hue_cycle"];
        presets[i].brightness = array[i]["brightness"];
        presets[i].white = array[i]["white"];
        presets[i].preset = array[i]["preset"];
        presets[i].num_devices = array[i]["num_devices"];
      }
      if (SERIAL_DEBUG) Serial.println();
    }
    if (array.success()) {      // this means we parsed a new preset array!
      savePresetFlag = true;    // so save it!
      return true;
    }
    else {
      return false;
    }
  }
  else if (variant.is<JsonObject>()) {
    JsonObject& root = variant;
    if (!root.success()) {
      if (SERIAL_DEBUG) Serial.printf("[JSON] parseObject() failed\n");
      return false;
    }
    else {
      // SDC(/LFO):
      state.speed = root["speed"];
      state.delay = root["delay"];
      state.hue   = root["hue"];
      for (int i = 0; i < currentFixtures; i++) {
        leds[i].h = state.hue;
      }
      // MODE: 
      uint8_t _mode = root["mode"];
      state.mode = (Mode) _mode;
      // HUE CYCLE:
      state.hue_cycle = root["hue_cycle"];
      if (state.hue_cycle == 0) hueCycleSpeed = 0;
      else hueCycleSpeed = map(state.hue_cycle, 0, 255, 175, 1);
      // BRIGHTNESS, WHITE:
      state.brightness = root["brightness"];
      state.white = root["white"];
      for (int i = 0; i < currentFixtures; i++) {
        whiteLeds[i].r = root["white"];   // using red channel of fastLED for white control
      }
      // PRESET:
      state.preset = root["preset"];
      // NUMBER OF DEVICES:
      state.num_devices = root["num_devices"];
    }
    return root.success();
  }
  else {
    if (SERIAL_DEBUG) Serial.printf("[JSON] variant not Object or Array\n");
    return false;
  }
}

const uint16_t jsonSendSize = 256;
void serializeJSON_connected(char * json) {
  // send state: 
  StaticJsonBuffer<jsonSendSize> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["speed"] = state.speed;
  root["delay"] = state.delay;
  root["hue"] = state.hue;
  root["mode"] = state.mode;
  root["hue_cycle"] = state.hue_cycle;
  root["brightness"] = state.brightness;
  root["white"] = state.white;
  root["preset"] = state.preset;
  root["num_devices"] = state.num_devices;
  root.printTo(json, jsonSendSize);
  //if (SERIAL_DEBUG) Serial.println(json);
  if (SERIAL_DEBUG) root.prettyPrintTo(Serial);
}

void serializeJSON_presets(char * json) {
  // send presets: 
  //StaticJsonBuffer<jsonSendSize*3> jsonBufferArray;
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  for (int i = 0; i < NUM_PRESETS; i++) {
    //StaticJsonBuffer<jsonSendSize> jsonBufferTemp;
    JsonObject& root = jsonBuffer.createObject();
    root["speed"] = presets[i].speed;
    root["delay"] = presets[i].delay;
    root["hue"] = presets[i].hue;
    root["mode"] = presets[i].mode;
    root["hue_cycle"] = presets[i].hue_cycle;
    root["brightness"] = presets[i].brightness;
    root["white"] = presets[i].white;
    root["preset"] = presets[i].preset;
    root["num_devices"] = presets[i].num_devices;
    array.add(root);
  }
  array.printTo(json, jsonBuffer.size());
  if (SERIAL_DEBUG) array.prettyPrintTo(Serial);
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
        // num is client ID (because I am a websocket server!)
        IPAddress ip = webSocket.remoteIP(num);
        if (SERIAL_DEBUG) Serial.printf("[ws] [%u] Connected from url: %u.%u.%u.%u%s replying...\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        loadPresets();
        yield();
        
        char presetJSON[jsonSendSize*3];
        serializeJSON_presets(presetJSON);
        webSocket.sendTXT(num, presetJSON);
        
        char connJSON[jsonSendSize];
        serializeJSON_connected(connJSON);
        webSocket.sendTXT(num, connJSON);    // send to same client (num) that connected to me!
      }
      break;
    case WStype_TEXT: {
        if (SERIAL_DEBUG) Serial.printf("[ws] [%u] got text: %s\n", num, payload);
        deserializeJSON(payload);
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
