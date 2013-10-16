//
//
//


#define LOGGING_MS		100			// -> 10 Hz logging
#define FLUSH_CNT		100			// SD flush is performed every LOGGING_MS * FLUSH_CNT ms

// Attention: You have to set 'const unsigned char OSS = 3;' in Barometer.h to get good results
// To do so use the Barometer_Sensor library of my committed code!
#define BARO_READ_MS		50			// max every 50ms baro reading (for oversampling = 3 the pressure conversion time is 25.5ms)


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
