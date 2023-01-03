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

class FlashClass {
	private:
		void readString(char *buffer, int start, int end);
		void writeString(const String &val, int start, int end);
		String getStationID();
		void setStationID(const String &newStationID);
	public:
		void begin();
		String getSSID();
		void setSSID(const String &newSSID);
		String getPassword();
		void setPassword(const String &newPassword);
		String stationID;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_FLASH)
extern FlashClass Flash;
#endif

#endif