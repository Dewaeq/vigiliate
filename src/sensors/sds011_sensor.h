#ifndef SDS_H
#define SDS_H

#include <SdsDustSensor.h>

#define SENSOR_TX D7
#define SENSOR_RX D8

// measure during 30 seconds
#define MEASURE_TIME 30 * 1000
// sleep for 30 minutes
#define SLEEP_TIME 30 * 60 * 1000

struct SDS011Reading {
    float pm25;
    float pm10;
    bool success;

    SDS011Reading() {};
    SDS011Reading(float pm25, float pm10, bool success): pm25(pm25), pm10(pm10), success(success) {};
};

enum State { Sleeping, Reading };

class SDS011Sensor {
    public:
        SDS011Sensor() {};
        void begin();
        void update();
        bool isSleeping();
        SDS011Reading result;
    private:
        SdsDustSensor sensor = SdsDustSensor(SENSOR_TX, SENSOR_RX);
        State state = State::Sleeping;
        unsigned long previousMillis;
        void read();
        void wake();
        void sleep();
};

#endif