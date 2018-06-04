void spiffsInit() {
  Serial.printf("[SPIFFS] Mounting file system...");
  if (SPIFFS.begin()) {
    //    Dir dir = SPIFFS.openDir("/");
    //    while (dir.next()) {
    //      String fileName = dir.fileName();
    //      size_t fileSize = dir.fileSize();
    //      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    //    }
    //    Serial.printf("\n");
  }
  else {
    Serial.printf("[SPIFFS] Failed to mount file system");
    return;
  }
  Serial.println(" mounted.");

  if (!loadPresets()) {
    //savePresets();
    savePresetFlag = true;                    // moved savePresets() to loop because it was causing some exceptions in websocket callback (receive buffer was filling up)
  }
  else {
    Serial.println("[load] Presets loaded!");
  }
}





bool loadPresets() {
  File presetFile = SPIFFS.open("/presets.json", "r");
  if (!presetFile) {
    if (SERIAL_DEBUG) Serial.printf("[load] Failed to open presets file\n");
    return false;
  }
  size_t size = presetFile.size();
  if (size > 1024) {
    if (SERIAL_DEBUG) Serial.printf("[load] Presets file size is too large\n");
    return false;
  }

  char buf[size];
  presetFile.readBytes(buf, size);
  //if (SERIAL_DEBUG) Serial.printf("[load] SIZE: %d\n", size);

  StaticJsonBuffer<512> jsonBuffer;   // (or just because I have the space..). (http://arduinojson.org/assistant/)
  JsonArray& array = jsonBuffer.parseArray(buf);
  //if (SERIAL_DEBUG) Serial.printf("[load] SIZEBUFF: %d\n", jsonBuffer.size());
  if (!array.success()) {
    if (SERIAL_DEBUG) Serial.printf("[load] Load Presets (parseArray()) failed\n");
    return false;
  }
  else {
    if (SERIAL_DEBUG) Serial.printf("[load] JSON Array test values: ");
    for (int i = 0; i < NUM_PRESETS; i++) {
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
      //if (SERIAL_DEBUG) Serial.printf("[load] SIZE2: %d  ", jsonBuffer2.size());
      //if (SERIAL_DEBUG) Serial.printf("P%u.preset=%u  ", i, presets[i].preset);
      if (SERIAL_DEBUG){
        Serial.printf("[lP] hue[%u]:%u\n", i, presets[i].hue);
        Serial.printf("[lP] array[%u] hue:%u\n", i, (uint8_t)array[i]["hue"]);
      }
    }
    if (SERIAL_DEBUG) Serial.printf("\n");
  }
  if (SERIAL_DEBUG) {
    Serial.printf("[lP] ");
    array.prettyPrintTo(Serial);
  }
  presetFile.close();
  return array.success();
}




bool savePresets() {
  if (SERIAL_DEBUG) Serial.printf("[save] Save Presets Start\n");

  SPIFFS.remove("/presets.json");   // delete so we don't append

  File presetFile = SPIFFS.open("/presets.json", "w");
  if (!presetFile) {
    if (SERIAL_DEBUG) Serial.printf("[save] Failed to open presets file for writing\n");
    return false;
  }
  if (SERIAL_DEBUG) Serial.printf("[save] preset file loaded for saving\n");
  
  DynamicJsonBuffer jsonBuffer;                 // couldn't get static buffer to work in the for loop. 
  JsonArray& json = jsonBuffer.createArray();
  for (uint8_t i = 0; i < NUM_PRESETS; i++) {
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
    json.add(root);
    if (SERIAL_DEBUG) {
      Serial.printf("[sP] hue[%u]:%u\n", i, presets[i].hue);
      Serial.printf("[sP] array[%u] hue:%u\n", i, (uint8_t)json[i]["hue"]);
    }
  }
  if (SERIAL_DEBUG) Serial.printf("[save] buffersize: %d\n", jsonBuffer.size());
  
  json.printTo(Serial);
  json.printTo(presetFile);
  presetFile.close();
  if (SERIAL_DEBUG) Serial.printf("[save] Presets saved!\n");
  return true;
}



/* format bytes helper function */
//String formatBytes(size_t bytes) {
//  if (bytes < 1024) {
//    return String(bytes) + "B";
//  } else if (bytes < (1024 * 1024)) {
//    return String(bytes / 1024.0) + "KB";
//  } else if (bytes < (1024 * 1024 * 1024)) {
//    return String(bytes / 1024.0 / 1024.0) + "MB";
//  } else {
//    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
//  }
//}

