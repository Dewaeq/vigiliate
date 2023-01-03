#include "sensors.h"

void SensorsClass::begin() {
    sds.begin();
}

void SensorsClass::update() {
    sds.update();
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORS)
SensorsClass Sensors;
#endif