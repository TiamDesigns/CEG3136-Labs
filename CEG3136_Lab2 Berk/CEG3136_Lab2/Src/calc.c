// Calculator app

#include <stdbool.h>
#include <stdint.h>

#include "calc.h"
#include "display.h"
#include "touchpad.h"

#define NMAX 11 //Maximum number of operands
#define FIB_MAX 20 //Maximum Fibonacci
#define FAC_MAX 12 //Maximum Factorial

static Press_t operation; //Selected operation
static Entry_t operand[NMAX]; //Numeric operands
static bool done; //Operand entry complete
static int count; //Count of operands received
static int needed; //Number of operands needed
static Entry_t result; //Calculation result
static int menuPage = 0; //Menu page index

static bool arrayCountEntered; //Whole array has been entered
static bool sortedNeedsDisplay; //Display check for sort function
static int sortedIndex; //Index for sort function

static enum {
    MENU,
    PROMPT,
    ENTRY,
    RUN,
    SHOW,
    SORT_SHOW,
    WAIT
} state = MENU;

extern uint32_t Increment(uint32_t n);
extern uint32_t Decrement(uint32_t n);
extern uint32_t Calc4(uint32_t op, uint32_t a, uint32_t b);
extern uint32_t Gcd(uint32_t a, uint32_t b);
extern uint32_t Factorial(uint32_t n);
extern uint32_t Fibonacci(uint32_t n);
extern uint32_t SortArray(uint32_t p, uint32_t n);
extern uint32_t Average(uint32_t p, uint32_t n);

static void ShowMenu(void) {
    DisplayPrint(CALC, 0, "Calculator app");
    switch (menuPage) {
    case 0:
        DisplayPrint(CALC, 1, "1:INC 2:DEC");
        break;
    case 1:
        DisplayPrint(CALC, 1, " 3:+ 4:- 5:x 6:/");
        break;
    case 2:
        DisplayPrint(CALC, 1, " 7:GCD 8:! 9:FIB");
        break;
    case 3:
        DisplayPrint(CALC, 1, " 10:SORT 11:AVG");
        break;
    }
}

static void ShowPrompt(void) {
    switch (operation) {
    case 1:
    case 2:
    	DisplayPrint(CALC, 0, "x =");
    	break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (count == 0)
            DisplayPrint(CALC, 0, "a =");
        else
            DisplayPrint(CALC, 0, "b (~=0) =");
        break;
    case N8:
        DisplayPrint(CALC, 0, "n (<=12) =");
        break;
    case N9:
        DisplayPrint(CALC, 0, "n (<=20) =");
        break;
    case 10:
    case 11:
        if (!arrayCountEntered)
            DisplayPrint(CALC, 0, "Size (1-10):");
        else
            DisplayPrint(CALC, 0, "Value %d:", count);
        break;
    default:
        break;
    }

    DisplayPrint(CALC, 1, "%lu", (unsigned long)operand[count]);
}

void Init_Calc(void) {
    DisplayEnable();
    TouchEnable();
}

