/* ############################ ts.s file : c3.1 ############################ */
// Page 58(74)
.text
.code 32
.global reset_handler, vectors_start, vectors_end

reset_handler:
	LDR sp, =svc_stack_top		// Set SVC mode stack
	BL copy_vectors			// copy vector table to address 0 : defined in C
	MSR cpsr, #0x92			// to IRQ mode
	LDR sp, =irq_stack_top		// set IRQ mode stack
	MSR cpsr, #0x13			// Go back to SVC mode with IRQ on
	BL main				// Call main() in C
	B .				// loop forever if main ever return

irq_handler:
	sub lr, lr, #4
	stmfd sp!, {r0-r12, lr}		// stack r0-r12 and link register
	bl IRQ_handler			// call IRQ_handler defined in C
	ldmfd sp!, {r0-r12, pc}^	// return

vectors_start:
	LDR PC, reset_handler_addr
	LDR PC, undefined_handler_addr
	LDR PC, swi_handler_addr
	LDR PC, prefetch_abort_handler_addr
	LDR PC, data_abort_handler_addr
	B .				// loop forever - reserved word
	LDR PC, irq_handler_addr
	LDR PC, fiq_handler_addr

	reset_handler_addr:		.word reset_handler
	undef_handler_addr:		.word undef_handler
	swi_handler_addr:		.word reset_handler
	prefetch_abort_handler_addr:	.word reset_handler
	data_abort_handler_addr:	.word reset_handler
	irq_handler_addr:		.word reset_handler
	fiq_handler_addr:		.word reset_handler
vectors_end:
/* ###################          END OF FILE      ############################ */
