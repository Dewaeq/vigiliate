#include "webserver.h"

void WebServerClass::begin() {
    server.begin();

    server.onNotFound([]() {
        WebServer.handleNotFound();
    });
}

void WebServerClass::addRoute(const String &route, std::function<void(void)> handler) {
    server.on(route, handler);
}

void WebServerClass::handleNotFound() {
	server.send(404, "text/html", "<h1>ERROR: 404</h1>");
}

bool WebServerClass::hasArg(const String &name) {
    return server.hasArg(name);
}

const String &WebServerClass::arg(const String &name) {
    return server.arg(name);
}

void WebServerClass::send(int code, const char *content_type, const char *content) {
    server.send(code, content_type, content);
}

void WebServerClass::send(int code, const char *content_type, const uint8_t *content, size_t content_length) {
    server.send(code, content_type, content, content_length);
}

void WebServerClass::send(int code, const char *content_type, const String &content) {
    server.send(code, content_type, content);
}

void WebServerClass::sendHeader(const char *name, const char *value) {
    server.sendHeader(name, value);
}

void WebServerClass::enableCORS() {
    server.enableCORS(true);
}

void WebServerClass::handle() {
    server.handleClient();
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WEBSERVER)
WebServerClass WebServer;
#endif
