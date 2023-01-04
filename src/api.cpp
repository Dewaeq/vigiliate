#include "api.h"
#include "json.h"
#include "webserver.h"
#include "wifi.h"
#include "flash.h"
#include "sensors/sensors.h"

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

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_API)
ApiClass Api;
#endif