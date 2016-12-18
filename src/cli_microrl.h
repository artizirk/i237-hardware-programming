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
#ifndef _CLI_MICRORL_H_
#define _CLI_MICRORL_H_

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))
#define UART_STATUS_MASK    0x00FF


void cli_print(const char * str);
char cli_get_char(void);
void cli_print_help(const char *const *argv);
void print_version(FILE *stream);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_rfid_read(const char *const *argv);
void cli_rfid_add(const char *const *argv);
void cli_rfid_remove(const char *const *argv);
void cli_rfid_list(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
int cli_execute(int argc, const char *const *argv);


#endif /* _CLI_MICRORL_H_ */
