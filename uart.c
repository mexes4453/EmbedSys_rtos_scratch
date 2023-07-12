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
    char    *base;  // device base address
    int     n;      // uart number 0-3
}           UART;

UART uart[4];       // 4 uart struct



int    uart_init(void)  // uart initialization function
{
    int     i;
    UART    *up;

    for (i=0; i<4; i++)
    {
        up = &uart[i];
        up->base = (char *)(0x101F1000) + (i * 0x1000);
        up->n = i;
    }
    uart[3].base = (char *)(0x10009000);    // uart 3 is at 0x10009000
    return (0);
}



int    ugetc(UART *up)                     // input a char from UART pointed to by up
{
    while (*(up->base + UFR) & RXFE);      // loop if UFR is RXFE (polling)
    return (*(up->base + UDR));            // return data from data register
}


int    uputc(UART *up, char c)              // Output a char to UART pointed to by up
{
    while ((*(up->base + UFR)) & TXFF);     // loop if UFR is TXFF (polling)
    *(up->base + UDR) = c;
    return (0);
}


int    ugets(UART *up, char *s)             // Input a string of characters
{
    while ((*s = ugetc(up)) != '\r')
    {
        uputc(up, *s);
        s++;
    }
    *s = 0;
    return (0);
}


int    uputs(UART *up, char *s)             // Output a string to UART pointed to by up
{
    while (*s)
    {
        uputc(up, *s);
        s++;
    }
}

// build, flash and execute command
// qmu-system-arm -M versatilepb -m 128M -kernel t.bin -serial mon:studio