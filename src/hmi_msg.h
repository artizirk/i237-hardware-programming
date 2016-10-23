#include <avr/pgmspace.h>
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define PROG_VERSION "Version: "GIT_DESCR" built on: "__DATE__" "__TIME__"\n"
#define LIBC_VERSION "avr-libc version: "__AVR_LIBC_VERSION_STRING__"\n"
#define STUD_NAME "Arti Zirk"
#define GET_MONTH_MSG "Enter Month name first letter >"

const char JANUARY[]  PROGMEM = "January";
const char FEBRUARY[] PROGMEM = "February";
const char MARCH[]    PROGMEM = "March";
const char APRIL[]    PROGMEM = "April";
const char MAY[]      PROGMEM = "May";
const char JUNE[]     PROGMEM = "June";

PGM_P const ENG_MONTH[] PROGMEM = {
                                    JANUARY,
                                    FEBRUARY,
                                    MARCH,
                                    APRIL,
                                    MAY,
                                    JUNE,
                                  };
#endif /* _HMI_MSG_H_ */
