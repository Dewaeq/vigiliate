#include "flash.h"
#include "utils.h"
#include <EEPROM.h>
#include <Arduino.h>

void FlashClass::begin() {
	EEPROM.begin(70);

	stationID = getStationID();
}

String FlashClass::readString(int start, int end) {
	String result;
	for (int i = start; i < end; i++) {
		int val = EEPROM.read(i);
		if (val == 0 || val == 255) {
			break;
		}

		result += char(val);
	}

	return result;
}

void FlashClass::writeString(String val, int start, int end) {
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

String FlashClass::getSSID() {
	return readString(MEM_SSID_START, MEM_SSID_END);
}

void FlashClass::setSSID(String newSSID) {
	if (newSSID.length() > 32) {
		Serial.println(F("SSID is too long to store!"));
		return;
	}
	
	writeString(newSSID, MEM_SSID_START, MEM_SSID_END);
}

String FlashClass::getPassword() {
	return readString(MEM_PASS_START, MEM_PASS_END);
}

void FlashClass::setPassword(String newPassword) {
	if (newPassword.length() > 32) {
		Serial.println(F("Password is too long to store!"));
		return;
	}
	
	writeString(newPassword, MEM_PASS_START, MEM_PASS_END);
}

String FlashClass::getStationID() {
	String id = readString(MEM_STATION_ID_START, MEM_STATION_ID_END);
	if (id == "") {
		char buffer[7];
		getRandomString(buffer, 7);
		id = String(buffer);

		setStationID(id);
	}

	return id;
}

void FlashClass::setStationID(String newStationID) {
	writeString(newStationID, MEM_STATION_ID_START, MEM_STATION_ID_END);
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_FLASH)
FlashClass Flash;
#endif
