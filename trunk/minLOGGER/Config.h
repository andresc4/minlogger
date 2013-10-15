//
//
//


#define LOGGING_MS		100			// -> 10 Hz logging
#define FLUSH_CNT		100			// SD flush is performed every LOGGING_MS * FLUSH_CNT ms

#define BARO_READ_MS		100			// -> 10 Hz baro reading


// ground tested but not in flight because of weather conditions !!!
// heading needs some work !!!
// GPS protocol DJI:
//#define GPS_PROTOCOL_DJI

// GPS protocol UBX:
#define GPS_PROTOCOL_UBX

// untested !!!
// GPS protocol NMEA:
//#define GPS_PROTOCOL_NMEA

// Barometer available:
//#define BAROMETER




#define ERROR_SD_BEGIN		1
#define ERROR_SD_WRITE		2
