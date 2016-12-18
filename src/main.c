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


#define BAUDRATE 9600

// For configuring arduino mega pin 25
#define LED_INIT DDRA |= _BV(DDA3);
#define LED_TOGGLE PORTA ^= _BV(PORTA3)
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
    /// Set arduino pin 25 as output
    LED_INIT;

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


int main (void)
{
    init_hw();
    start_ui();
    start_cli();

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char (prl, cli_get_char());
    }
}


// System clock
ISR(TIMER5_COMPA_vect)
{
    system_time++;
}
