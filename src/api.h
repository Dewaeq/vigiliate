#ifndef API_H
#define API_H

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "timer.h"

#define SERVER_URL "http://192.168.0.190/"
#define API_POST_DELAY 5 * 60 * 1000

class ApiClass {
    public:
        void update();
        void getReadings();
        void getStatus();
        void submitReadings();
    private:
        HTTPClient http;
        WiFiClient client;
        Timer timer = Timer(API_POST_DELAY);
        const char *getPostReadingUrl();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_API)
extern ApiClass Api;
#endif

#endif