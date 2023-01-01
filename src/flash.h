// Can't use ifndef on this header, or compilation will fail
// TODO: investigate why this happens (currently clueless)
// #ifndef FLASH_H
// #define FLASH_H

#include <Arduino.h>

// start values are inclusive, end values are exclusive
#define MEM_SSID_START 0
#define MEM_SSID_END 32
#define MEM_PASS_START 32
#define MEM_PASS_END 64
#define MEM_STATION_ID_START 64
#define MEM_STATION_ID_END 70

class Flash {
	private:
		static String readString(int start, int end);
		static void writeString(String val, int start, int end);
	public:
		static String getSSID();
		static void setSSID(String newSSID);
		static String getPassword();
		static void setPassword(String newPassword);
		static String getStationID();
		static void setStationID(String newStationID);
};

// #endif