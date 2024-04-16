/* ############################ vid.c file : c2.7 ########################### */
/*                                                                            */
/* Date    : 05.08.2023                                                       */
/* purpose : Configure LCD driver for text 				      */
/*                                                                            */
/* ########################################################################## */
/*
 *
 * +-------+--------------+----------+-------------+-------------+------------+
 * |  Mode |  Resolution  |   OSC1   |   TimeReg0  |   TimeReg1  |  timeReg2  |
 * +-------+--------------+----------+-------------+-------------+------------+
 * |  VGA  |  640x480     |  0x02C77 |  0x3F1F3F9C |  0x090B61DF | 0x067F1800 |
 * |  SVGA |  800x600     |  0x02CAC |  0x1313A4C4 |  0x0505F6F7 | 0x071F1800 |
 * +-------+--------------+----------+-------------+-------------+------------+
 * 00 timing0
 * 04 timing1
 * 08 timing2
 * 0C timing3
 * 10 upperPanelFrameBaseAddressRegister // use the upper panel
 * 14 lowerPanelFrameBaseAddressRegister // not use lower panel
 * 18 controlRegister // NOTE: QEMU emulated PL110 CR is at 0x18
 *
 * note
 * ----
 * The font ﬁle, font.bin,
  is a raw bitmap of 128 ASCII chars, in which each char is represented by a 8x16 bitmap
  i.e. each char is represented by 16 bytes,
  each byte speciﬁes the pixels of a scan line of the char.

  To display a char, use the char's ASCII code value (multiplied by 16) as an offset
  oto access its bytes in the bitmap.

  Then scan the bits in each byte. For each bit with 0, write BGR = 0x000000 (black) to the
  corresponding pixel. For each bit with 1, write BRG = 0x111111 (white) to the pixel.

  Instead of black and white, each char can also be displayed in color by writing different 
  RGB values to the pixels.

 */


#include <stdint.h>

#define REG_OSC1      (*((unsigned int volatile *)0x1000001C))
#define REG_TIME_REG0 (*((volatile unsigned int *)0x10120000))
#define REG_TIME_REG1 (*((volatile unsigned int *)0x10120004))
#define REG_TIME_REG2 (*((volatile unsigned int *)0x10120008))
#define REG_BUF_ADDR  (*((volatile unsigned int *)0x10120010))

#define RED 	0
#define BLUE 	1
#define GREEN 	2
#define WHITE 	3

extern char	_binary_font_start;	// created by linker
int		color;
uint8_t		cursor;
int 		row, col, scroll_row;
unsigned char 	*font;
int volatile 	*fb;

char *tab = "0123456789ABCDEF";	// used for int to hex conversion
int WIDTH = 640;		// default with of VGA mode for 640 X 480




int fbuf_init(void)
{
	int i;
	fb = (int volatile *)0x200000;		// reserve from 2MB to 4MB
	font = &_binary_font_start;	// font bitmap 
	
	if (WIDTH == 640)
	{
		/* Configuration for mode 640x480 */
		REG_OSC1      = 0x02C77;
		REG_TIME_REG0 = 0x3F1F3F9C;
		REG_TIME_REG1 = 0x090B61DF; 
		REG_TIME_REG2 = 0x067F1800;
	}
	else
	{
		/* Configuration for mode 800x600 */
		REG_OSC1      = 0x02CAC;
		REG_TIME_REG0 = 0x1313A4C4; 
		REG_TIME_REG1 = 0x0505F6F7; 
		REG_TIME_REG2 = 0x071F1800; 
	}

	/* set start of buffer address to lcd config register for buffer */
	REG_BUF_ADDR = 0x200000;
	*(volatile unsigned int *)(0x10120018) = 0x82B;
	cursor = 127; // cursor = row 127 in font bitmap
}


/**
 * @brief clear pixel at (x, y)
 * @detail sets all the color bytes (RGB) to 0
 *
 * @param y represent row index
 * @param x represent col index
 */
int	clrpix(int x, int y)	
{
	int pix = (y * 640) + x;
	fb[pix] = 0x00000000;
}



/**
 * @brief set pixel at (x, y)
 * @detail sets all the color bytes (RGB) to either
 *  red, green, or blue depending the value of the
 *  setting - color variable.
 *
 * @param y represent row index
 * @param x represent col index
 */
int	setpix(int x, int y)
{
	int pix = (y * 640) + x;
	if (color==RED)
		fb[pix] = 0x000000FF;
	if (color==BLUE)
		fb[pix] = 0x00FF0000;
	if (color==GREEN)
		fb[pix] = 0x0000FF00;
}


