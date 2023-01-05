#ifndef DHT22_H
#define DHT22_H

#include <DHT.h>
#include "timer.h"

#define SENSOR_PIN D6
#define SENSOR_TYPE DHT22

#define DHT_SLEEP_TIME 10 * 1000

struct DHT22Reading {
    float humitidy;
    float temperature;
    bool success;

    DHT22Reading() {};
    DHT22Reading(float hum, float temp, bool success): humitidy(hum), temperature(temp), success(success) {};
};

class DHT22Sensor {
    public:
        DHT22Sensor() {};
        void begin();
        void update();
        DHT22Reading result;
    private:
        DHT sensor = DHT(SENSOR_PIN, SENSOR_TYPE);
        Timer timer = Timer(DHT_SLEEP_TIME);
        void read();
};

#endif