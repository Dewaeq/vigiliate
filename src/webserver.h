#ifndef MY_WEBSERVER_H
#define MY_WEBSERVER_H

#include <ESP8266WebServer.h>

class WebServerClass {
    public:
        void begin();
        void addRoute(const char *route, std::function<void(void)> handler);
        bool hasArg(const char *name);
        const String &arg(const char *name) const;
        void send(int code, const char *content_type, const char *content);
        void send(int code, const char *content_type, const uint8_t *content, size_t content_length);
        void send(int code, const char *content_type, const String &content);
        void sendHeader(const char *name, const char *value);
        void handle();
    private:
        ESP8266WebServer server = ESP8266WebServer(80);
        void handleNotFound();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WEBSERVER)
extern WebServerClass WebServer;
#endif

#endif