/**
* @brief display char at (x, y)
* @detail sets all the color bytes (RGB) to either
*  red, green, or blue depending the value of the
*  setting - color variable.
*
* @param c character to display
* @param y represent row index
* @param x represent col index
* @note see note at top of file. for font accessing
*/
int	dispChar( unsigned char c, int x, int y) 
{
	int		r, bit;
	unsigned char	*caddr, byte;
	caddr = font + c*16;		// access/get the char font addr
	
	for (r=0; r<16; r++)		// each char is represent by 8 x 16 (col x row)
	{				// 16 bytes (each byte(ROW) has 8 bits(COL))
		byte = *(caddr + r);	// iterate through each of 16 bytes
		for (bit=0; bit<8; bit++)		// iterate through each bit in each byte
		{
			if (byte & (1<<bit))		// check each bit value
			{
				setpix(x+bit, y+r);	// set each bit accordingly
			}
		}
	}
}


/**
* @brief display char at (x, y)
* @detail sets all the color bytes (RGB) to 0
*
* @param c character to display
* @param y represent row index
* @param x represent col index
* @note see note at top of file. for font accessing
*/
int	delChar( unsigned char c, int x, int y)
{
	int row, bit;
	unsigned char *caddr, byte;
	caddr = font + c*16;

	for (row=0; row<16; row++)
	{
		byte = *(caddr + row);
		for (bit=0; bit<8; bit++)
		{
			if (byte & (1<<bit))
				clrpix(x+bit, y+row);
		}
	}
}

/**
 *
 * scroll up by one line (the hard way)
 *
 */
int scroll(void)
{
	int	i;
	for (i=64*640; i<640*480; i++)
		fb[i] = fb[i + 640*16];
}



/**
 *
 * print char at (row, col)
 */
int kpchar(char c, int ro, int co) 
{

	int x,y;
	x = co * 8;
	y = ro * 16;
	dispChar(c, x, y);
}


/**
 *
 * erase char at (row, col)
 */
int unkpchar(char c, int ro, int co) 
{

	int x,y;
	x = co * 8;
	y = ro * 16;
	delChar(c, x, y);
}


/**
 *
 * erase the char at (row, col)
 * These are global variable
 */
int erasechar(void)
{
	int r, bit, x, y;
	unsigned char *caddr, byte;
	x = col * 8;
	y = row * 16;
	for (r=0; r<16; r++)
	{
		for (bit=0; bit<8; bit++)
		{
			clrpix(x+bit, y+r);
		}
	}

}


/**
 *
 * clear the cursor at (row, col)
 * These are global variable
 */
int clrcursor(void)
{
	unkpchar(127, row, col);
}


int putcursor(unsigned char c) // set cursor at (row,col)
{
	kpchar(c, row, col);
}


/**
 *
 *  print char at cursor position
 *  using the global row, col variable
 *  cursor is a global variable that store the cursor char
 */
int kputc(char c)
{
	clrcursor();
	if (c == '\r')	// return key
	{
		col = 0;
		putcursor(cursor);
		goto end;
	}
	if (c == '\n')	// new line
	{
		row++;
		if ( row >= 25 )	
		{
			row = 24;
			scroll();
		}	
		putcursor(cursor);
		goto end;
	}
	if (c == '\b') 	// back space key
	{
		if (col > 0)
		{
			col--;
			erasechar();
			putcursor(cursor);
		}
		goto end;
	}
	kpchar(c, row, col);
	col++;
	if (col >= 80)
	{
		col = 0;
		row++;
		if ( row >= 25 )
		{
			row = 24;
			scroll();
		}
	}
	putcursor(cursor);
end:
	return (0);

}




/* The following implements the formatted printing */

int kprints(char *s)
{
	while (*s)
	{
		kputc(*s);
		s++;
	}
}


/**
 *
 * display int in hex format 
 * tab for mapping hex remainder should exist
 *
 */
int krpx(int x)
{
	char c;
	if (x)
	{
		c = tab[x % 16];
		krpx(x / 16);
	}
	kputc(c);
}




int kprintx(int x)
{
	kputc('0');
	kputc('x');
	(x == 0) ? kputc('0') : krpx(x);
	kputc(' ');
}




/**
 *
 * display int in dec format 
 * tab for mapping dec remainder should exist
 *
 */
int krpu(int x)
{
	char c;
	if (x)
	{
		c = tab[x % 10];
		krpx(x / 10);
	}
	kputc(c);
}




int kprintu(int x)
{
	(x == 0) ? kputc('0') : krpu(x);
	kputc(' ');
}


int kprinti(int x)
{
	if (x<0)
	{
		kputc('-');
		x = -x;
	}
	kprintu(x);
}


int kprintf(char *fmt, ...)
{
	int 	*ip;
	char	*cp;

	cp = fmt;
	ip = (int *) &fmt + 1;

	while (*cp)
	{
		if (*cp != '%')
		{
			kputc(*cp);
			if (*cp== '\n')
			{
				kputc('\r');
				cp++;
				continue ;
			}
		}
		cp++;
		switch(*cp)
		{
			case 'c': kputc((char) *ip); 	break ;
			case 's': kprints((char *)*ip); break ;
			case 'd': kprinti(*ip);		break ;
			case 'u': kprintu(*ip);		break ;
			case 'x': kprintx(*ip);		break ;
		}
		cp++; ip++;
	}
}



