/* Arduino Wireless + Node Workshop
    ⓒ2018 Elgin-Skye McLaren & Brendan Matkin

   To install Wemos board definition: https://github.com/esp8266/Arduino#installing-with-boards-manager
 *  **NOTE**: Use version 2.4.0-rc2 instead of 2.4.1 (this introduced some new features that are still unstable)

   To install Wemos USB driver: https://wiki.wemos.cc/downloads

*/

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ping.h>
//#include <ESP8266WiFiMulti.h>           // alt #include <WiFiManager.h>  (captive portal)
#include <Bounce2.h>                      // button debounce lib. https://github.com/thomasfredericks/Bounce2/wiki
#include "OSCMessage.h"                   // osc messages library https://github.com/CNMAT/OSC


//ESP8266WiFiMulti WiFiMulti;             // multiple wifi AP
//WiFiManager wifiManager;                // alt. (wifi auto connect + captive portal)
WiFiUDP udp;
WiFiUDP udpSend;
Bounce button = Bounce();                 // button debouncer


/* -- settings -- */
#define BUTTON_PIN 0


/* -- network settings -- */
const char* ssid = "Napkin";                              // ap to connect to
const char* password = "itsasecret";                      // wpa2 password
//const char* remoteURL = "http://10.0.0.100/test.html";  // test server address!
const char* deviceName = "unique-name";                   // DHCP device name - something unique too!
IPAddress mIP(224, 0, 0, 10);                              // multicast ip address
uint16_t mPort = 7777;                                    // multicast port

String oscAddress = "/buttonPressed";                     // osc address to send to



/* -- begin SETUP -- */
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.printf("\n\n[INFO] ArduinoWiFi Workshop -- Multicast OSC\n");


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


  /* initialize OSC */
  Serial.print("\n[INFO] Opening UDP port for OSC...");
  if (udp.beginMulticast(WiFi.localIP(), mIP, mPort)) {
      Serial.printf("joined (listening on) multicast group: ");
      Serial.print(mIP);
      Serial.printf(":%u\n", mPort);
  }
  else {
    Serial.printf("(could not join multicast group at: ");
    Serial.print(mIP);
    Serial.printf(":%u\n", mPort);
  }


  /* who am I this time? */
  Serial.printf("\n[INFO] WiFi connected.\n");
  Serial.printf("\n[INFO] IP address:  ");
  Serial.println(WiFi.localIP());
  Serial.printf("[INFO] %s (%s) ready. \n", deviceName, WiFi.macAddress().c_str());
  yield();
}





/* -- begin LOOP -- */
void loop() {
  if (WiFi.isConnected()) {                 // only do this stuff if we haven't dropped our connection!
    button.update();                        // update the button debounce object
    //if (button.fell()) {                    // only act on the transition!

    static uint32_t timer;
    if (millis() - timer > 1000) {
      timer = millis();
      // do some things!
      /* OSC OUT */ 
      OSCMessage msg(oscAddress.c_str());   // must be c string, i.e. null terminated const char*
      msg.add(1);                           // just sending a one to "/buttonPressed"
      //msg.add(anotherArgument);           // can send multiple arguments in a single message
      udpSend.beginPacketMulticast(mIP, 7778, WiFi.localIP());   // make a udp packet to carry our OSC message
      msg.send(udpSend);                        // put message into udp packet
      udpSend.endPacket();                      // sends the packet
      msg.empty();                          // cleanup. not always required by doesn't hurt
      // msg.add(1).add(2).send(udp);       // also allowed
      Serial.println("send");
    }

    /* OSC IN */
    OSCMessage inMsg;                       // create an empty message object
    int udpSize = udp.parsePacket();        // get the size of a waiting udp packet (0 if none)
    if (udpSize > 0) {
      Serial.println("udp packet!");
      while (udpSize--) {
        inMsg.fill(udp.read());               // empty udp packet into empty message
      }
      if (!inMsg.hasError()) {                // make sure it's a valid OSC Message
        inMsg.route("/test", exampleRouteCallback);
        //inMsg.dispatch("/test", exampleDispatchCallback);
      }
      else {
        Serial.println("[OSC] msg parse error");
      }
    }
  }
  yield();                                  // let the WiFi stack finish up if needed 
}
