#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hmi_msg.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "uart_wrap.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"

#define BAUDRATE 9600
#define BLINK_DELAY_MS 100

// For configuring arduino mega pin 25
#define LED_INIT DDRA |= _BV(DDA3);
#define LED_ON PORTA |= _BV(PORTA3)
#define LED_OFF PORTA &= ~_BV(PORTA3)
#define LED_DELAY _delay_ms(BLINK_DELAY_MS)

static inline void init_hw (void)
{
    // IO init
    /// Set arduino pin 25 as output
    LED_INIT;

    // UART init
    uart0_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;

    // LCD init
    lcd_init();
    lcd_clrscr();

    // Enable interupts
    sei();
}

static inline void start_ui (void)
{
    // Print program and libc versions
    fprintf_P(stderr, PSTR(PROG_VERSION),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(LIBC_VERSION), PSTR(__AVR_LIBC_VERSION_STRING__));

    // print student name
    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout); // Add a new line to the uart printout
    lcd_puts_P(PSTR(STUD_NAME));

    // ASCII table print
    print_ascii_tbl(stdout);
    unsigned char ascii[128];
    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }
    print_for_human(stdout, ascii, sizeof(ascii));

    fprintf_P(stdout, PSTR(GET_MONTH_MSG));
}

static inline void search_month (void)
{
    char letter;

    fscanf(stdin, "%c", &letter);
    fprintf(stdout, "%c\n", letter);
    lcd_goto(0x40); // Got to the beginning of the next line
    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(&letter, (PGM_P)pgm_read_word(&months[i]), 1)) {
            fprintf_P(stdout, (PGM_P)pgm_read_word(&months[i]));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
            lcd_putc(' ');
        }
    }

    lcd_puts_P(PSTR("                ")); // Clear the end of the line
    fprintf_P(stdout, PSTR(GET_MONTH_MSG));
}

int main (void)
{
    init_hw();
    start_ui();

    while (1) {
        LED_ON;
        LED_DELAY;

        if (uart0_available()) {
            search_month();
        }

        LED_OFF;
        LED_DELAY;
    }
}
