#include "routes.h"
#include "html.h"
#include "webserver.h"
#include "flash.h"
#include "gui.h"
#include "wifi.h"
#include "api.h"

using namespace routes;

void routes::begin() {
    WebServer.addRoute("/", handleRoot);
	WebServer.addRoute("/connect", handleConnect);
	WebServer.addRoute("/disconnect", handleDisconnect);
	WebServer.addRoute("/api/readings", api::handleReadings);
	WebServer.addRoute("/api/status", api::handleStatus);
}

void routes::handleRoot() {
    WebServer.sendHeader("Content-Encoding", "gzip");
	WebServer.send(200, "text/html", index_html, html_size);
}

void routes::handleConnect() {
    Wifi.connect();
}

void routes::handleDisconnect() {
	Wifi.disconnect();
}

void api::handleReadings() {
    Api.getReadings();
}

void api::handleStatus() {
    Api.getStatus();
}
