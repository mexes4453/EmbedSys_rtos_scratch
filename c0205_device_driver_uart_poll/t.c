#include "uart.h"
#include <stddef.h>

extern UART uart[4];
int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int sum;

int	strncmp(const char *s1, const char *s2, size_t n);

int main()
{
	int i;
	char string[64];
	UART *up;
	uart_init();			// initialize uart
	up = &uart[0];			// using first uart on board
	uputs(up, "Enter lines from serial terminal 0\n\r");
	while (1)
	{
		ugets(up, string);
		uputs(up, "  ");
		uputs(up, "\n\r");
		if (strncmp(string, "end", (unsigned int)3) == 0)
			break;
	}
	
	uputs(up, "Compute sum of array\n\r");
	sum = 0;
	for (i=0; i<10; i++)
	{
		sum += v[i];				// equals 55
	}
	uputs(up, "sum = ");			// screen output "sum = "
	uputc(up, (sum/10)+0x30);		// screen output "sum = 5"
	uputc(up, (sum%10)+0x30);		// screen output "sum = 55"
	uputs(up, "\n\rEND OF RUN\n\r");

}



int	strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	unsigned char	chr1;
	unsigned char	chr2;

	i = 0;
	while (i < n)
	{
		chr1 = (unsigned char)(*(s1 + i));
		chr2 = (unsigned char)(*(s2 + i));
		if ((chr1 - chr2) != 0)
			return (chr1 - chr2);
		i++;
	}
	return (0);
}
