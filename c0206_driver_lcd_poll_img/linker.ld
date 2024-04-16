
ENTRY(reset_start)		/* define start as the entry address */
SECTIONS			/* program sections */
{
	. = 0x10000;	/*loading addressm required by QEMU */
	.text : { *(.text) } /* all text in .text section */
	.data : { *(.data) } /* all data in .data section */
	.bss  : { *(.bss) }  /* all bss in .bss section */
	.data : { *(img1.o) } /* include img.o as a data section */
	.data : { *(img2.o) } /* include img.o as a data section */
	. =ALIGN(8);
	. =. + 0x1000;		/* 4 kb stack space */
	stack_top =.;		/* stack_top is a symbol exported by linker */
}
