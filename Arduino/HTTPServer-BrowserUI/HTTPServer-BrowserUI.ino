// esp8266 2.4.0-rc2
// This is a great resource!! https://tttapa.github.io/ESP8266/Chap14%20-%20WebSocket.html

extern "C" {
#include "user_interface.h"   // allows wifi_set_sleep_type
}
#include "settings.h"
#include "index.h"

#define LED_BUILTIN   2       // not correctly mapped for ESP-12x


ESP8266WebServer server(80);                            // http server
WebSocketsServer webSocket = WebSocketsServer(81);  // websocket server



void setup() {
  wifi_set_sleep_type(NONE_SLEEP_T);        // disable sleep (Non-battery or constant streaming only! might help with analogRead)

  /* init LED PIN */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);           // low is on

  /* start Serial */
  Serial.begin(115200);
  Serial.printf("\n");
  delay(500);
  Serial.printf("\n\n\nESP Server with Browser UI\n");
  Serial.printf("Workshop by Elgin-Skye McLaren and Brendan Matkin\n");

  /* wifi access point: */
  WiFi.mode(WIFI_AP);
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  WiFi.softAP(ssid_AP, password_AP);          // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid_AP);
  Serial.println("\" started.\r\n");

  /* websocket server: */
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");

  /* http server: */
  server.on ( "/", handleRoot );              // alt (spiffs): https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );

  /* who am I this time?  */
  delay(100);
  Serial.printf("%s (%s) ready. \n", deviceName.c_str(), WiFi.macAddress().c_str());
  Serial.printf("\n---\n\n");
  Serial.printf("IP address:  ");
  Serial.println(WiFi.softAPIP());
  yield();
}



void loop() {
  webSocket.loop();
  server.handleClient();
  
  yield();
}



/* SERVER CALLBACKS: */
void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}
void handleNotFound() {
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
}

