#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hmi_msg.h"
#include "uart-wrapper.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"

#define BAUD 9600
#define BLINK_DELAY_MS 100

volatile uint32_t time;

static inline void init_system_clock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}

static inline void hw_init() {
    DDRA |= _BV(DDA3); // setup Arduino pin 25 as output
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
    init_system_clock();
    sei(); // Enable interupts
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
}

static inline void print_prog_version() {
    fprintf_P(stderr, PSTR(PROG_VERSION),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(LIBC_VERSION), PSTR(__AVR_LIBC_VERSION_STRING__));
}


static inline void print_program_startup() {
    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout); /* Add a new line to the uart printout */
    lcd_puts_P(PSTR(STUD_NAME));

    /* ASCII table print */
    print_ascii_tbl(stdout);
    unsigned char ascii[128];
    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }
    print_for_human(stdout, ascii, sizeof(ascii));

    fprintf_P(stdout, PSTR(GET_MONTH_MSG));
}


static inline void search_month() {
    /* Month search and print */
    char letter;
    fscanf(stdin, "%c", &letter);
    fprintf(stdout, "%c\n", letter);
    lcd_goto(0x40); /* Got to the beginning of the next line */
    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(&letter, (PGM_P)pgm_read_word(&months[i]), 1)) {
            fprintf_P(stdout, (PGM_P)pgm_read_word(&months[i]));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
            lcd_putc(' ');
        }
    }
    fprintf_P(stdout, PSTR(GET_MONTH_MSG));
    lcd_puts_P(PSTR("                ")); /* Clear the end of the line */
}

static inline void heartbeat() {
    static uint32_t last_time;
    uint32_t cur_time = time;
    if ((last_time - cur_time) > 0) {
        // Toggle led on arduino pin 25
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME "\n"), cur_time);
    }
    last_time = cur_time;
}

int main (void)
{

    hw_init();
    print_prog_version();
    print_program_startup();


    while (1) {
        heartbeat();
        if(uart0_available()) {
            search_month();
        }
    }
}

/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    time++;
}
