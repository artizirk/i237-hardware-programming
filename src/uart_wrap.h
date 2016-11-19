#ifndef _UART_WRAP_H_
#define _UART_WRAP_H_

int uart0_putc_wrap(char c, FILE *stream);
int uart0_getc_wrap(FILE *stream);

int uart3_putc_wrap(char c, FILE *stream);


/* http://www.ermicro.com/blog/?p=325 */

FILE uart0_io = FDEV_SETUP_STREAM(uart0_putc_wrap, uart0_getc_wrap, _FDEV_SETUP_RW);
FILE uart3_out = FDEV_SETUP_STREAM(uart3_putc_wrap, NULL, _FDEV_SETUP_WRITE);

#endif /* _UART_WRAP_H_ */
