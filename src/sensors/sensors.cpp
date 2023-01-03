#include "sensors.h"

void SensorsClass::begin() {
    sds.begin();
}

void SensorsClass::update() {
    sds.update();
}

SensorsReading SensorsClass::getReadings() {
    SensorsReading reading;
    reading.sds = sds.result;

    return reading;
}


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORS)
SensorsClass Sensors;
#endif