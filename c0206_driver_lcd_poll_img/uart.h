#ifndef UART_H
#define UART_H


/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ #
 * UART DRIVER - ARM926EF-S Board                                 #  
 *                                                                #
 *  |bits |  7   |  6   |  5   |  4   |  3   |  2  |  1  |  0  |  #
 *  |desc | TXFE | RXFF | TXFF | RXFE | BUSY |  -  |  -  |  -  |  #
 *                                                                #
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

#define TXFE 0x80   // Flag register bit - transmit empty
#define RXFF 0x40   // Flag register bit - receive full
#define TXFF 0x20   // Flag register bit - transmit full
#define RXFE 0x10   // Flag register bit - receive empty
#define BUSY 0x08   // Flag register bit - busy

#define UDR 0x00    // uart data register address
#define UFR 0x18    // uart flag register address

typedef volatile struct uart
{
    char    *base;                        // device base address
    int     n;                            // uart number 0-3
}           UART;

int    uart_init(void);                   // uart initialization function
int    ugetc(UART *up);                   // input a char from UART pointed to by up
int    uputc(UART *up, char c);           // Output a char to UART pointed to by up
int    ugets(UART *up, char *s);          // Input a string of characters
int    uputs(UART *up, char *s);          // Output a string to UART pointed to by up
int    uprintf(char *fmt, ...);





#endif    /* UART  */
