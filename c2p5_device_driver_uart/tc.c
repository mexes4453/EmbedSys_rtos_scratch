/* **********************************************************************
 *                                                                      *
 * main.c                                                               *
 * uart testing                                                         *
 * c program initialises UART driver and use it for UART I/O operations *
 *                                                                      *
 *                                                                      *
 * *********************************************************************/


int v[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int sum;

#include "string.c" 		// contains strlen and strcmp
#include "uart.c"		// UART driver code file

int main()
{
	int	i;
	char	string[64];
	UART	*up;

	uart_init();		// Initialise UARTs
	up = &uart[0];		// test uart[0]
	uprints(up, "Enter  lines from serial terminal 0\n\r");
	while (1)
	{
		ugets(up, string);
		uprints(up, "    ");
		uprints(up, string);
		uprints(up, "\n\r");
		if (strcmp(string, "end") == 0)
		{
			break ;
		}
	}
	uprints(up, "Compute sum of array:\n\r");
	sum = 0;
	for (i=0; i<10; i++)
		sum += v[i];			// sum is 55
	uprints(up, "sum = ");
	uputc(up, (sum/10)+'0');		// prints the first 5 after conversion from int to char
	uputc(up, (sum%10)+'0');		// prints the second 5	after conversion from int to char
	uprints(up, "\n\rEND OF RUN\n\r");
	return (0);
}
