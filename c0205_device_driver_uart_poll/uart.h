#ifndef UART_H
#define UART_H



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





#endif    /* UART  */