void Task_Calc(void) {
    switch (state) {
    case MENU:
        operation = NONE;
        for (int i = 0; i < NMAX; i++)
            operand[i] = 0;
        count = 0;
        needed = 0;
        result = 0;
        arrayCountEntered = false;
        sortedNeedsDisplay = false;
        sortedIndex = 0;

        ShowMenu();
        state = PROMPT;
        break;

    case PROMPT: {
        if (operation == NONE) {
        	Press_t key = TouchInput(CALC);
            if (key == NEXT) {
                menuPage = (menuPage + 1) % 4;
                ShowMenu();
                break;
            }

            switch (key) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                operation = key;
                break;
            default:
                break;
            }

            if (operation == NONE)
                break;

            count = 0;
            arrayCountEntered = false;
            switch (operation) {
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                needed = 2;
                break;
            case 1:
            case 2:
            case 8:
            case 9:
            case 10:
            case 11:
                needed = 1;
                break;
            default:
                needed = 0;
                break;
            }
        }

        if (operation == 6 && count == 2 && operand[1] == 0) {
            count = 1;
            operand[1] = 0;
            DisplayPrint(CALC, 0, "b cannot be 0!");
            DisplayPrint(CALC, 1, "Enter b:");
            state = ENTRY;
            break;
        }

        if (operation == 8 && count == 1 && operand[0] > FAC_MAX) {
            count = 0;
            operand[0] = 0;
            DisplayPrint(CALC, 0, "Maximum n is 12");
            DisplayPrint(CALC, 1, "n (<=12) =");
            state = ENTRY;
            break;
        }

        if (operation == 9 && count == 1 && operand[0] > FIB_MAX) {
            count = 0;
            operand[0] = 0;
            DisplayPrint(CALC, 0, "Maximum n is 20");
            DisplayPrint(CALC, 1, "n (<=20) =");
            state = ENTRY;
            break;
        }

        if ((operation == 10 || operation == 11) &&
            !arrayCountEntered && count == 1) {
            if (operand[0] < 1 || operand[0] > 10) {
                count = 0;
                operand[0] = 0;
                DisplayPrint(CALC, 0, "Invalid count");
                DisplayPrint(CALC, 1, "How many (1-10):");
                state = ENTRY;
                break;
            }
            arrayCountEntered = true;
            needed = operand[0] + 1;
        }

        if (count >= needed) {
            state = RUN;
        } else {
            ShowPrompt();
            state = ENTRY;
        }
        break;
    }

    case ENTRY: {
        Entry_t oldValue = operand[count];
        done = TouchEntry(CALC, &operand[count]);
        if (operand[count] != oldValue) {
            DisplayPrint(CALC, 1, "%lu", (unsigned long)operand[count]);
        }
        if (done) {
            count++;
            state = PROMPT;
        }
        break;
    }

    case RUN:
        DisplayPrint(CALC, 0, "Calculating...");
        DisplayPrint(CALC, 1, "");
        switch (operation) {
        case 1:
        	result = Increment(operand[0]);
        	state = SHOW;
        	break;
        case 2:
        	result = Decrement(
        			operand[0]);
        	state = SHOW;
        	break;
        case 3:
            result = Calc4(0, operand[0], operand[1]);
            state = SHOW;
            break;
        case 4:
            result = Calc4(1, operand[0], operand[1]);
            state = SHOW;
            break;
        case 5:
            result = Calc4(2, operand[0], operand[1]);
            state = SHOW;
            break;
        case 6:
            result = Calc4(3, operand[0], operand[1]);
            state = SHOW;
            break;
        case 7:
            result = Gcd(operand[0], operand[1]);
            state = SHOW;
            break;
        case 8:
            result = Factorial(operand[0]);
            state = SHOW;
            break;
        case 9:
            result = Fibonacci(operand[0]);
            state = SHOW;
            break;
        case 10:
            SortArray((uint32_t)(uintptr_t)&operand[1], operand[0]);
            sortedIndex = 0;
            sortedNeedsDisplay = true;
            state = SORT_SHOW;
            break;
        case 11:
            result = Average((uint32_t)(uintptr_t)&operand[1], operand[0]);
            state = SHOW;
            break;
        default:
            state = MENU;
            break;
        }
        break;

    case SHOW:
        if (operation == 11) {
            DisplayPrint(CALC, 0, "Average:");
            DisplayPrint(CALC, 1, "%lu.%lu", (unsigned long)(result / 10), (unsigned long)(result % 10));
        }
        else {
            DisplayPrint(CALC, 0, "Result:");
            DisplayPrint(CALC, 1, "%lu", (unsigned long)result);
        }
        state = WAIT;
        break;

    case SORT_SHOW:
        if (sortedNeedsDisplay) {
            DisplayPrint(CALC, 0, "Sorted %d/%lu", sortedIndex + 1, (unsigned long)operand[0]);
            DisplayPrint(CALC, 1, "%lu", (unsigned long)operand[sortedIndex + 1]);
            sortedNeedsDisplay = false;
        }
        if (TouchInput(CALC) == NEXT) {
            sortedIndex++;
            if (sortedIndex >= (int)operand[0])
                state = WAIT;
            else
                sortedNeedsDisplay = true;
        }
        break;

    case WAIT:
        if (TouchInput(CALC) != NONE)
            state = MENU;
        break;
    }
}
