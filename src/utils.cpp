#include "utils.h"
#include <Arduino.h>

void getRandomString(char value[], int length) {
	randomSeed(getRandomSeed());

	for (int i = 0; i < length - 1; i++) {
		size_t index = random(0, 62);
		value[i] = CHARS[index];
	}

	value[length - 1] = '\0';
}

unsigned long getRandomSeed() {
	unsigned long seed = 0UL;
	unsigned long reading = 0UL;
	int interval = 5;
	int bit = 0;

	for (int i = 0; i < 31; i++) {
		for (int j = 0; j < 3; j++) {
			delay(2 + interval);

			reading = analogRead(A0);
			bit ^= reading & 1;
			// randomly chosen numbers, no concrete meaning
			interval = (reading % 6) * 10;
		}
		seed |= (long)bit << i;
	}

	return seed;
}
