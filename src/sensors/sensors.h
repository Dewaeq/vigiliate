#ifndef SENSORS_H
#define SENSORS_H

#include "sds011_sensor.h"
#include "dht22_sensor.h"

struct SensorsReading {
    SDS011Reading sds;
    DHT22Reading dht;
};

class SensorsClass {
    public:
        void begin();
        void update();
        SensorsReading getReadings();
    private:
        SDS011Sensor sds;
        DHT22Sensor dht;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORS)
extern SensorsClass Sensors;
#endif

#endif