#include <avr/pgmspace.h>
#include "hmi_msg.h"

static const char m1[] PROGMEM = "January";
static const char m2[] PROGMEM = "February";
static const char m3[] PROGMEM = "March";
static const char m4[] PROGMEM = "April";
static const char m5[] PROGMEM = "May";
static const char m6[] PROGMEM = "June";

extern PGM_P const months[] PROGMEM = {m1,m2,m3,m4,m5,m6};
