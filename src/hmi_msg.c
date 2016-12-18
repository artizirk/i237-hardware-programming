#include <avr/pgmspace.h>
#include "hmi_msg.h"

static const char m1[] PROGMEM = "January";
static const char m2[] PROGMEM = "February";
static const char m3[] PROGMEM = "March";
static const char m4[] PROGMEM = "April";
static const char m5[] PROGMEM = "May";
static const char m6[] PROGMEM = "June";

PGM_P const months[] PROGMEM = {m1, m2, m3, m4, m5, m6};

const char help_cmd[] PROGMEM = HELP_CMD;
const char help_help[] PROGMEM = HELP_HELP;
const char ver_cmd[] PROGMEM = VER_CMD;
const char ver_help[] PROGMEM = VER_HELP;
const char ascii_cmd[] PROGMEM = ASCII_CMD;
const char ascii_help[] PROGMEM = ASCII_HELP;
const char month_cmd[] PROGMEM = MONTH_CMD;
const char month_help[] PROGMEM = MONTH_HELP;
const char read_cmd[] PROGMEM = READ_CMD;
const char read_help[] PROGMEM = READ_HELP;
const char add_cmd[] PROGMEM = ADD_CMD;
const char add_help[] PROGMEM = ADD_HELP;
const char remove_cmd[] PROGMEM = REMOVE_CMD;
const char remove_help[] PROGMEM = REMOVE_HELP;
const char list_cmd[] PROGMEM = LIST_CMD;
const char list_help[] PROGMEM = LIST_HELP;
