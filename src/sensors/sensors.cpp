#include "sensors.h"
#include "gui.h"

void SensorsClass::begin() {
    sds.begin();
    dht.begin();
}

void SensorsClass::update() {
    sds.update();
    dht.update();
    getReadings();
}

SensorsReading SensorsClass::getReadings() {
    SensorsReading reading;
    reading.sds = sds.result;
    reading.dht = dht.result;

    Gui.setBody("Temp: ");
    Gui.appendBody(String(reading.dht.temperature));
    Gui.appendBody("Hum: ", true);
    Gui.appendBody(String(reading.dht.humitidy));
    Gui.appendBody("PM2.5: ", true);
    Gui.appendBody(String(reading.sds.pm25));
    Gui.appendBody("PM10: ", true);
    Gui.appendBody(String(reading.sds.pm10));
    Gui.show();

    return reading;
}


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORS)
SensorsClass Sensors;
#endif