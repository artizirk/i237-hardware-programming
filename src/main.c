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
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hmi_msg.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "uart_wrap.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/helius_microrl/microrl.h"
#include "cli_microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "rfid.h"


#define BAUDRATE 9600

// For configuring arduino mega pin 25
#define LED_INIT DDRA |= _BV(DDA3)
#define DOOR_INIT DDRA |= _BV(DDA1)
#define LED_TOGGLE PORTA ^= _BV(PORTA3)
#define DOOR_OPEN PORTA |= _BV(PORTA1)
#define DOOR_CLOSE PORTA &= ~_BV(PORTA1)
#define UART_STATUS_MASK    0x00FF

// Current system time
volatile uint32_t system_time;

// Create microrl object and pointer on it
static microrl_t rl;
static microrl_t *prl = &rl;


static inline void init_system_clock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}

static inline uint32_t time(void)
{
    uint32_t cur_time;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        cur_time = system_time;
    }
    return cur_time;
}


static inline void init_rfid_reader(void)
{
    MFRC522_init();
    PCD_Init();
}


static inline void init_hw (void)
{
    // IO init
    // Set arduino pin 25 as output
    LED_INIT;

    // Set Arduino pin 23 as output
    DOOR_INIT;

    // System clock
    init_system_clock();

    // UART init
    uart0_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;

    // LCD init
    lcd_init();
    lcd_clrscr();

    // Init RFID-RC522
    init_rfid_reader();

    // Enable interupts
    sei();
}

static inline void start_ui (void)
{
    print_version(stderr);

    // print student name
    fprintf_P(stdout, PSTR(STUD_NAME));
    fputc('\n', stdout); // Add a new line to the uart printout
    lcd_puts_P(PSTR(STUD_NAME));
}

static inline void start_cli(void)
{
    // Call init with ptr to microrl instance and print callback
    microrl_init (prl, cli_print);
    // Set callback for execute
    microrl_set_execute_callback (prl, cli_execute);
}

static inline void heartbeat (void)
{
    static uint32_t time_prev;
    uint32_t time_cur = time();
    if (time_cur <= time_prev) {
        return;
    }
    time_prev = time_cur;
    fprintf_P(stderr, PSTR(UPTIME_MSG "\n"), time_cur);
    LED_TOGGLE;
}

static inline void handle_door()
{
    Uid uid;
    card_t card;
    uint32_t time_cur = time();
    static uint32_t message_start;
    static uint32_t door_open_start;
    if (PICC_IsNewCardPresent()) {
        PICC_ReadCardSerial(&uid);
        card.uid_size = uid.size;
        memcpy(&card.uid, &uid.uidByte, uid.size);
        card.user = NULL;
        card_t *found_card = rfid_find_card(&card);
        if (found_card) {
            lcd_goto(0x40);
            lcd_puts(found_card->user);
            for (int8_t i = 16 - strlen(found_card->user); i > -1; i--) {
                lcd_putc(' ');
            }
            DOOR_OPEN;
        } else {
            DOOR_CLOSE;
            lcd_goto(0x40);
            lcd_puts_P(access_denied_msg);
            for (int8_t i = 16 - strlen_P(access_denied_msg); i > -1; i--) {
                lcd_putc(' ');
            }
        }
        door_open_start = time_cur;
        message_start = time_cur;
    }

    if ((message_start + 5) < time_cur) {
        message_start = time_cur + 5;
        lcd_goto(0x40);
        for (int8_t i = 16; i > -1; i--) {
            lcd_putc(' ');
        }
    }

    if ((door_open_start + 2) < time_cur) {
        DOOR_CLOSE;
    }
}

int main (void)
{
    init_hw();
    start_ui();
    start_cli();

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char (prl, cli_get_char());
        handle_door();
    }
}


// System clock
ISR(TIMER5_COMPA_vect)
{
    system_time++;
}
