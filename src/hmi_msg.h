#include <avr/pgmspace.h>
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

const char PROG_VERSION[]  PROGMEM = "Version: %s built on: %s %s\n";
const char LIBC_VERSION[]  PROGMEM = "avr-libc version: %s\n";
const char STUD_NAME[]     PROGMEM = "Arti Zirk";
const char GET_MONTH_MSG[] PROGMEM = "Enter Month name first letter >";
const char ENG_MONTH[6][9] PROGMEM = {
                                        "January",
                                        "February",
                                        "March",
                                        "April",
                                        "May",
                                        "June",
                                      };

#endif /* _HMI_MSG_H_ */
