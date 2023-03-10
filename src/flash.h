#ifndef VIGILIATE_FLASH_H
#define VIGILIATE_FLASH_H

#include <Arduino.h>

// start values are inclusive, end values are exclusive
#define MEM_SSID_START 0
#define MEM_SSID_END 32
#define MEM_PASS_START 32
#define MEM_PASS_END 64
#define MEM_STATION_ID_START 64
#define MEM_STATION_ID_END 70

// subtract one for null byte
#define MAX_SSID_SIZE MEM_SSID_END - MEM_SSID_START
#define MAX_PASS_SIZE MEM_PASS_END - MEM_PASS_START

#define FLASH_MEM_SIZE 70

class FlashClass {
	private:
		void readString(char *buffer, int start, int end);
		void writeString(const String &val, int start, int end);
		void writeString(const char *val, int start, int end);
		char *getStationID();
		void setStationID(const char *newStationID);
	public:
		void begin();
		char *getSSID();
		void setSSID(const String &newSSID);
		char *getPassword();
		void setPassword(const String &newPassword);
		char stationID[7];
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_FLASH)
extern FlashClass Flash;
#endif

#endif