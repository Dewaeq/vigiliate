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

// start values are inclusive, end values are exclusive
#define MEM_SSID_START 0
#define MEM_SSID_END 32
#define MEM_PASS_START 32
#define MEM_PASS_END 64
// station id string is ALWAYS 6 characters long, not including null character
#define MEM_STATION_ID_START 64
#define MEM_STATION_ID_END 70


// get/set credentials
String getSSID();
String getPass();
String getStationID();
void setSSID(String newSSID);
void setPass(String newPass);
void setStationID(String newStationID);
void writeString(String val, int start, int end);
String readString(int start, int end);

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

// HTML/JSON parsing
void addLineBreak(String &content);
void addInput(String &content, String label, String name);
template <typename T>
void addJSONValue(String &json, String key, T value, bool comma = true);

// UI methods
void setUi();
void setStatus();
void setBody();

// Helpers
void getRandomString(char value[], int length);
unsigned long getRandomSeed();

#endif
