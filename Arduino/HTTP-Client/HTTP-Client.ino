/* Arduino Wireless + Node Workshop
    ⓒ2018 Elgin-Skye McLaren & Brendan Matkin

   To install Wemos board definition: https://github.com/esp8266/Arduino#installing-with-boards-manager
 *  **NOTE**: Use version 2.4.0-rc2 instead of 2.4.1 (this introduced some new features that are still unstable)

   To install Wemos USB driver: https://wiki.wemos.cc/downloads

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>          // helper to simplify http requests
//#include <ESP8266WiFiMulti.h>         // alt #include <WiFiManager.h>  (captive portal)
//#include <ESP8266WebServer.h>
#include <Bounce2.h>                    // button debounce lib. 


//ESP8266WiFiMulti WiFiMulti;             // multiple wifi AP
//WiFiManager wifiManager;                // alt. (wifi auto connect + captive portal)
//ESP8266WebServer server(80);            // http server
Bounce button = Bounce();                 // button debouncer


/* -- settings -- */
#define BUTTON_PIN 0


/* -- network settings -- */
const char* ssid = "your-ssid";                           // make it unique!
const char* password = "your-password";                   // at least 8 characters long
const char* remoteURL = "http://10.0.0.100/test.html";    // test server address!
const char* deviceName = "unique-name";                   // DHCP device name - something unique too!




/* -- begin SETUP -- */
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.printf("\n\n[INFO] ArduinoWiFi + Node Workshop\n");


  /* connect to WiFi */
  Serial.printf("[INFO] Connecting to: ");
  Serial.print(ssid);
  //WiFi.persistent(false);                   // don't re-write ssid/password to flash every time (avoid degredation)
  WiFi.mode(WIFI_STA);                        // without this ESP will be station and AP at the same time - this can give you headaches!!!
  WiFi.hostname(deviceName);                  // DHCP Hostname    -- does this even work?!
  //WiFi.config(staticIP, gateway, subnet);   // set static IP, defaults to DHCP if config not called
  WiFi.begin(ssid, password);                 // connect to your existing network
  int restartCounter = 0;
  while (!WiFi.isConnected()) {               // auto reset if it's not connecting (occasionally hangs otherwise)
    delay(100); Serial.print("."); 
    restartCounter++;
    if (restartCounter > 50) ESP.restart();   // if it takes more than 5 (50x100ms) seconds to connect, restart!
  }
  Serial.printf("  connected.\n");            // yay it worked!


  /* init LED (turn on LED when connected) */
  pinMode(LED_BUILTIN, OUTPUT);               // LED_BUILTIN is automagically assigned to the built in LED
  digitalWrite(LED_BUILTIN, LOW);             // low is on!


  /* init button/debounce */
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.attach(BUTTON_PIN);                  // connect physical pin to debouncer object
  button.interval(5);                         // in ms (essentially how often to check the pin)


  /* who am I this time? */
  Serial.printf("\n[INFO] WiFi connected.\n");
  Serial.printf("\n[INFO] IP address:  ");
  Serial.println(WiFi.localIP());
  Serial.printf("[INFO] %s (%s) ready. \n", deviceName, WiFi.macAddress().c_str());
  yield();
}




/* -- begin LOOP -- */
void loop() {
  if (WiFi.status() != WL_CONNECTED) {      // only do this stuff if we haven't dropped
    button.update();                        // update the button debounce object
    if (button.fell()) {                    // only act on the transition!
      // do some things!
      // maybe talk to your server!

      HTTPClient http;
      Serial.printf("[HTTP] Begin (%s).. \n", remoteURL);
      http.begin(remoteURL);                                // connect to URL
      Serial.printf("[HTTP] GET..");
      int httpCode = http.GET();                            // send http GET method
      if (httpCode > 0) {
        Serial.printf(". code: %d\n", httpCode);            // got a server response, what is it?
        if (httpCode == HTTP_CODE_OK) {                     // OK (aka code == 200)
          String payload = http.getString();                // read the page!
          Serial.println(payload);
        }
      }
      else {
        Serial.printf(". FAILED! Error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    }
  }

}
