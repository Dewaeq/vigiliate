#include "sds011_sensor.h"
#include "gui.h"

void SDS011Sensor::begin() {
    sensor.begin();
    sensor.setQueryReportingMode();

    delay(500);

    wake();
}

void SDS011Sensor::update() {
    if (timer.isOver()) {
        // stop sleep and start reading
        if (isSleeping()) {
            wake();
        } 
        // read values and send sensor to bed
        else {
            read();
            sleep();
        }
    }
}

bool SDS011Sensor::isSleeping() {
    return state == State::Sleeping;
}

void SDS011Sensor::wake() {
    state = State::Reading;
    timer.setInterval(SDS_MEASURE_TIME);
    sensor.wakeup();
}

void SDS011Sensor::sleep() {
    state = State::Sleeping;
    timer.setInterval(SDS_SLEEP_TIME);

    WorkingStateResult state = sensor.sleep();
    if (state.isWorking()) {
        Serial.println(F("Failed to make sds011 sensor sleep!"));
        Gui.showError("Failed to make sds011 sleep");
    }
}

void SDS011Sensor::read() {
    result = SDS011Reading(0, 0, false);

    PmResult pm = sensor.queryPm();

    if (pm.isOk()) {
        result.pm10 = pm.pm10;
        result.pm25 = pm.pm25;
        result.success = true;
    } else {
        Gui.showError("Failed to read from sds011");
    }
}