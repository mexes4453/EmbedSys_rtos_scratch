	.global start, stack_top	// stack_top is defined in link.ld (linker script)

start:
	ldr sp, =stack_top             // set SVC mode stack pointer
	bl main				// call main() in .c file
	b .				// if main() returns, then loop forever

