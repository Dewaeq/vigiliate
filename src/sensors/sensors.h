#ifndef SENSORS_H
#define SENSORS_H

#include "sds011.h"

struct SensorsReading {
    SDS011Reading sds;
};

class SensorsClass {
    public:
        void begin();
        void update();
        SensorsReading getReadings();
    private:
        SDS011 sds;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORS)
extern SensorsClass Sensors;
#endif

#endif