#ifndef SENSORS_H
#define SENSORS_H

#include "sds011_sensor.h"
#include "dht22_sensor.h"
#include "timer.h"

#define GUI_UPDATE_TIME 5 * 1000

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
        Timer timer = Timer(GUI_UPDATE_TIME);
        SDS011Sensor sds;
        DHT22Sensor dht;
        void showReadings();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORS)
extern SensorsClass Sensors;
#endif

#endif