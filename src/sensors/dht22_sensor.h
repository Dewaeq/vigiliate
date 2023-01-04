#ifndef DHT22_H
#define DHT22_H

#include <DHT.h>

#define SENSOR_PIN D6
#define SENSOR_TYPE DHT22

#define SLEEP_TIME 10 * 1000

struct DHT22Reading {
    float humitidy;
    float temperature;
    bool succes;

    DHT22Reading() {};
    DHT22Reading(float hum, float temp, bool succes): humitidy(hum), temperature(temp), succes(succes) {};
};

class DHT22Sensor {
    public:
        DHT22Sensor() {};
        void begin();
        void update();
        DHT22Reading result;
    private:
        DHT sensor = DHT(SENSOR_PIN, SENSOR_TYPE);
        unsigned long previousMillis;
        void read();
};

#endif