#include "dht22_sensor.h"

void DHT22Sensor::begin() {
    sensor.begin();
}

void DHT22Sensor::update() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= SLEEP_TIME) {
        read();
        
        previousMillis = currentMillis;
    }
}

void DHT22Sensor::read() {
    result = DHT22Reading(0, 0, false);

    float hum = sensor.readHumidity();
    float temp = sensor.readTemperature();

    if (!isnan(hum) && !isnan(temp)) {
        result.humitidy = hum;
        result.temperature = temp;
        result.succes = true;
    }

    Serial.print("Temperature: ");
    Serial.println(String(result.temperature));
    Serial.print("Humidity: ");
    Serial.println(String(result.humitidy));
}