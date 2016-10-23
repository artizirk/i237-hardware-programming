#include <avr/pgmspace.h>
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#define PROG_VERSION "Version: "GIT_DESCR" built on: "__DATE__" "__TIME__"\n"
#define LIBC_VERSION "avr-libc version: "__AVR_LIBC_VERSION_STRING__"\n"
#define STUD_NAME "Arti Zirk"
#define GET_MONTH_MSG "Enter Month name first letter >"
const char ENG_MONTH[6][9] PROGMEM = {
                                        "January",
                                        "February",
                                        "March",
                                        "April",
                                        "May",
                                        "June",
                                      };

                                            };*/
#endif /* _HMI_MSG_H_ */
