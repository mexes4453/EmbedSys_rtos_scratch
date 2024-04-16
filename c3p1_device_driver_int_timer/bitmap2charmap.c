/**
 * converts bitmap of characters to map of characters
 * input : font.bin (bitmap image loaded as binary file)
 *
 *
 * execution:
 * ---------
 *  $ a.out font.bin > font.h
 *
 */

#include <stdio.h>

main( int argc, char *argv[] )
{
	int i, n; u8 buf[16];
	FILE *fp = fopen(argv[1], "r"); // fopen binary file for read
	
	while ( (n = fread( buf, 1, 16, fp ) )	// read 16 bytes
	{
		for ( i=0; i<n; i++ )		// write each byte as 2 hex
			printf("0x%2x ", buf[i]);
	}
	printf("\n");
}

