#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include "main.h"
#include "html.h"
#include "flash.h"
#include "utils.h"
#include "json.h"
#include "wifi.h"
#include "webserver.h"

Adafruit_SSD1306 display(DISPL_WIDTH, DISPL_HEIGHT, DISPL_MOSI, DISPL_CLK,
						 DISPL_DC, DISPL_RES, DISPL_CS);

float temp;
float hum;
float pm25;
float pm10;

String bodyText;
String statusText;
bool hasError;

void setup() {
	Serial.begin(9600);
	Flash.begin();

	if (!display.begin()) {
		Serial.println(F("Failed to connect to display!"));
		for (;;)
			;
	}

	delay(500);

	Serial.println();
	display.clearDisplay();
	display.setTextColor(WHITE);
	display.display();

	Connection con = Wifi.autoConfig();

	Serial.println(F("Setting server"));
	setupWebServer();
}


void loop() {
	WebServer.handle();
	setUi();
}

void setupWebServer() {
	IPAddress ip = Wifi.getIP();

	Serial.print(F("Server is active on: "));
	Serial.println(ip.toString());

	WebServer.enableCORS();
	WebServer.begin();

	WebServer.addRoute("/", handleRoot);
	WebServer.addRoute("/connect", []() {
		Wifi.connect();
	});
	WebServer.addRoute("/disconnect", handleDisconnect);
	WebServer.addRoute("/api/readings", handleReadings);
	WebServer.addRoute("/api/status", handleStatus);


	Serial.println(F("Server is set up"));
}

void handleRoot() {
	Serial.println("____root____");

	WebServer.sendHeader("Content-Encoding", "gzip");
	WebServer.send(200, "text/html", index_html, html_size);
}

void handleDisconnect() {
	Serial.println("____disconnect____");
	bodyText = F("Disconnecting WiFi...");

	WebServer.send(200, "text/plain", "succes");

	delay(200);
	Flash.setSSID("");
	Flash.setPassword("");
	WiFi.disconnect();
	ESP.reset();
}

void handleReadings() {
	Json json;
	json.open();
	json.addKeyValue("temp", temp);
	json.addKeyValue("hum", hum);
	json.addKeyValue("pm25", pm25);
	json.addKeyValue("pm10", pm10);
	json.close();

	String response = json.build();

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

	String response = json.build();

	WebServer.send(200, "application/json", response);
}

void setUi() {
	display.clearDisplay();

	setStatus();
	setBody();
	display.drawLine(0, STATUS_HEIGHT, DISPL_WIDTH, STATUS_HEIGHT, WHITE);

	display.display();
	delay(200);
}

void setStatus() {
	display.setTextSize(1);
	display.setCursor(0, 0);

	if (!hasError) {
		IPAddress ip = Wifi.getIP();
		String ssid = Wifi.getSSID();

		display.println(ip.toString());
		display.println(ssid);
	} else {
		display.println("ERROR");
	}
}

void setBody() {
	display.setTextSize(hasError ? 2 : 1);
	display.setCursor(0, STATUS_HEIGHT + 5);
	display.println(bodyText);
}