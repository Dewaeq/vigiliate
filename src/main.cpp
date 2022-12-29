#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include "main.h"
#include "html.h"

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
	EEPROM.begin(70);

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

	String ssid = getSSID();
	String pass = getPass();
	stationID = getStationID();

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

String getSSID() {
	String ssid = readString(MEM_SSID_START, MEM_SSID_END);
	return ssid;
}

String getPass() {
	String pass = readString(MEM_PASS_START, MEM_PASS_END);
	return pass;
}

String getStationID() {
	String id = readString(MEM_STATION_ID_START, MEM_STATION_ID_END);
	if (id == "") {
		char buffer[7];
		getRandomString(buffer, 7);

		id = String(buffer);
		setStationID(id);
	}

	return id;
}

String readString(int start, int end) {
	String result;

	for (int i = start; i < end; i++) {
		int val = EEPROM.read(i);
		if (val == 0 || val == 255)
			break;
		result += char(val);
	}

	return result;
}

void writeString(String val, int start, int end) {
	uint8_t size = val.length();

	for (int i = start; i < end; i++) {
		if (i - start < size) {
			EEPROM.write(i, val[i - start]);
		} else {
			EEPROM.write(i, 0);
		}
	}
	EEPROM.commit();
}

void setSSID(String newSSID) {
	if (newSSID.length() > 32) {
		Serial.println(F("SSID is too long to store!"));
		return;
	}
	writeString(newSSID, MEM_SSID_START, MEM_SSID_END);
}

void setPass(String newPass) {
	if (newPass.length() > 32) {
		Serial.println(F("Password is too long to store!"));
		return;
	}
	writeString(newPass, MEM_PASS_START, MEM_PASS_END);
}

void setStationID(String newStationID) {
	if (newStationID.length() > 6) {
		Serial.println(F("StationID is too long to store!"));
		return;
	}
	writeString(newStationID, MEM_STATION_ID_START, MEM_STATION_ID_END);
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
	return;

	String msg = F("<!DOCTYPE html><html><body>");
	msg += "<h1>Local config</h1>";

	if (!WiFi.isConnected()) {
		msg += F("<form action='/connect' method='GET'>");

		addInput(msg, "SSID: ", "ssid");
		addLineBreak(msg);
		addInput(msg, "PASS: ", "pass");
		addLineBreak(msg);

		msg += F("<input type='submit'>");
		msg += F("</form>");
	} else {
		msg += F("<p>Station is online</p>");
		msg += F("<p>Temperature: 15C</p>");
		msg += F("<p>Humidity: 75%</p>");

		msg += F("<form action='/disconnect' method='GET'>");
		msg += F("<button type='submit'>Disconnect WiFi</button>");
		msg += F("</form>");
	}

	msg += F("</body></html>");
	server.send(200, "text/html", msg);
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

	setSSID(newSSID);
	setPass(newPass);

	String msg = F("<p>ssid: ");
	msg += newSSID;
	addLineBreak(msg);
	msg += "pass: ";
	msg += newPass;
	msg += "</p>";

	msg += F("<p>Station will now restart and try to establish a connection. "
			 "See the display for the result</p>");

	server.send(200, "text/html", msg);

	delay(200);
	ESP.reset();
}

void handleDisconnect() {
	Serial.println("____disconnect____");
	bodyText = F("Disconnecting WiFi...");

	String msg = F("<p>Restarting station... This may take some minutes</p>");

	server.send(200, "text/html", msg);

	delay(200);
	setSSID("");
	setPass("");
	WiFi.disconnect();
	ESP.reset();
}

void handleReadings() {
	String response = "{";

	addJSONValue(response, "temp", temp);
	addJSONValue(response, "hum", hum);
	addJSONValue(response, "pm25", pm25);
	addJSONValue(response, "pm10", pm10, false);

	response += "}";

	server.send(200, "application/json", response);
}

void handleStatus() {
	String response = "{";

	addJSONValue(response, "connected", WiFi.isConnected());
	addJSONValue(response, "id", stationID); 
	addJSONValue(response, "ip", getWifiIP().toString());
	addJSONValue(response, "ssid", getWifiSSID(), false);

	response += "}";

	server.send(200, "application/json", response);
}

void handleNotFound() {
	server.send(404, "text/html", "<h1>ERROR: 404</h1>");
}

void addLineBreak(String &content) {
	content += "<br>";
}

void addInput(String &content, String label, String name) {
	content += "<label for='";
	content += name;
	content += "'>";
	content += label;
	content += "</label>";

	content += "<input name='";
	content += name;
	content += "'>";
}

// adds a comma to the end of the added line if [comma] is true
template <typename T>
void addJSONValue(String &json, String key, T value, bool comma) {
	json += "\"" + key + "\": \"" + value + "\"";

	if (comma) {
		json += ",";
	}
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

static const char CHARS[] =
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

void getRandomString(char value[], int length) {
	randomSeed(getRandomSeed());

	for (int i = 0; i < length - 1; i++) {
		size_t index = random(0, 62);
		value[i] = CHARS[index];
	}

	value[length - 1] = '\0';
}

unsigned long getRandomSeed() {
	unsigned long seed = 0UL;
	unsigned long reading = 0UL;
	int interval = 5;
	int bit = 0;

	for (int i = 0; i < 31; i++) {
		for (int j = 0; j < 3; j++) {
			delay(2 + interval);

			reading = analogRead(A0);
			bit ^= reading & 1;
			// randomly chosen numbers, no concrete meaning
			interval = (reading % 6) * 10;
		}
		seed |= (long)bit << i;
	}

	return seed;
}
