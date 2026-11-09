/*
 * alarm.c
 *
 *  Created on: Oct 5, 2026
 *      Author: tmorr015
 */


// Alarm system app
#include <stdio.h>
#include "alarm.h"
#include "systick.h"
#include "gpio.h"
#include "display.h"

// GPIO pins
static const Pin_t BlueLED = {GPIOB, 7}; //Blue LED
static const Pin_t RedLED = {GPIOA, 9}; //Red LED
static const Pin_t GreenLED = {GPIOC, 7}; //Green LED
static const Pin_t MotionDetector = {GPIOB, 9}; //Motion detector
static const Pin_t E_STOP = {GPIOB, 2};
static const Pin_t BUZZER = {GPIOA, 0}; //Buzzer pin active high

static enum {DISARMED, ARMED, TRIGGERED} state;

// Constants
#define ShortHold 2000
#define LongHold 3000
#define BLINK_DELAY 1000

// Variables
static Time_t lastToggle;
static int MotionFlag;
static int LongPressFlag;
static int ShortPressFlag;
static int BlinkLastToggle;


// Declare interrupt callback functions
static void CallbackMotionDetect();
static void CallbackButtonPress();
static void CallbackButtonRelease();

// Initialization code
void Init_Alarm (void) {
	GPIO_Enable(BlueLED);
	GPIO_Mode(BlueLED, OUTPUT);
	GPIO_Enable(RedLED);
	GPIO_Mode(RedLED, OUTPUT);
	GPIO_Enable(GreenLED);
	GPIO_Mode(GreenLED, OUTPUT);
	GPIO_Enable(MotionDetector);
	GPIO_Mode(MotionDetector, INPUT);
	GPIO_Enable(E_STOP);
	GPIO_Mode(E_STOP, INPUT);
	GPIO_Enable(E_STOP);
	GPIO_Mode(E_STOP, INPUT);
	GPIO_Enable(BUZZER);
	GPIO_Mode(BUZZER, OUTPUT);

	GPIO_Output(RedLED, LOW);
	GPIO_Output(BlueLED, LOW);
	GPIO_Output(GreenLED, LOW);

	GPIO_Callback(MotionDetector, CallbackMotionDetect, RISE);
	GPIO_Callback(E_STOP, CallbackButtonPress, RISE);
	GPIO_Callback(E_STOP, CallbackButtonRelease, FALL);

	DisplayEnable();
	DisplayColor(ALARM, WHITE);
	DisplayPrint(ALARM, 0, "DISARMED");

	lastToggle = TimeNow();
	MotionFlag = 0;
	LongPressFlag = 0;
	ShortPressFlag = 0;
	BlinkLastToggle = TimeNow();
}

// Task code (state machine)
void Task_Alarm (void) {
switch (state) {
	case DISARMED:
		GPIO_Output(RedLED, LOW);
		GPIO_Output(BlueLED, LOW);
		GPIO_Output(GreenLED, LOW);
		GPIO_Output (BUZZER, LOW);


		if (ShortPressFlag) {
			ShortPressFlag = 0;
			state = ARMED;
			DisplayColor(ALARM, YELLOW);
			DisplayPrint(ALARM, 0, "ARMED");
			GPIO_Output(BlueLED, HIGH);
			GPIO_Output(GreenLED, LOW);
			printf("ARMED at time %u\n", TimeNow());
			BlinkLastToggle = TimeNow();
		}

		MotionFlag = 0;
		LongPressFlag = 0;

		break;

	case ARMED:

		GPIO_Output(RedLED, LOW);
		GPIO_Output (BUZZER, LOW);


		if (LongPressFlag) {
			LongPressFlag = 0;
			state = DISARMED;
			DisplayColor(ALARM, WHITE);
			DisplayPrint(ALARM, 0, "DISARMED");
			printf("DISARMED at time %u\n", TimeNow());

		}

		if (MotionFlag) {
				MotionFlag = 0;
				state = TRIGGERED;
				DisplayColor(ALARM, RED);
				DisplayPrint(ALARM, 0, "TRIGGERED");
				printf("TRIGGERED at time %u\n", TimeNow());
			}

		if (TimePassedSince(BlinkLastToggle) >= BLINK_DELAY){
			GPIO_Toggle (BlueLED);
			GPIO_Toggle (GreenLED);
			BlinkLastToggle = TimeNow();
			}

		ShortPressFlag = 0;

		break;

	case TRIGGERED:

		GPIO_Output(RedLED, HIGH);
		GPIO_Output(BlueLED, LOW);
		GPIO_Output(GreenLED, LOW);

		if (ShortPressFlag) {
			ShortPressFlag = 0;
			state = ARMED;
			DisplayColor(ALARM, YELLOW);
			DisplayPrint(ALARM, 0, "ARMED");
			printf("ARMED at time %u\n", TimeNow());
			GPIO_Output(BlueLED, HIGH);
			GPIO_Output(GreenLED, LOW);
			BlinkLastToggle = TimeNow();
		}

		if (LongPressFlag) {
				LongPressFlag = 0;
				state = DISARMED;
				DisplayColor(ALARM, WHITE);
				DisplayPrint(ALARM, 0, "DISARMED");
				printf("DISARMED at time %u\n", TimeNow());
				// ...
			}

		if (TimePassedSince(BlinkLastToggle) <= 1){
			GPIO_Output (BUZZER, HIGH);
			}

		else if ((TimePassedSince(BlinkLastToggle) > 1) && (TimePassedSince(BlinkLastToggle) <=10)){
			GPIO_Output (BUZZER, LOW);
		}

		else if (TimePassedSince(BlinkLastToggle) > 10){
			BlinkLastToggle = TimeNow();
		}

		MotionFlag = 0;

		break;
	}

	if ((TimePassedSince(lastToggle) > LongHold) && (GPIO_Input(E_STOP) == HIGH))
		LongPressFlag = 1;
}


void CallbackMotionDetect (void) {
	MotionFlag = 1;

}

void CallbackButtonPress (void) {

	lastToggle = TimeNow();


}

void CallbackButtonRelease(void){

	if ((TimePassedSince(lastToggle) <= ShortHold) && (TimePassedSince(lastToggle) >= 50)){
		ShortPressFlag = 1;
	    		}
}

