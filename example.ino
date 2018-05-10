///*
// * ESP8266 Web server with Web Socket to control an LED.
// *
// * The web server keeps all clients' LED status up to date and any client may
// * turn the LED on or off.
// *
// * For example, clientA connects and turns the LED on. This changes the word
// * "LED" on the web page to the color red. When clientB connects, the word
// * "LED" will be red since the server knows the LED is on.  When clientB turns
// * the LED off, the word LED changes color to black on clientA and clientB web
// * pages.
// *
// * References:
// *
// * https://github.com/Links2004/arduinoWebSockets
// *
// */
//
//#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
//#include <WebSocketsServer.h>
//#include <Hash.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//
//static const char ssid[] = "Napkin";
//static const char password[] = "itsasecret";
//MDNSResponder mdns;
//
//static void writeLED(bool);
//
//ESP8266WiFiMulti WiFiMulti;
//
//ESP8266WebServer server(80);
//WebSocketsServer webSocket = WebSocketsServer(81);
//
//static const char PROGMEM INDEX_HTML[] = R"rawliteral(
//<!DOCTYPE html>
//<html>
//<head>
//<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
//<title>ESP8266 WebSocket Demo</title>
//<style>
//  body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }
//</style>
//<script>
//var websock;
//function start() {
//  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
//  websock.onopen = function(evt) { console.log('websock open'); };
//  websock.onclose = function(evt) { console.log('websock close'); };
//  websock.onerror = function(evt) { console.log(evt); };
//  websock.onmessage = function(evt) {
//    console.log(evt);
//    var e = document.getElementById('ledstatus');
//    if (evt.data === 'ledon') {
//      e.style.color = 'red';
//    }
//    else if (evt.data === 'ledoff') {
//      e.style.color = 'black';
//    }
//    else {
//      console.log('unknown event');
//    }
//  };
//}
//function buttonclick(e) {
//  websock.send(e.id);
//}
//</script>
//</head>
//<body onload="javascript:start();">
//<h1>ESP8266 WebSocket Demo</h1>
//<div id="ledstatus"><b>LED</b></div>
//<button id="ledon"  type="button" onclick="buttonclick(this);">On</button> 
//<button id="ledoff" type="button" onclick="buttonclick(this);">Off</button>
//</body>
//</html>
//)rawliteral";
//
//// GPIO#0 is for Adafruit ESP8266 HUZZAH board. Your board LED might be on 13.
//const int LEDPIN = 0;
//// Current LED status
//bool LEDStatus;
//
//// Commands sent through Web Socket
//const char LEDON[] = "ledon";
//const char LEDOFF[] = "ledoff";
//
//void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
//{
//  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
//  switch(type) {
//    case WStype_DISCONNECTED:
//      Serial.printf("[%u] Disconnected!\r\n", num);
//      break;
//    case WStype_CONNECTED:
//      {
//        IPAddress ip = webSocket.remoteIP(num);
//        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
//        // Send the current LED status
//        if (LEDStatus) {
//          webSocket.sendTXT(num, LEDON, strlen(LEDON));
//        }
//        else {
//          webSocket.sendTXT(num, LEDOFF, strlen(LEDOFF));
//        }
//      }
//      break;
//    case WStype_TEXT:
//      Serial.printf("[%u] get Text: %s\r\n", num, payload);
//
//      if (strcmp(LEDON, (const char *)payload) == 0) {
//        writeLED(true);
//      }
//      else if (strcmp(LEDOFF, (const char *)payload) == 0) {
//        writeLED(false);
//      }
//      else {
//        Serial.println("Unknown command");
//      }
//      // send data to all connected clients
//      webSocket.broadcastTXT(payload, length);
//      break;
//    case WStype_BIN:
//      Serial.printf("[%u] get binary length: %u\r\n", num, length);
//      hexdump(payload, length);
//
//      // echo data back to browser
//      webSocket.sendBIN(num, payload, length);
//      break;
//    default:
//      Serial.printf("Invalid WStype [%d]\r\n", type);
//      break;
//  }
//}
//
//void handleRoot()
//{
//  server.send_P(200, "text/html", INDEX_HTML);
//}
//
//void handleNotFound()
//{
//  String message = "File Not Found\n\n";
//  message += "URI: ";
//  message += server.uri();
//  message += "\nMethod: ";
//  message += (server.method() == HTTP_GET)?"GET":"POST";
//  message += "\nArguments: ";
//  message += server.args();
//  message += "\n";
//  for (uint8_t i=0; i<server.args(); i++){
//    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//  }
//  server.send(404, "text/plain", message);
//}
//
//static void writeLED(bool LEDon)
//{
//  LEDStatus = LEDon;
//  // Note inverted logic for Adafruit HUZZAH board
//  if (LEDon) {
//    digitalWrite(LEDPIN, 0);
//  }
//  else {
//    digitalWrite(LEDPIN, 1);
//  }
//}
//
//void setup()
//{
//  pinMode(LEDPIN, OUTPUT);
//  writeLED(false);
//
//  Serial.begin(115200);
//
//  //Serial.setDebugOutput(true);
//
//  Serial.println();
//  Serial.println();
//  Serial.println();
//
//  for(uint8_t t = 4; t > 0; t--) {
//    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
//    Serial.flush();
//    delay(1000);
//  }
//
//  WiFiMulti.addAP(ssid, password);
//
//  while(WiFiMulti.run() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(100);
//  }
//
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(ssid);
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//
//  if (mdns.begin("espWebSock", WiFi.localIP())) {
//    Serial.println("MDNS responder started");
//    mdns.addService("http", "tcp", 80);
//    mdns.addService("ws", "tcp", 81);
//  }
//  else {
//    Serial.println("MDNS.begin failed");
//  }
//  Serial.print("Connect to http://espWebSock.local or http://");
//  Serial.println(WiFi.localIP());
//
//  server.on("/", handleRoot);
//  server.onNotFound(handleNotFound);
//
//  server.begin();
//
//  webSocket.begin();
//  webSocket.onEvent(webSocketEvent);
//}
//
//void loop()
//{
//  webSocket.loop();
//  server.handleClient();
//}
