#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#define DISPL_WIDTH 128
#define DISPL_HEIGHT 64
#define STATUS_HEIGHT 15

#define DISPL_MOSI 13 // D7
#define DISPL_CLK 14  // D5
#define DISPL_RES 2	  // D4
#define DISPL_DC 0	  // D3
#define DISPL_CS 15	  // D8

// WiFi connection
bool testWifi(String ssid);
void setupAP();
void setupWebServer();
IPAddress getWifiIP();
String getWifiSSID();

// routes
void handleRoot();
void handleConnect();
void handleDisconnect();
void handleReadings();
void handleStatus();
void handleNotFound();

// UI methods
void setUi();
void setStatus();
void setBody();

#endif
