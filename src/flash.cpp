#include "flash.h"
#include "utils.h"
#include <EEPROM.h>
#include <Arduino.h>

void FlashClass::begin() {
	EEPROM.begin(70);

	char *result = getStationID();
	strcpy(stationID, result);
}

void FlashClass::readString(char *buffer, int start, int end) {
	int last = end - start;

	for (int i = start; i < end; i++) {
		int val = EEPROM.read(i);
		if (val == 0 || val == 255) {
			last = i - start;
			break;
		}

		buffer[i - start] = char(val);
	}

	buffer[last] = '\0';
}

void FlashClass::writeString(const String &val, int start, int end) {
	uint8_t size = val.length();
	for (int i = start; i < end; i++) {
		if (i - start < size) {
			EEPROM.write(i, val[i - start]);
		} else {
			EEPROM.write(i, 0);
		}
	}

	EEPROM.commit();
}

char *FlashClass::getSSID() {
	static char buffer[32];
	readString(buffer, MEM_SSID_START, MEM_SSID_END);

	return buffer;
}

void FlashClass::setSSID(const String &newSSID) {
	if (newSSID.length() > 32) {
		Serial.println(F("SSID is too long to store!"));
		return;
	}
	
	writeString(newSSID, MEM_SSID_START, MEM_SSID_END);
}

char *FlashClass::getPassword() {
	static char buffer[32];
	readString(buffer, MEM_PASS_START, MEM_PASS_END);

	return buffer;
}

void FlashClass::setPassword(const String &newPassword) {
	if (newPassword.length() > 32) {
		Serial.println(F("Password is too long to store!"));
		return;
	}
	
	writeString(newPassword, MEM_PASS_START, MEM_PASS_END);
}

char *FlashClass::getStationID() {
	static char buffer[7];

	readString(buffer, MEM_STATION_ID_START, MEM_STATION_ID_END);

	if (strcmp(buffer, "") == 0) {
		getRandomString(buffer, 7);
		setStationID(buffer);
	}

	return buffer;
}

void FlashClass::setStationID(const String &newStationID) {
	writeString(newStationID, MEM_STATION_ID_START, MEM_STATION_ID_END);
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_FLASH)
FlashClass Flash;
#endif
