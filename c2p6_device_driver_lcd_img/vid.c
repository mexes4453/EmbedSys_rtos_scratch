/* ############################ vid.c file : c2.6 ########################### */
/*                                                                            */
/* Date    : 05.08.2023                                                       */
/* purpose : Configure LCD driver for image 				      */
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
 */


#define REG_OSC1      (*((unsigned int volatile *)0x1000001C))
#define REG_TIME_REG0 (*((volatile unsigned int *)0x10120000))
#define REG_TIME_REG1 (*((volatile unsigned int *)0x10120004))
#define REG_TIME_REG2 (*((volatile unsigned int *)0x10120008))
#define REG_BUF_ADDR  (*((volatile unsigned int *)0x10120010))

static int volatile *fb;
//int WIDTH = 640;	// default with of VGA mode for 640 X 480

int fbuf_init(void)
{
	fb = (int  volatile *)0x200000;		// reserve from 2MB to 4MB
	
	/* Configuration for mode 640x480 */
	REG_OSC1      = 0x02C77;
	REG_TIME_REG0 = 0x3F1F3F9C;
	REG_TIME_REG1 = 0x090B61DF; 
	REG_TIME_REG2 = 0x067F1800;

	/* Configuration for mode 800x600 */
	REG_OSC1      = 0x02CAC;
	REG_TIME_REG0 = 0x1313A4C4; 
	REG_TIME_REG1 = 0x0505F6F7; 
	REG_TIME_REG2 = 0x071F1800; 

	/* set start of buffer address to lcd config register for buffer */
	REG_BUF_ADDR = 0x200000;
	*(volatile unsigned int *)(0x10120018) = 0x82B;
}
