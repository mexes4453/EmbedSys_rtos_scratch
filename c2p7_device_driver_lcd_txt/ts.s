/* ############################ ts.s file : c2.6 ############################ */

	.global reset_start, stack_top

reset_start:
	ldr sp, =stack_top		// Set up stack pointer in SVC mode
	bl main				// call main() function
	b .				// loop forever

/* ###################          END OF FILE      ############################ */
