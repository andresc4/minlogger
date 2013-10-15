//
//
//


static uint8_t      log_got_home = 0;               // home found
static long         log_got_home_millis = 0;        // timestamp of got home
static float        log_home_alt = 0;               // home altitude

static uint8_t      log_fix_type = 0;               // GPS lock
static uint8_t      log_satellites_visible = 0;     // number of satelites
static float        log_lat = 0;                    // latitude
static float        log_lon = 0;                    // longitude
static float        log_alt = 0;                    // altitude
static float        log_heading = 0;                // ground course heading
static float        log_groundspeed = 0;            // ground speed
static float        log_climb = 0;                  // climb rate


#ifdef BAROMETER
//static float        log_temperature = 0;
static float        log_pressure = 0;
#endif
