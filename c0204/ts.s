/* ts.s file of program c2.4 */
	.text
	.global start, sum
start:	ldr sp, =stack_top
/* Prepare r0 and r1 to store value to be passed in as argument for */
/* call to the sum function defined in c file                       */
        ldr r2, =a
	ldr r0, [r2] 		/*   r0=a                           */
	ldr r2, =b
	ldr r1, [r2] 		/*   r1=b                           */
	bl sum			/*   call sum func: c = sum(a,b)    */
	ldr r2, =c
	str r0, [r2]            /*   store return value in c        */
stop:	b stop


.data
a:	.word 1
b:	.word 2
c:	.word 0

