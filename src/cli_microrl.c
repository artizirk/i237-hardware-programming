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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "rfid.h"


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
    {month_cmd, month_help, cli_handle_month, 1},
    {read_cmd, read_help, cli_rfid_read, 0},
    {add_cmd, add_help, cli_rfid_add, 1},
    {remove_cmd, remove_help, cli_rfid_remove, 1},
    {list_cmd, list_help, cli_rfid_list, 0}
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
    print_version(stdout);
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
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


void cli_rfid_read(const char *const *argv)
{
    (void) argv;
    Uid uid;
    Uid *uid_ptr = &uid;
    if (PICC_IsNewCardPresent()) {
        printf(CARD_SELECTED_MSG "\n");
        PICC_ReadCardSerial(uid_ptr);
        printf_P(PSTR(UID_SIZE_MSG "\n"), uid.size);
        printf_P(PSTR(UID_SAK_MSG"\n"), uid.sak);
        printf_P(PSTR(CARD_UID_MSG));
        for (byte i = 0; i < uid.size; i++) {
            printf("%02X", uid.uidByte[i]);
        }
        putc('\n', stdout);
    } else {
        printf_P((PSTR(CARD_NOT_SELECTED)));
    }
}


void cli_rfid_add(const char *const *argv)
{
    (void) argv;
    Uid uid;
    card_t card;
    if (PICC_IsNewCardPresent()) {
        PICC_ReadCardSerial(&uid);
        card.uid_size = uid.size;
        memcpy(&card.uid, &uid.uidByte, uid.size);
        char *user = malloc(strlen(argv[1]) + 1);
        strcpy(user, argv[1]);
        card.user = user;
        rfid_add_card(&card);
        free(user); // card user has ben copied to the linked list
    } else {
        printf_P(PSTR(UNABLE_TO_DETECT_CARD_MSG "\n"));
    }
}


void cli_rfid_remove(const char *const *argv)
{
    (void) argv;
    rfid_remove_card_by_user(argv[1]);
}


void cli_rfid_list(const char *const *argv)
{
    (void) argv;
    rfid_list_cards();
}


void cli_print_cmd_error(void)
{
    printf_P(PSTR(CLI_NO_CMD "\n"));
}


void cli_print_cmd_arg_error(void)
{
    printf_P(PSTR(CLI_ARGS_MSG "\n"));
}


int cli_execute(int argc, const char *const *argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            putc('\n', stdout);
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
    putc('\n', stdout);
    cli_print_cmd_error();
    return 0;
}
