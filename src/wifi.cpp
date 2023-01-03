#include "wifi.h"
#include "flash.h"
#include "webserver.h"
#include "utils.h"
#include "gui.h"
#include <ESP8266WiFi.h>

void WifiClass::autoConfig() {
    Gui.appendBody("Loading Wifi credentials...\n");
    Gui.show();

    Credentials credentials = getStoredCredentials();
    bool succes = testWifi(credentials);
    bool isAP = false;

    if (!succes) {
        credentials = startAP();
        isAP = true;
    } else {
        Gui.setBody("Connected to wifi");
        Gui.show();
    }

    connection = Connection(getIP(), credentials, isAP);
}

IPAddress WifiClass::getIP() {
    if (WiFi.getMode() == WIFI_STA) {
		return WiFi.localIP();
	} else {
		return WiFi.softAPIP();
	}
}

String WifiClass::getSSID() {
	if (WiFi.getMode() == WIFI_STA) {
		return WiFi.SSID();
	} else {
		return AP_SSID;
	}
}

bool WifiClass::isConnected() {
    return WiFi.isConnected();
}

Credentials WifiClass::getStoredCredentials() {
    String ssid = Flash.getSSID();
	String password = Flash.getPassword();

    return Credentials(ssid, password);
}

bool WifiClass::testWifi(Credentials credentials) {
    Gui.appendBody("Connecting to ");
    Gui.appendBody(credentials.ssid.c_str(), true);
    Gui.show();

    WiFi.begin(credentials.ssid, credentials.password);

    for (int i = 0; i < 10; i++) {
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }

        Gui.appendBody(".");
        Gui.show();
        delay(500);
    }

    return false;
}

Credentials WifiClass::startAP() {
    WiFi.mode(WIFI_AP);

    char password[9];
    getRandomString(password, 9);

    bool succes = WiFi.softAP(AP_SSID, password);
    if (!succes) {
        Serial.println(F("ERROR: Failed to start AP"));
        Gui.appendBody("ERROR: Failed to start AP");
    } else {
        Gui.setBody("AP is active, connect to continue setup.\nPassword: ");
        Gui.appendBody(password);
    }

    Serial.printf("AP password: %s\n", password);
    Gui.show();

    return Credentials(AP_SSID, String(password));
}

void WifiClass::connect() {
    Serial.println("____connect____");

	if (!WebServer.hasArg("ssid") || !WebServer.hasArg("pass")) {
		WebServer.send(400, "text/plain", "error: invalid arguments");
		return;
	}

	String newSSID = WebServer.arg("ssid");
	String newPass = WebServer.arg("pass");

	newSSID.trim();
	newPass.trim();

	Flash.setSSID(newSSID);
	Flash.setPassword(newPass);

	WebServer.send(200, "text/plain", "succes");

	delay(200);
	ESP.reset();
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WIFI)
WifiClass Wifi;
#endif