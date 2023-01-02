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

ESP8266WebServer server(80);
Adafruit_SSD1306 display(DISPL_WIDTH, DISPL_HEIGHT, DISPL_MOSI, DISPL_CLK,
						 DISPL_DC, DISPL_RES, DISPL_CS);
String stationID;
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

	String ssid = Flash.getSSID();
	String pass = Flash.getPassword();
	stationID = Flash.getStationID();

	WiFi.begin(ssid, pass);

	if (ssid.length() > 0 && testWifi(ssid)) {
		bodyText = F("Connected to WiFi");
	} else {
		WiFi.disconnect();
		WiFi.mode(WIFI_AP);

		Serial.println(F("Setting AP"));
		bodyText = F("Setting up AP...");

		setupAP();
	}

	Serial.println(F("Setting server"));
	setupWebServer();
}


void loop() {
	server.handleClient();
	setUi();
}

bool testWifi(String ssid) {
	Serial.print(F("Connecting: "));
	Serial.print(ssid);
	bodyText = "Connecting to:\n" + ssid + "\n";

	for (int i = 0; i < 15; i++) {
		if (WiFi.status() == WL_CONNECTED) {
			return true;
		}

		delay(500);
		Serial.print(".");
		bodyText += ".";
		setUi();
	}

	Serial.println();
	Serial.println(F("Failed to connect!"));
	return false;
}

void setupAP() {
	WiFi.mode(WIFI_AP);
	char pass[9];
	getRandomString(pass, 9);

	Serial.print("AP password: ");
	Serial.println(pass);

	bool succes = WiFi.softAP("Station Config", pass);

	if (!succes) {
		Serial.println(F("Failed to setup AP!"));

		hasError = true;
		bodyText = F("Failed to setup AP!");
		return;
	}

	Serial.println(F("AP is setup"));
	Serial.print(F("IP: "));
	Serial.println(WiFi.softAPIP());

	bodyText = F("AP is active, connect to continue setup.\nPassword: ");
	bodyText += pass;
}


void setupWebServer() {
	IPAddress ip = getWifiIP();

	Serial.print(F("Server is active on: "));
	Serial.println(ip.toString());

	server.enableCORS(true);
	server.begin();

	server.on("/", handleRoot);
	server.on("/connect", handleConnect);
	server.on("/disconnect", handleDisconnect);
	server.on("/api/readings", handleReadings);
	server.on("/api/status", handleStatus);

	server.onNotFound(handleNotFound);

	Serial.println(F("Server is set up"));
}

IPAddress getWifiIP() {
	if (WiFi.getMode() == WIFI_STA) {
		return WiFi.localIP();
	} else {
		return WiFi.softAPIP();
	}
}

String getWifiSSID() {
	if (WiFi.getMode() == WIFI_STA) {
		return WiFi.SSID();
	} else {
		return "Station Config";
	}
}

void handleRoot() {
	Serial.println("____root____");

	server.sendHeader("Content-Encoding", "gzip");
	server.send(200, "text/html", index_html, html_size);
}

void handleConnect() {
	Serial.println("____connect____");

	if (!server.hasArg("ssid") || !server.hasArg("pass")) {
		server.send(400, "text/plain", "error: invalid arguments");
		return;
	}

	String newSSID = server.arg("ssid");
	String newPass = server.arg("pass");

	newSSID.trim();
	newPass.trim();

	Flash.setSSID(newSSID);
	Flash.setPassword(newPass);

	server.send(200, "text/plain", "succes");

	delay(200);
	ESP.reset();
}

void handleDisconnect() {
	Serial.println("____disconnect____");
	bodyText = F("Disconnecting WiFi...");

	String msg = F("<p>Restarting station... This may take some minutes</p>");

	server.send(200, "text/html", msg);

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

	server.send(200, "application/json", response);
}

void handleStatus() {
	Json json;
	json.open();
	json.addKeyValue("connected", WiFi.isConnected());
	json.addKeyValue("id", stationID);
	json.addKeyValue("ip", getWifiIP().toString());
	json.addKeyValue("ssid", getWifiSSID());
	json.close();

	String response = json.build();

	server.send(200, "application/json", response);
}

void handleNotFound() {
	server.send(404, "text/html", "<h1>ERROR: 404</h1>");
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
		IPAddress ip = getWifiIP();
		String ssid = getWifiSSID();

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