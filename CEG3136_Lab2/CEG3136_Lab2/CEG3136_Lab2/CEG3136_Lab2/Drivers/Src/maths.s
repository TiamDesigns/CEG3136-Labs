// Math routines for Calculator app
.syntax unified
.cpu cortex-m33
.thumb

.section .text

// uint32_t Increment(uint32_t num);
.global Increment
.type Increment, %function
Increment:
    add     r0, r0, #1
    bx      lr

// uint32_t Decrement(uint32_t num);
.global Decrement
.type Decrement, %function
Decrement:
    sub     r0, r0, #1
    bx      lr

// uint32_t Add(uint32_t num1, uint32_t num2);
.global Add
.type Add, %function
Add:
    add     r0, r0, r1
    bx      lr

// uint32_t Sub(uint32_t num1, uint32_t num2);
.global Sub
.type Sub, %function
Sub:
    sub     r0, r0, r1
    bx      lr

// uint32_t Mul(uint32_t num1, uint32_t num2);
.global Mul
.type Mul, %function
Mul:
    mul     r0, r0, r1
    bx      lr

// uint32_t Div(uint32_t num1, uint32_t num2);
.global Div
.type Div, %function
Div:
    udiv     r0, r0, r1
    bx      lr

// uint32_t Factorial(uint32_t num);
.global Factorial
.type Factorial, %function
Factorial:
	PUSH {R4, LR}
	MOV R4, R0 ; //Allow recursion
	CMP R4, #2
	BGE recurse ; //Branch if n >= 2
	MOVS R0, #1
endf:
	POP {R4, PC}
recurse:
	SUBS R0, R4, #1
	BL Factorial
	MUL R0, R4, R0
	B endf

// uint32_t GCD(uint32_t A, uint32_t B);
.global GCD
.type GCD, %function
GCD:
gcd_loop:
	CMP r0, r1
	BEQ gcd_done
	BGT a_greater
	SUB R1,R1,R0
	B gcd_loop

a_greater:
	SUB R0, R0, R1
	B gcd_loop

gcd_done:
	BX lr

// uint32_t Fibonacci(uint32_t n);
.global Fibonacci
.type Fibonacci, %function
Fibonacci:
	CMP R0, #1
	BLS lessThan2
	MOV R1, #1
	MOV R2, #1
	MOV R3, #0
	B fibLoop

fibLoop:
	CMP R0, #2
	BLO endFibLoop
	ADD R3, R1, R2
	MOV R1, R2
	MOV R2, R3
	SUB R0, R0, #1
	B fibLoop

endFibLoop:
	MOV R0, R3
	BX LR

lessThan2:
	MOV R0, #1
	BX  LR
