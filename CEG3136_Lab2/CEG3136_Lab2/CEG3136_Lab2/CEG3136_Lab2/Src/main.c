// Main program

#include <stdint.h>
#include <stdio.h>
#include "systick.h"
#include "gpio.h"
#include "alarm.h"
#include "i2c.h"
#include "touchpad.h"
#include "calc.h"

int main(void) {
    ConfigureSysClk();
    printf("Welcome to Lab %c%c%c%d!\n", 67, 69, 71, 0xC40);

    Init_Alarm();
    Init_Calc();

    StartSysTick();

    while(1){
    	Task_Alarm();
    	Task_Calc();

    	//housekeeping
    	UpdateDisplay();
    	ScanTouchpad();
    	ServiceI2CRequests();

    	WaitForSysTick();
    }
}


