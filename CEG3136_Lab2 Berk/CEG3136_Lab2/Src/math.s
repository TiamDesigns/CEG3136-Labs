// Math routines for Calculator app
/*
.syntax unified
.cpu cortex-m33
.thumb

.section .text

// uint32_t Increment(uint32_t num);
.global Increment
.type Increment, %function
Increment:
	add r0, r0, #1
	bx lr

// uint32_t Decrement(uint32_t num);
.global Decrement
.type Decrement, %function
Decrement:
	sub r0, r0, #1
	bx lr

.global Average
.type Average, %function
Average :

.global Sort
.type Sort, %function
Sort :

.global Fibonacci
.type Fibonacci, %function
Fibonacci :
    cmp r0, #1
    bls fib_done

    push {r4, r5, r6, lr}
    mov r4, r0

    subs r0, r4, #1
    bl Fibonacci
    mov r5, r0

    subs r0, r4, #2
    bl Fibonacci

    adds r0, r0, r5
    pop {r4, r5, r6, pc}

fib_done:
    bx lr

.global Factorial
.type Factorial, %function
Factorial :
	MOV R1, #1
	CMP R0, #2
	BLS endf
	loopf : MUL R1, R0, R1
	SUBS R0, R0, #1
	BNE loopf
	MOV R0, r1
	endf : BX lr

.global GreatestCommonDivider
.type GreatestCommonDivider, %function
GreatestCommonDivider :
	loop : UDIV r2, r0, r1
	MLS r3, r1, r2, r0

	MOV r0, r1
	MOV r1, r3

	CMP r1, #0
	BNE loop

.global FourFunction
.type FourFunction, %function
 FourFunction :
*/

.syntax unified
.cpu cortex-m33
.thumb
.section .text

// uint32_t Increment(uint32_t n);
.global Increment
.type Increment, %function
.thumb_func
Increment:
    adds r0, r0, #1
    bx lr
.size Increment, .-Increment

// uint32_t Decrement(uint32_t n);
.global Decrement
.type Decrement, %function
.thumb_func
Decrement:
    subs r0, r0, #1
    bx lr
.size Decrement, .-Decrement

// uint32_t Calc4(uint32_t op, uint32_t a, uint32_t b);
// op: 0 add, 1 subtract, 2 multiply, 3 divide
.global Calc4
.type Calc4, %function
.thumb_func
Calc4:
    cmp r0, #0
    beq calc_add
    cmp r0, #1
    beq calc_sub
    cmp r0, #2
    beq calc_mul
    cmp r0, #3
    beq calc_div
    movs r0, #0
    bx lr

calc_add:
    adds r0, r1, r2
    bx lr

calc_sub:
    subs r0, r1, r2
    bx lr

calc_mul:
    mul r0, r1, r2
    bx lr

calc_div:
    cmp r2, #0
    beq calc_zero
    udiv r0, r1, r2
    bx lr

calc_zero:
    movs r0, #0
    bx lr
.size Calc4, .-Calc4

// uint32_t Gcd(uint32_t a, uint32_t b);
.global Gcd
.type Gcd, %function
.thumb_func
Gcd:
    cmp r1, #0
    beq gcd_done

gcd_loop:
    udiv r2, r0, r1
    mul r2, r2, r1
    subs r2, r0, r2
    mov r0, r1
    mov r1, r2
    cmp r1, #0
    bne gcd_loop

gcd_done:
    bx lr
.size Gcd, .-Gcd

// uint32_t Factorial(uint32_t n);
.global Factorial
.type Factorial, %function
.thumb_func
Factorial:
    movs r1, #1

factorial_loop:
    cmp r0, #1
    bls factorial_done
    mul r1, r1, r0
    subs r0, r0, #1
    b factorial_loop

factorial_done:
    mov r0, r1
    bx lr
.size Factorial, .-Factorial

// uint32_t Fibonacci(uint32_t n);
.global Fibonacci
.type Fibonacci, %function
.thumb_func
Fibonacci:
    cmp r0, #1
    bls fib_done

    push {r4, r5, r6, lr}
    mov r4, r0

    subs r0, r4, #1
    bl Fibonacci
    mov r5, r0

    subs r0, r4, #2
    bl Fibonacci

    adds r0, r0, r5
    pop {r4, r5, r6, pc}

fib_done:
    bx lr

.size Fibonacci, .-Fibonacci

// uint32_t SortArray(uint32_t p, uint32_t n);
.global SortArray
.type SortArray, %function
.thumb_func
SortArray:
    push {r4-r6, lr}
    movs r2, #1

sort_outer:
    cmp r2, r1
    bhs sort_done
    mov r3, r2

sort_inner:
    cmp r3, #0
    beq sort_next
    lsls r4, r3, #2
    ldr r5, [r0, r4]
    subs r6, r4, #4
    ldr r12, [r0, r6]
    cmp r12, r5
    bls sort_next
    str r12, [r0, r4]
    str r5, [r0, r6]
    subs r3, r3, #1
    b sort_inner

sort_next:
    adds r2, r2, #1
    b sort_outer

sort_done:
    movs r0, #0
    pop {r4-r6, pc}
.size SortArray, .-SortArray

// uint32_t Average(uint32_t p, uint32_t n);
.global Average
.type Average, %function
.thumb_func
Average:
    push {r4, lr}
    cmp r1, #0
    beq average_zero
    movs r2, #0
    movs r3, #0

average_loop:
    cmp r3, r1
    bhs average_done
    lsls r4, r3, #2
    ldr r4, [r0, r4]
    adds r2, r2, r4
    adds r3, r3, #1
    b average_loop

average_done:
    movs r4, #10
    mul r2, r2, r4
    udiv r0, r2, r1
    pop {r4, pc}

average_zero:
    movs r0, #0
    pop {r4, pc}
.size Average, .-Average

