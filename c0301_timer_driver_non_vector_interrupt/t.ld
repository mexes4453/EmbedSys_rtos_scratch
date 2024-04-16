
ENTRY(reset_handler)

SECTIONS
{
	. = 0x10000;      /* Loading address */
	.text : { ts.o *(.text) }
	.data : { *(.data) }
	.bss  : { *(.bss) }
	. = ALIGN(8);
	. = . + 0x1000;             /* 4kb of SVC stack space */
	svc_stack_top = .;  
	. = . + 0x1000;             /* 4kb of IRQ stack space */
	irq_stack_top = .;  
}

