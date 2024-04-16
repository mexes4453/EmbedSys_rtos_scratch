
/* ############################** t.c file : c2.6 ########################### */
/*                                                                            */
/* Date    : 05.08.2023                                                       */
/* purpose : Program execution                                                */
/*                                                                            */
/* ########################################################################## */


//#include "defines.h"
#include "vid.c"	// DRIVER LCD
#include "uart.h"	// DRIVER UART

extern char _binary_img1_bmp_start, _binary_img2_bmp_start;
extern int volatile *fb;

#define WIDTH 640

int show_bmp( char *p, int start_row, int start_col )
{
	int 		h, w, pixel, rsize, i, j;
	unsigned char	r, g, b;
	char		*pp;

	int		*q = (int *)p + 14;	// skip first 14 bytes - file header
	w = *(q + 1);				// retrieve width in pixels
	h = *(q + 2);				// retrieve height in pixels
	p += 54;				// offset img pointer to image data location

	/* BMP img are stored upside down and each row is a multiple of 4 bytes */
	/* Steps to ensure that the row size is a multiple of 4                 */
	/*
	 * 01. desired multiple (n = 4)
	 * 02. add n-1 (4-1 = 3) to the number. Increase nbr enough for clipping
	 * 03. divide result by n. This discard the remainder -> clipped.
	 * 04. multiply result by n. Result is now raised to a multiple of n (4)
	 */  
	rsize = 4 * (( 3 * w + 3)/ 4);		// ensure size is multiple of 4
	
	p += (h - 1) * rsize;			// offset to last row of pix within image data

	for ( i=start_row; i<(start_row + h); i++)
	{
		pp = p;
		for ( j=start_col; j<(start_col + w); j++)
		{
			b = *pp; g = *(pp + 1); r = *(pp + 2);	// blue, green, red values
			pixel = ((b<<16) | (g<<8) | r);		// pixel value
			fb[ i*WIDTH + j ] = pixel;		// write to frame buffer
			pp += 3;				// advance to next pixel
		}
		p -= rsize;					// to preceding row
	}
	uprintf("\nBMP image height=%d width=%d\n", h, w);
}

extern UART uart[4]; // already defined in uart


int main(void)
{
	char c, *p;
	uart_init();		// initialize uarts
	fbuf_init();		// default to VGA mode
	UART *up = &uart[0];	// use uart 0
	int nbr = 30;
	uprintf("This is the number %d\n", nbr);

	while (1)
	{
		// display image 1
		p = &_binary_img1_bmp_start;
		show_bmp(p, 0, 30);
					
		// Prompt user to make input
		uprintf("Enter a key from this UART: ");	

		// Capture input from user
		ugetc(up);


		// display image 2
		p = &_binary_img2_bmp_start;
		show_bmp(p, 30, 0);	
	}

	while (1){}			// loop forever

}
