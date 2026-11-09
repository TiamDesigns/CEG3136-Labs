// Calculator app

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "calc.h"
#include "display.h"
#include "touchpad.h"

#define NMAX  10  // Maximum number of operands

static Press_t operation;      // Selected operation
static Press_t subOperation;   // Selected sub-operation
static Entry_t operand[NMAX];  // Numeric operands
static bool done;              // Operand entry complete
static int count;              // Count of operands received
static Entry_t result;         // Calculation result
static int scrollCount;

// Flags
static int subOpFlag;

static enum {MENU, PROMPT, CLASSIC4, ENTRY, RUN, SHOW, SCROLL, WAIT} state;

// Assembly subroutines to be called as functions
uint32_t Increment(uint32_t n);
uint32_t Decrement(uint32_t n);
uint32_t Add(uint32_t num1, uint32_t num2);
uint32_t Sub(uint32_t num1, uint32_t num2);
uint32_t Mul(uint32_t num1, uint32_t num2);
uint32_t Div(uint32_t num1, uint32_t num2);
uint32_t Factorial(uint32_t num);
uint32_t GCD(uint32_t A, uint32_t B);
uint32_t Fibonacci(uint32_t n);

// Initialization
void Init_Calc (void) {
    DisplayEnable();
    TouchEnable();
}

// Runtime
void Task_Calc (void) {
    switch (state) {

    case MENU:
        // Prepare for a new operation
        operation = NONE;
        subOperation = NONE;
        scrollCount = 0;
    	subOpFlag = 0;
        for (int i = 0; i < NMAX; i++)
            operand[i] = 0;
        count = 0;
        result = 0;

        // Display menu
        DisplayPrint(CALC, 0, "Calculator app");
        DisplayPrint(CALC, 1, "1:INC 2:DEC ...");
        state = PROMPT;

        break;

    case PROMPT:

        if (operation == NONE)
            operation = TouchInput(CALC);

        switch ((int)operation) {
        case 1:   // Increment
        case 2:   // Decrement
        case 4:   // Factorial
        case 6:   // Fibonacci
            if (count == 1)
                state = RUN;
            else {
                DisplayPrint(CALC, 0, "Enter a number:");
                state = ENTRY;
            }
            break;

         case 3:	// Classic 4
             // Display menu
             DisplayPrint(CALC, 0, "Select Classic");
             DisplayPrint(CALC, 1, "1:ADD 2:SUB ...");

             state = CLASSIC4;
        	 break;


         case 5:	// GCD
         	if (count == 2) {
         		 state = RUN;
         	} else if (count == 1) {
           		 DisplayPrint(CALC, 0, "Enter num2:");
           		 state = ENTRY;
         	} else {
          		 DisplayPrint(CALC, 0, "Enter num1:");
          		 state = ENTRY;

         	}
        	 break;

         case NEXT:
         	state = SCROLL;
         	operation = NONE;
         	break;

         default:
            operation = NONE;
            break;

        }

        break;

    case CLASSIC4:

     subOpFlag = 1;

     if (subOperation == NONE) {
    	 subOperation = TouchInput(CALC);
     }

   	 switch ((int)subOperation) {

        case 1: // ADD
        case 2: // SUBTRACT
        case 3: // MULTIPLY
        case 4: // DIVIDE

        	if (count == 2) {
        		 state = RUN;
        	} else if (count == 1) {
          		 DisplayPrint(CALC, 0, "Enter num2:");
          		 state = ENTRY;
        	} else {
         		 DisplayPrint(CALC, 0, "Enter num1:");
         		 state = ENTRY;

        	}
        	break;

        case NEXT:
        	subOperation = NONE;
        	state = SCROLL;
        	break;

        default:
            subOperation = NONE;
        	break;
   	 }
    	break;

    case ENTRY:
        done = TouchEntry(CALC, &operand[count]);
        DisplayPrint(CALC, 1, "%u", operand[count]);
        if (done) {
            count++;
            if (subOpFlag) {
            	state = CLASSIC4;
            } else {
            	state = PROMPT;
            }
        }
        break;

    case RUN:

        DisplayPrint(CALC, 0, "Calculating...");
        DisplayPrint(CALC, 1, "");
        state = SHOW;

        switch ((int)operation) {
        case 1: result = Increment(operand[0]); break;
        case 2: result = Decrement(operand[0]); break;
        case 3:
        	switch ((int)subOperation) {
        	case 1: result = Add(operand[0], operand[1]); break;
        	case 2: result = Sub(operand[0], operand[1]); break;
        	case 3: result = Mul(operand[0], operand[1]); break;
        	case 4: result = Div(operand[0], operand[1]); break;
        	} break;
        case 4: result = Factorial(operand[0]); break;
        case 5: result = GCD(operand[0], operand[1]); break;
        case 6: result = Fibonacci(operand[0]); break;
        default: break;
        }
        break;

    case SHOW:
        DisplayPrint(CALC, 0, "Result:");
        DisplayPrint(CALC, 1, "%u", result);
        state = WAIT;
        break;

    case SCROLL:
    	scrollCount += 1;

    	if (subOpFlag) {

    		state = CLASSIC4;

            DisplayPrint(CALC, 0, "Select Classic");

    		switch (scrollCount) {
    		case 0:
                DisplayPrint(CALC, 1, "1:ADD 2:SUB ...");
                break;
    		case 1:
                DisplayPrint(CALC, 1, "2:SUB 3:MUL ...");
                break;
    		case 2:
                DisplayPrint(CALC, 1, "3:MUL 4:DIV ...");
                break;
    		case 3:
                DisplayPrint(CALC, 1, "4:DIV 1:ADD ...");
                break;
    		default:
    			scrollCount = 0;
    			DisplayPrint(CALC, 1, "1:ADD 2:SUB ...");
    			break;
    		}

    	} else {

    		state = PROMPT;

            DisplayPrint(CALC, 0, "Calculator app");

    		switch (scrollCount) {
    		case 0:
                DisplayPrint(CALC, 1, "1:INC 2:DEC ...");
                break;
    		case 1:
                DisplayPrint(CALC, 1, "2:DEC 3:C4F ...");
                break;
    		case 2:
                DisplayPrint(CALC, 1, "3:C4F 4:FAC ...");
                break;
    		case 3:
                DisplayPrint(CALC, 1, "4:FAC 5:GCD ...");
                break;
    		case 4:
                DisplayPrint(CALC, 1, "5:GCD 6:FIB ...");
                break;
    		case 5:
                DisplayPrint(CALC, 1, "6:FIB 7:SOR ...");
                break;
    		case 6:
                DisplayPrint(CALC, 1, "7:SOR 8:AVG ...");
                break;
    		case 7:
				DisplayPrint(CALC, 1, "8:AVG 1:INC ...");
				break;
    		default:
    			scrollCount = 0;
    			DisplayPrint(CALC, 1, "1:INC 2:DEC ...");
    			break;
    		}

    	}

		break;

    case WAIT:
        // Press any pad to return to the menu
        if (TouchInput(CALC) != NONE)
            state = MENU;
        break;
    }
}
