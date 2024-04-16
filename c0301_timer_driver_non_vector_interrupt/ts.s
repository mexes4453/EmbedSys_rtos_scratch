
/* This file defines the entry point and reset code */


.text
.code 32
.global  reset_handler, vector_start, vector_end

/* By default, the cpu starts in SVC mode **/
reset_handler:
    LDR sp, =svc_stack_top   /* set SVC mode stack defined in linker script */
    BL copy_vectors          /* copy vector table to address 0 */
    MSR cpsr, #0x92          /* switch cpu to IRQ mode */
    LDR sp, =irq_stack_top   /* set IRQ mode stack defined in linker script */
    MSR cpsr, #0x13          /* switch cpu back to SVC mode */
    BL main                  /* call main() in c file  */
    B .                      /* loop if main ever return */

irq_handler:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}  /* push r0-r12 and lr onto the stack */
    bl IRQ_handler           /* call IRQ_handler() in c           */
    ldmfd sp!, {r0-r12, pc}^ /* return                            */
    /* Note that the symbol "^" at the end of LDMFD instruction   */
    /* indicates that the CPSR will be restored from the saved SPSR */
    /* The symbol restores the saved SPSR only if the PC is loaded  */
    /* at the same time as done above, otherwise, only the bank registers */
    /* of the previous mode will be restored, excluding the SPSR          */
    

vector_start:
    LDR PC, reset_handler_addr
    LDR PC, undef_handler_addr
    LDR PC, swi_handler_addr
    LDR PC, prefetch_abort_handler_addr
    LDR PC, data_abort_handler_addr
    B .
    LDR PC, irq_handler_addr
    LDR PC, fiq_handler_addr
    reset_handler_addr:                     .word reset_handler
    undef_handler_addr:                     .word undef_handler
    swi_handler_addr:                       .word swi_handler
    prefetch_abort_handler_addr:            .word prefetch_abort_handler
    data_abort_handler_addr:                .word data_abort_handler
    irq_handler_addr:                       .word irq_handler
    fiq_handler_addr:                       .word fiq_handler
vector_end:

    
    
    

	
