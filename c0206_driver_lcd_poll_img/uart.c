

#include "uart.h"
#include <stddef.h>

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



/* ======= PRINTF =========================================================*/

static int	ft_printf_nbr_base(UART *up, unsigned int n, size_t base, int c)
{
	char	*ptr;
	int     len;
	char	nbr_ch;


	len = 0;
	ptr = "0123456789abcdef";
	if (n >= base)
	{
//		len += ft_printf_nbr_base(up, (n / base), base, c);
	}

//	nbr_ch = ptr[n % base];
	if (c == 0 && (nbr_ch >= 97 && nbr_ch <= 102))
	{
		uputc(up, nbr_ch);
	}
	else if (c && (nbr_ch >= 97 && nbr_ch <= 102))
	{
		nbr_ch -= 32;
		uputc(up, nbr_ch);
	}
	else
		uputc(up, nbr_ch);
	len++;
	return (len);
}



static int	ft_printf_u(UART *up, unsigned int d)
{
	int	len;

	len = 0;
	len += ft_printf_nbr_base(up, d, 10, 0);
	return (len);
}



int    uprintf(char *fmt, ...)
{
	int    *ip;
	char    *cp;
	UART	*up = &uart[0];

	cp = fmt;
	ip = (int *) &fmt + 1;		// points to first args in ...

	while (*cp)
	{
		if (*cp != '%')
		{
			uputc(up, *cp);

			/* return carriage on newline */
			if (*cp == '\n')
			{
				uputc(up, '\r');
			}
			cp++;
			continue;
		}
		/* Handle format characters */
		cp++;
		switch(*cp)
		{
			case 'c': uputc(up, (char) *ip); break ;
			case 's': uputs(up, (char *) ip); break ;
			case 'd': ft_printf_u(up, (int)*ip); break ;
		}
		cp++; ip++;
	}
}

