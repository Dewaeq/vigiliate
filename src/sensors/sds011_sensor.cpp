#include "sds011_sensor.h"

void SDS011Sensor::begin() {
    sensor.begin();
    sensor.setQueryReportingMode();
}

void SDS011Sensor::update() {
    unsigned long currentMillis = millis();
    unsigned long interval = isSleeping() ? SLEEP_TIME : MEASURE_TIME;

    if (currentMillis - previousMillis >= interval) {
        // stop sleep and start reading
        if (isSleeping()) {
            wake();
        } 
        // read values and send sensor to bed
        else {
            read();
            sleep();
        }

        previousMillis = currentMillis;
    }
}

bool SDS011Sensor::isSleeping() {
    return state == State::Sleeping;
}

void SDS011Sensor::wake() {
    state = State::Reading;
    sensor.wakeup();
}

void SDS011Sensor::sleep() {
    state = State::Sleeping;

    WorkingStateResult state = sensor.sleep();
    if (state.isWorking()) {
        Serial.println("Failed to make sds011 sensor sleep!");
    }
}

void SDS011Sensor::read() {
    result = SDS011Reading(0, 0, false);

    PmResult pm = sensor.queryPm();

    if (pm.isOk()) {
        result.pm10 = pm.pm10;
        result.pm25 = pm.pm25;
        result.succes = true;
    }
}