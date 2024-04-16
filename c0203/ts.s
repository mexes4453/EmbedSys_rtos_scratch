/* ts.s file of program c2.3 */
	.text
	.global start, sum
start:	ldr sp, =stack_top
	bl main                 /* call main() in c program */
stop:	b stop

sum:	// int sum(int a, b, c, d, e, f) {return a+b+c+d+e+f} 
/*	upon entry, stack_top contains e, f, passed by main() in c */
/*	Establish stack frame */
	stmfd sp!, {fp, lr}	/* push fp, lr to stack   */
	add fp, sp, #4          /* fp-> saved lr on stack */
/*	compute sum of all (6) parameters                 */
	add r0, r0, r1          /* first 4 parameters are in r0-r3 */
	add r0, r0, r2
	add r0, r0, r3
	ldr r3, [fp, #4]        /* load e into r3         */
	add r0, r0, r3          /* add to sum in r0       */
	ldr r3, [fp, #8]        /* load f into r3         */
	add r0, r0, r3          /* add to sum in r0       */
/*      Return to caller                                  */
	sub sp, fp, #4          /* sp=fp-4 (point at saved FP) */
	ldmfd sp!, {fp, pc}     /* return to caller            */

