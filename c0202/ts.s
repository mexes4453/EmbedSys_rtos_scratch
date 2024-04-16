.text
.global start

start:  ldr sp, =stack_top /* set stack pointer     */
        bl sum             /* call function sum     +/
                           * branch to label 'sum' */
stop:   b stop             /* looping forever       */



sum:    /* int sum(): compute the sum of an int array in Result           */
        /* stmfd sp!, {r0-r4, lr}      /+ save r0-r4, lr on stack         */
        push {r0-r4, lr}               /* save r0-r4, lr on stack         */
        mov r0, #0                     /* r0 = 0; */
        ldr r1, =Array                 /* store array address in r1 (r1 = &Array;)   */
        ldr r2, =N                     /* store N address in r2; (r2=&N)  */
        ldr r2, [r2]                   /* r2 = N (deferencing &N)         */
loop:   ldr r3, [r1], #4               /* r3 = *(r1++); inc addr by 4bytes*/      
        add r0, r0, r3                 /* r0 += r3                        */
        sub r2, r2, #1                 /* r2--; reduce nbr of int by 1    */
        cmp r2, #0                     /* if (r2 != 0) : check if r2==0   */
        bne loop                       /* goto loop; jump to loop label   */
        ldr r4, =Result                /* r4 = &Result; ptr to result     */
        str r0, [r4]                   /* Result = r0; store ans to result*/
        /* ldmfd sp!, {r0-r4, pc}      /+ pop stack, return to caller func*/
        pop {r0-r4, pc}                /* pop stack, return to caller func*/


.data    /* data section */
        N:       .word 10   /* number of array elements (int) */
        Result:  .word 0
        Array:   .word 1,2,3,4,5,6,7,8,9,10

