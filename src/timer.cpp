#include "timer.h"
#include <Arduino.h>

void Timer::setInterval(unsigned long interval) {
    this->interval = interval;
}

bool Timer::isOver() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        return true;
    }

    return false;
}