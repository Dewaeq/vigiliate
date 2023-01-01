#include "flash.h"
#include "utils.h"
#include <EEPROM.h>
#include <Arduino.h>

void Flash::begin() {
	EEPROM.begin(70);
}

String Flash::readString(int start, int end) {
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

void Flash::writeString(String val, int start, int end) {
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

String Flash::getSSID() {
	return Flash::readString(MEM_SSID_START, MEM_SSID_END);
}

void Flash::setSSID(String newSSID) {
	if (newSSID.length() > 32) {
		Serial.println(F("SSID is too long to store!"));
		return;
	}
	
	Flash::writeString(newSSID, MEM_SSID_START, MEM_SSID_END);
}

String Flash::getPassword() {
	return Flash::readString(MEM_PASS_START, MEM_PASS_END);
}

void Flash::setPassword(String newPassword) {
	if (newPassword.length() > 32) {
		Serial.println(F("Password is too long to store!"));
		return;
	}
	
	Flash::writeString(newPassword, MEM_PASS_START, MEM_PASS_END);
}

String Flash::getStationID() {
	String id = Flash::readString(MEM_STATION_ID_START, MEM_STATION_ID_END);
	if (id == "") {
		char buffer[7];
		getRandomString(buffer, 7);
		id = String(buffer);

		Flash::setStationID(id);
	}

	return id;
}

void Flash::setStationID(String newStationID) {
	Flash::writeString(newStationID, MEM_STATION_ID_START, MEM_STATION_ID_END);
}