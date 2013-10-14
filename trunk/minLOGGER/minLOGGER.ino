/**
 ******************************************************************************
 *
 * @file       minLOGGER.ino
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


#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 


//#define membug


#ifdef GPS_PROTOCOL_DJI
#define SERIAL_SPEED  115200	// How fast our data is coming to serial port
#else
#define SERIAL_SPEED  57600	// How fast our data is coming to serial port
#endif


/* **********************************************/
/* ***************** INCLUDES *******************/

// Get the common arduino functions
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif

#ifdef membug
#include <MemoryFree.h>
#endif

#include "Config.h"
#include "Vars.h"

#include "Logger.h"


void setup() 
{
    Serial.begin(SERIAL_SPEED);

#ifdef membug
    Serial.println(freeMem());
#endif

    logger_init();

} // END of setup();


void home_check()
{
    static uint8_t log_alt_cnt = 0;
    static float log_alt_prev = 0;
  
    // criteria for a stable home position:
    //  - GPS fix
    //  - with at least 5 satellites
    //  - log_alt stable for 30 * 100ms = 3s
    //  - log_alt stable means the delta is lower 0.5m
    if (log_fix_type > 1 && log_satellites_visible >= 5 && log_alt_cnt < 30) {
        if (fabs(log_alt_prev - log_alt) > 0.5) {
            log_alt_cnt = 0;
            log_alt_prev = log_alt;
        } else {
            if (++log_alt_cnt >= 30) {
                log_home_alt = log_alt;  	// take this stable log_alt as log_home_alt
	        log_got_home = 1;
            }
        }
    }
}


void loop() 
{
    logger_read();
    if (!log_got_home) {
	home_check();
    } else {
	logger_write();
    }
}
