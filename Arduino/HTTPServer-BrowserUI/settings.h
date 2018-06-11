#ifndef SETTINGS_H
#define SETTINGS_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>

#include "wifiConfig.h"                       // AP ssid and password

#define SERIAL_DEBUG true


/* -- Network Settings: -- */
String deviceName = "ESP-Server";             // used for DHCP (make it unique)
const char* ssid_AP = WIFI_SSID;              // see wifiConfig.h
const char* password_AP = WIFI_PASSWORD;      // see wifiConfig.h

IPAddress local_IP(10, 0, 0, 1);              // static IP
IPAddress gateway(10, 0, 0, 1);               // gateway (for static)
IPAddress subnet(255, 255, 255, 0);           // subnet (for static)


#endif /* SETTINGS_H */

