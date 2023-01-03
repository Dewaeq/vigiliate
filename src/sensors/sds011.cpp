#include "sds011.h"

void SDS011::begin() {
    sensor.begin();
    sensor.setQueryReportingMode();
}

void SDS011::update() {
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

bool SDS011::isSleeping() {
    return state == State::Sleeping;
}

void SDS011::wake() {
    state = State::Reading;
    sensor.wakeup();
}

void SDS011::sleep() {
    state = State::Sleeping;

    WorkingStateResult state = sensor.sleep();
    if (state.isWorking()) {
        Serial.println("Failed to make sds011 sensor sleep!");
    }
}

void SDS011::read() {
    result = SDS011Reading(0, 0, false);

    PmResult pm = sensor.queryPm();

    if (pm.isOk()) {
        result.pm10 = pm.pm10;
        result.pm25 = pm.pm25;
        result.succes = true;
    }
}