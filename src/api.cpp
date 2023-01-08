#include "api.h"
#include "json.h"
#include "webserver.h"
#include "wifi.h"
#include "flash.h"
#include "sensors/sensors.h"

void ApiClass::update() {
	if (timer.isOver()) {
		submitReadings();
	}
}

void ApiClass::submitReadings() {
	SensorsReading reading = Sensors.getReadings();

	Json json;
	json.open();
	json.addKeyValue("station_token", Flash.stationID);
	json.addKeyValue("date");
	json.addKeyValue("temperature", reading.dht.temperature);
	json.addKeyValue("humidity", reading.dht.humitidy);
	json.addKeyValue("pm10", reading.sds.pm10);
	json.addKeyValue("pm25", reading.sds.pm25);
	json.addKeyValue("co2", 0.0f);
	json.addKeyValue("voc", 0.0f);
	json.close();

	const char *request = json.build();
	const char *url = getPostReadingUrl();

	http.begin(client, url);
	http.addHeader("Content-Type", "application/json");
	http.PUT(request);
	http.end();
}

void ApiClass::getReadings() {
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

void ApiClass::getStatus() {
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

const char * ApiClass::getPostReadingUrl() {
	static char buffer[64];
	strcpy(buffer, SERVER_URL);
	strcat(buffer, "reading/");
	strcat(buffer, Flash.stationID);
	strcat(buffer, "/new");

	return buffer;
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_API)
ApiClass Api;
#endif