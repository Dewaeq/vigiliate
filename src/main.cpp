#include <Arduino.h>
#include "main.h"
#include "flash.h"
#include "wifi.h"
#include "webserver.h"
#include "gui.h"
#include "api.h"
#include "sensors/sensors.h"

void setup() {
	Serial.begin(9600);
	Serial.println();
	Flash.begin();
	Gui.begin();
	Sensors.begin();

	delay(500);

	Gui.setStatus("Initiasing");
	Wifi.autoConfig();
	setStatus();

	Serial.println(F("Setting server"));
	setupWebServer();
}

void loop() {
	WebServer.handle();
	Api.update();
	Sensors.update();
}

void setupWebServer() {
	IPAddress ip = Wifi.getIP();

	Serial.print(F("Server is active on: "));
	Serial.println(ip.toString());

	WebServer.begin();

	Serial.println(F("Server is set up"));
}

void setStatus() {
	String ip = Wifi.getIP().toString();
	const char *ssid = Wifi.getSSID();

	Gui.setStatus("");
	Gui.appendStatus(ip);
	Gui.appendStatus(ssid, true);

	Gui.show();
}
