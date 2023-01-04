#include <Arduino.h>
#include "main.h"
#include "html.h"
#include "flash.h"
#include "json.h"
#include "wifi.h"
#include "webserver.h"
#include "gui.h"
#include "sensors/sensors.h"

void setup() {
	Serial.begin(9600);
	Serial.println();
	Flash.begin();
	Gui.begin();
	Sensors.begin();

	delay(500);

	Wifi.autoConfig();
	setStatus();

	Serial.println(F("Setting server"));
	setupWebServer();
}

void loop() {
	WebServer.handle();
	Sensors.update();
}

void setupWebServer() {
	IPAddress ip = Wifi.getIP();

	Serial.print(F("Server is active on: "));
	Serial.println(ip.toString());

	WebServer.begin();

	WebServer.addRoute("/", handleRoot);
	WebServer.addRoute("/connect", []() {
		Wifi.connect();
	});
	WebServer.addRoute("/disconnect", []() {
		Wifi.disconnect();
	});
	WebServer.addRoute("/api/readings", handleReadings);
	WebServer.addRoute("/api/status", handleStatus);


	Serial.println(F("Server is set up"));
}

void handleRoot() {
	Serial.println("____root____");

	WebServer.sendHeader("Content-Encoding", "gzip");
	WebServer.send(200, "text/html", index_html, html_size);
}

void handleReadings() {
	SensorsReading reading = Sensors.getReadings();

	Json json;
	json.open();
	json.addKeyValue("pm25", reading.sds.pm25);
	json.addKeyValue("pm10", reading.sds.pm10);
	json.addKeyValue("temp", reading.dht.temperature);
	json.addKeyValue("hum", reading.dht.humitidy);
	json.close();

	const char *response = json.build();

	WebServer.send(200, "application/json", response);
}

void handleStatus() {
	Json json;
	json.open();
	json.addKeyValue("connected", Wifi.isConnected());
	json.addKeyValue("id", Flash.stationID);
	json.addKeyValue("ip", Wifi.getIP().toString());
	json.addKeyValue("ssid", Wifi.getSSID());
	json.close();

	const char *response = json.build();

	WebServer.send(200, "application/json", response);
}

void setStatus() {
	String ip = Wifi.getIP().toString();
	const char *ssid = Wifi.getSSID();

	Gui.setStatus("");
	Gui.appendStatus(ip);
	Gui.appendStatus(ssid, true);

	Gui.show();
}
