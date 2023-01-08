#include "sensors.h"
#include "gui.h"

void SensorsClass::begin() {
    sds.begin();
    dht.begin();
}

void SensorsClass::update() {
    sds.update();
    dht.update();

    if (timer.isOver()) {
        showReadings();
    }
}

void SensorsClass::showReadings() {
    SensorsReading reading = getReadings();

    Gui.setBody("Temp: ");
    Gui.appendBody(String(reading.dht.temperature));
    Gui.appendBody("Hum: ", true);
    Gui.appendBody(String(reading.dht.humitidy));
    Gui.appendBody("PM2.5: ", true);
    Gui.appendBody(String(reading.sds.pm25));
    Gui.appendBody("PM10: ", true);
    Gui.appendBody(String(reading.sds.pm10));
    Gui.show();
}

SensorsReading SensorsClass::getReadings() {
    SensorsReading reading;
    reading.sds = sds.result;
    reading.dht = dht.result;

    return reading;
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORS)
SensorsClass Sensors;
#endif