/*   Copyright (C) 2016 Arti Zirk <arti.zirk@gmail.com>
 *
 *   This file is part of I237 Door Access program.
 *
 *   I237 Door Access is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   I237 Door Access is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with I237 Door Access.  If not, see <http://www.gnu.org/licenses/>.
 */
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
