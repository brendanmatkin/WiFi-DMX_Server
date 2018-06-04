void networkInit() {
  if (DMX_AP) {
    WiFi.mode(WIFI_AP);
    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    WiFi.softAP(ssid_AP, password_AP);             // Start the access point
    Serial.print("Access Point \"");
    Serial.print(ssid_AP);
    Serial.println("\" started\r\n");
  }
  else {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.persistent(false);                   // don't re-write ssid/password to flash every time (avoid degredation)
    WiFi.mode(WIFI_STA);                      //
    //WiFi.hostname(deviceName);              // DHCP Hostname    -- does this even work?!
    WiFi.begin(ssid, password);               // connect to WiFi
  
    /* auto reset if it's not connecting (occasionally hangs otherwise) */
    int restartCounter = 0;
    while (!WiFi.isConnected()) {
      delay(100); Serial.print("."); restartCounter++;
      if (restartCounter > 100) ESP.restart();  // if it takes more than ~5 seconds to connect, restart!
    }
    Serial.println("  connected.");            // yay it worked!
    //address = WiFi.localIP()[3];                     // get address
    //if (address < 10) deviceName.concat("00"); else if (address < 100) deviceName.concat("0"); deviceName.concat(address);    // add 3 digit address to deviceName
  }
  yield();
  
  /* who am I this time?  */
  delay(100);
  //Serial.printf("\nWiFi connected.\n");
  Serial.printf("IP address:  ");
  Serial.println(DMX_AP ? WiFi.softAPIP() : WiFi.localIP());
  //Serial.printf("OSC Address: %s\n", oscAddress.c_str());
  //if (ENABLE_WEBSOCKET) Serial.printf("Websocket looking for server at: %s:%u\n", serverIP.toString().c_str(), wsPort);
//  Serial.printf("%s (%s) ready. \n", deviceName.c_str(), WiFi.macAddress().c_str());
//  Serial.printf("\n---\n\n");
}



void websocketInit() {
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}



void mdnsInit() {
  if (MDNS.begin(host)) {                        // start the multicast domain name server
    Serial.print("mDNS responder started: http://");
    Serial.print(host);
    Serial.println(".local");
  }
  else {
    Serial.print("mDNS failed");
  }

  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
}


void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}


void handleNotFound() {
  //digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  //digitalWrite ( led, 0 );
}


void serverInit() {
  server.on ( "/", handleRoot );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );

  Serial.printf("%s (%s) ready. \n", deviceName.c_str(), WiFi.macAddress().c_str());
  Serial.printf("\n---\n\n");
}

