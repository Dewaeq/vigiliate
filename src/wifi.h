#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define AP_SSID "Station Config"

struct Credentials {
    char ssid[32];
    char password[32];

    Credentials() {};
    Credentials(const char *ssid, const char *password);
};

struct Connection {
    IPAddress ip;
    Credentials credentials;
    bool isAP;

    Connection() {};
    Connection(IPAddress ip, Credentials creds, bool isAP): ip(ip), credentials(creds), isAP(isAP) {};
};

class WifiClass {
    public:
        WifiClass() {};
        struct Connection connection;
        // login with the saved credentials, if unsuccessful, start an AP
        void autoConfig();
        IPAddress getIP();
        char *getSSID();
        bool isConnected();
        // connect from api request
        void connect();
        void disconnect();
    private:
        Credentials getStoredCredentials();
        bool testWifi(Credentials credentials);
        Credentials startAP();
        
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WIFI)
extern WifiClass Wifi;
#endif

#endif