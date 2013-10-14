/**
 ******************************************************************************
 *
 * @file       Logger.ino
 * @author     Joerg-D. Rothfuchs
 * @brief      Implements a logger.
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/> or write to the 
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <SD.h>

#include "Logger.h"


#ifdef GPS_PROTOCOL_DJI
// see http://www.rcgroups.com/forums/showthread.php?t=1995704
#include "GPS_DJI.h"
#endif


#ifdef GPS_PROTOCOL_UBX
// see http://openpilot.org
#include "GPS_UBX.h"
#endif


#ifdef GPS_PROTOCOL_NMEA
// see http://arduiniana.org/libraries/tinygps/
#include <TinyGPS.h>
TinyGPS gps;
#endif


#ifdef BAROMETER
#include "Barometer.h"
#include <Wire.h>
Barometer myBarometer;
#endif


File logFile;


int logger_init(void) {
	int i = 1;
	char fileName[12];
	
	pinMode(10, OUTPUT);
	
	if (!SD.begin(10)) {
		return ERROR_SD_BEGIN;		// ERROR card not present or can't be initialized
	}
	
	do {	// search first unused file name
		sprintf(fileName, "%08u.log", i++);
	} while (SD.exists(fileName));

	if (logFile = SD.open(fileName, FILE_WRITE)) {
		logFile.print("Latitude,Longitude,Altitude,FixType,SatCnt");
#ifdef BAROMETER
		logFile.print(",BaroAltitude");
#endif
		logFile.println();
		logFile.flush();
	} else {
		return ERROR_SD_WRITE;		// ERROR can't write
	}

#ifdef BAROMETER
	myBarometer.init();
#endif
	
	return 0;
}


int logger_read(void) {
	
	// grabbing gps data
	while (Serial.available() > 0) {
		uint8_t c = Serial.read();
		
#ifdef GPS_PROTOCOL_DJI
		if (parse_dji(c) == PARSER_COMPLETE_SET) {
			log_fix_type		= get_dji_status();
			log_satellites_visible	= get_dji_satellites();
			log_lat			= get_dji_latitude();
			log_lon			= get_dji_longitude();
			log_alt			= get_dji_altitude();
			log_heading 		= get_dji_heading();
			log_groundspeed 	= get_dji_groundspeed();
			log_climb		= -1.0 * get_dji_down();
		}
#endif
		
#ifdef GPS_PROTOCOL_UBX
		if (parse_ubx(c) == PARSER_COMPLETE_SET) {
			log_fix_type		= get_ubx_status();
			log_satellites_visible	= get_ubx_satellites();
			log_lat			= get_ubx_latitude();
			log_lon			= get_ubx_longitude();
			log_alt			= get_ubx_altitude();
			log_heading 		= get_ubx_heading();
			log_groundspeed 	= get_ubx_groundspeed();
			log_climb		= -1.0 * get_ubx_down();
		}
#endif

#ifdef GPS_PROTOCOL_NMEA
		long lat, lon;
		unsigned long fix_age;
	
		if (gps.encode(c)) {		// process new gps info
			gps.get_position(&lat, &lon, &fix_age);
			log_lat			= lat / 1000000.0;
			log_lon			= lon / 1000000.0;
			
			// log_fix_type should be  0-1=no fix, 2=2D, 3=3D
			if (fix_age == TinyGPS::GPS_INVALID_AGE)		// TODO	temporary solution, optionally use info from GSA message (but not implemented in TinyGPS lib yet)
				log_fix_type = 0;
			else if (fix_age > 5000)
				log_fix_type = 2;
			else
				log_fix_type = 3;
			
			log_satellites_visible	= gps.satellites();
			log_heading 		= gps.f_course();
			log_alt			= gps.f_altitude();
			log_groundspeed 	= gps.f_speed_mps();
			//log_climb		= 0;				// TODO not implemented yet
		}
#endif
	}
	
#ifdef BAROMETER
	log_baro_temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT());	// get the temperature, bmp085ReadUT MUST be called first
	log_baro_pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());		// get the temperature
	log_baro_alt = myBarometer.calcAltitude(log_baro_pressure);				// uncompensated caculation [m]
#endif

        return 0;
}


int logger_write(void) {
	static long lastlog = 0;
	static uint8_t flush_cnt = 0;
	
	if (millis() - lastlog < LOGGING_MS) return 0;
	lastlog = millis();
	
	if (logFile) {
		logFile.print(log_lat, 8);
		logFile.print(",");
		logFile.print(log_lon, 8);
		logFile.print(",");
		logFile.print(log_alt);
		logFile.print(",");
		logFile.print(log_fix_type);
		logFile.print(",");
		logFile.print(log_satellites_visible);
#ifdef BAROMETER
		logFile.print(",");
		logFile.print(log_baro_alt);
#endif
		logFile.println();
		if (flush_cnt++ >= FLUSH_CNT) {
			logFile.flush();
			flush_cnt = 0;
		}
	} else {
		return ERROR_SD_WRITE;		// ERROR can't write
	}
	
        return 0;
}
