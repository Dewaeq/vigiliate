#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define AP_SSID "Station Config"

struct Credentials {
    String ssid;
    String password;

    Credentials(String s, String p) : ssid(s), password(p) {};
};

struct Connection {
    IPAddress ip;
    Credentials credentials;

    Connection(IPAddress addr, Credentials creds): ip(addr), credentials(creds) {};
};

class WifiClass {
    public:
        // login with the saved credentials, if unsuccessful, start an AP
        Connection autoConfig();
        IPAddress getIP();
        String getSSID();
        bool isConnected();
        // connect from api request
        void connect();
    private:
        Credentials getStoredCredentials();
        bool testWifi(Credentials credentials);
        Credentials startAP();
        
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WIFI)
extern WifiClass Wifi;
#endif

#endif