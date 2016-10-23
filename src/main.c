#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hmi_msg.h"
#include "uart.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 100

int main (void)
{
    /* Init */
    DDRA |= _BV(DDA3);
    uart0_init();
    uart3_init();
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
    /* End init */

    /* Print version info to stderr */
    fprintf_P(stderr, PSTR(PROG_VERSION));
    fprintf_P(stderr, PSTR(LIBC_VERSION));
    /* End version print */

    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout); /* Add a new line to the uart printout */
    lcd_puts_P(PSTR(STUD_NAME));

    /* ASCII table print */
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};
    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }
    print_for_human(stdout, ascii, sizeof(ascii));

    while (1) {
        /* set pin 3 high to turn led on */
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);

        /* Month search and print */
        char month_first_leter;
        fprintf_P(stdout, PSTR(GET_MONTH_MSG));
        fscanf(stdin, "%c", &month_first_leter);
        fprintf(stdout, "%c\n", month_first_leter);
        lcd_goto(0x40); /* Got to the beginning of the next line */
        for (int i = 0; i < 6; i++) {
            if (!strncmp_P(&month_first_leter, ENG_MONTH[i], 1)) {
                fprintf_P(stdout, ENG_MONTH[i]);
                fputc('\n', stdout);
                lcd_puts_P(ENG_MONTH[i]);
                lcd_putc(' ');
            }
        }

        lcd_puts_P(PSTR("                ")); /* Clear the end of the line */


        /* set pin 3 low to turn led off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}
