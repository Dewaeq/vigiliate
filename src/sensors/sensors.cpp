#include "sensors.h"

void SensorsClass::begin() {
    sds.begin();
    dht.begin();
}

void SensorsClass::update() {
    sds.update();
    dht.update();
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