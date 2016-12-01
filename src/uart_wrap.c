#include <avr/io.h>
#include <stdio.h>
#include "../lib/andygock_avr-uart/uart.h"

#define UART_STATUS_MASK    0x00FF

int uart0_putc_wrap(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart0_putc_wrap('\r', stream);
    }

    uart0_putc(c);
    return 0;
}

int uart0_getc_wrap(FILE *stream)
{
    (void) stream;
    // Probabbly should add some error checking in here but because
    // this function is only called out when there is at least one character
    // available in the input buffer (see main.c line 114) then error checking
    // is not currently necessary.
    return uart0_getc() & UART_STATUS_MASK;
}

int uart3_putc_wrap(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart3_putc_wrap('\r', stream);
    }

    uart3_putc(c);
    return 0;
}
