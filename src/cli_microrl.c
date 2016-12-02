#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1}
};


void cli_print(const char *str)
{
    printf("%s", str);
}


char cli_get_char(void)
{
    if (uart0_peek() != UART_NO_DATA) {
        return uart0_getc() & UART_STATUS_MASK;
    } else {
        return 0x00;
    }
}


void cli_print_help(const char *const *argv)
{
    (void) argv;
    putc('\n', stdout);
    printf_P(PSTR(CLI_HELP_MSG "\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        printf_P(cli_cmds[i].cmd);
        printf_P(PSTR(" : "));
        printf_P(cli_cmds[i].help);
        putc('\n', stdout);
    }
}


void print_version(FILE *stream)
{
    // Print program and libc versions
    fprintf_P(stream, PSTR(PROG_VERSION "\n"),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stream, PSTR(LIBC_VERSION "\n"),
              PSTR(__AVR_LIBC_VERSION_STRING__),
              PSTR(__VERSION__));
}


void cli_print_ver(const char *const *argv)
{
    (void) argv;
    putc('\n', stdout);
    print_version(stdout);
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    putc('\n', stdout);
    // ASCII table print
    print_ascii_tbl(stdout);
    unsigned char ascii[128];
    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }
    print_for_human(stdout, ascii, sizeof(ascii));
}


void cli_handle_month(const char *const *argv)
{
    putc('\n', stdout);
    lcd_goto(0x40); // Got to the beginning of the next line
    char spaces_to_print = 16;
    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(argv[1],
                       (PGM_P)pgm_read_word(&months[i]),
                       strlen(argv[1]))) {
            char printed_count;
            printed_count = fprintf_P(stdout, (PGM_P)pgm_read_word(&months[i]));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P)pgm_read_word(&months[i]));
            lcd_putc(' ');
            spaces_to_print -= (printed_count + 1);
        }
    }
    // Clear the end of the line
    for (; spaces_to_print > -1; spaces_to_print--) {
        lcd_putc(' ');
    }
}


void cli_print_cmd_error(void)
{
    putc('\n', stdout);
    printf_P(PSTR(CLI_NO_CMD "\n"));
}


void cli_print_cmd_arg_error(void)
{
    putc('\n', stdout);
    printf_P(PSTR(CLI_ARGS_MSG "\n"));
}


int cli_execute(int argc, const char *const *argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}
