#ifndef SENSORS_H
#define SENSORS_H

#include "sds011.h"

class SensorsClass {
    public:
        void begin();
        void update();
    private:
        SDS011 sds;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORS)
extern SensorsClass Sensors;
#endif

#endif