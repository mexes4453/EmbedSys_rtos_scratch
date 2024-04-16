
ENTRY(reset_handler)		/* define start as the entry address */
SECTIONS			/* program sections */
{
	. = 0x10000;			/*loading address required by QEMU */
	.text : { ts.o *(.text) } 	/* all text in .text section */
	.data : { *(.data) }		/* all data in .data section */
	.bss : { *(.bss) } 		/* all bss in .bss section */
	.data : { *(img1.o) }		/* include img.o as a data section */
	. =ALIGN(8);
	. = . + 0x1000;			/* 4 kb of SVC stack space */
	svc_stack_top =.;		/* stack_top is a symbol exported by linker */
	. = . + 0x1000;			/* 4 kb of IRQ stack space */
      	irq_stack_top =.;		/* irq_stack_top is a symbol exported by linker */
}
