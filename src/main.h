#ifndef MAIN_H
#define MAIN_H

#include "timer.h"

void setupWebServer();
void setStatus();
void reboot();

// Reboot every two days, to circumvent memory fragmentation and other edge case bugs
Timer rebootTimer(1000 * 3600 * 24 * 2);

#endif
