#include <stdio.h>
#include <stdbool.h>
#include "alarm.h"
#include "systick.h"
#include "gpio.h"
#include "display.h"

#define DEBOUNCE_TIME 0
#define SHORT_PRESS_TIME 2000
#define LONG_PRESS_TIME 3000
#define MIN_LED_BLINK_TIME 1000

typedef enum {
DISARMED,ARMED,TRIGGERED
} AlarmState_t;

static AlarmState_t state = DISARMED;

static const Pin_t RedLED = {GPIOA, 9};
static const Pin_t BlueLED = {GPIOB, 7};
static const Pin_t GreenLED = {GPIOC, 7};
static const Pin_t Motion = {GPIOB, 9};
static const Pin_t Button = {GPIOB, 2};

static volatile bool motionDetectedFlag = false;
static volatile bool shortPressedFlag = false;
static volatile bool longPressedFlag = false;

static volatile Time_t buttonPressTime = 0;
static volatile Time_t buttonHeldTime = 0;
static Time_t lastBlinkTime = 0;
static bool BlueLEDstate = false;
static bool GreenLEDstate = false;

static void CallbackMotionDetect(void);
static void CallbackButtonPress(void);
static void CallbackButtonRelease(void);

static void TurnOffAllLEDs(void)
{
	GPIO_Output(RedLED, LOW);
	GPIO_Output(BlueLED, LOW);
	GPIO_Output(GreenLED, LOW);
}

static void TransitionTo(AlarmState_t newState, const char *reason)
{
	state = newState;
	TurnOffAllLEDs();
	const char *stateNames[] = {"DISARMED", "ARMED", "TRIGGERED"};
	printf("[%u ms] State: %s | Reason: %s\n", TimeNow(), stateNames[newState], reason);
}

void Init_Alarm(void)
{
	GPIO_Enable(RedLED);
	GPIO_Mode(RedLED, OUTPUT);
	GPIO_Output(RedLED, LOW);
	GPIO_Enable(BlueLED);
	GPIO_Mode(BlueLED, OUTPUT);
	GPIO_Output(BlueLED, LOW);
	GPIO_Enable(GreenLED);
	GPIO_Mode(GreenLED, OUTPUT);
	GPIO_Output(GreenLED, LOW);
	GPIO_Enable(Motion);
	GPIO_Mode(Motion, INPUT);
	GPIO_Callback(Motion, CallbackMotionDetect, RISE);
	GPIO_Enable(Button);
	GPIO_Mode(Button, INPUT);
	GPIO_Callback(Button, CallbackButtonPress, RISE);
	GPIO_Callback(Button, CallbackButtonRelease, FALL);

	TransitionTo(DISARMED, "System Initialization");
    
    DisplayEnable();
    DisplayColor(ALARM, WHITE);
    DisplayPrint(ALARM, 0, "DISARMED");
}

void Task_Alarm(void)
{
	switch (state)
	{
	case DISARMED:
		TurnOffAllLEDs();
		break;
	case ARMED:
		if (TimePassedSince(lastBlinkTime) >= MIN_LED_BLINK_TIME)
		{
			lastBlinkTime = TimeNow();
			BlueLEDstate = !BlueLEDstate;
			GreenLEDstate = !BlueLEDstate;
			GPIO_Output(BlueLED, BlueLEDstate);
			GPIO_Output(GreenLED, GreenLEDstate);
		}
		break;
	case TRIGGERED:
		GPIO_Output(RedLED, HIGH);
		break;
	}


	if (TimePassedSince(buttonPressTime) >= LONG_PRESS_TIME && GPIO_Input(Button)) //if button has been pressed for over 50ms but under 2s, transition to Armed
		{
			TransitionTo(DISARMED, "Long Button Press");
            DisplayColor(ALARM, WHITE);
            DisplayPrint(ALARM, 0, "DISARMED");
		}

	if (shortPressedFlag) //if button has been pressed for over 50ms but under 2s, transition to Armed
	{
		shortPressedFlag = false;
		TransitionTo(ARMED, "Short Button Press");
        DisplayColor(ALARM, YELLOW);
        DisplayPrint(ALARM, 0, "ARMED");
	}

	if (motionDetectedFlag) //if motion is detected, transition to triggered
	{
		motionDetectedFlag = false;
		if (state == ARMED)
		{
			TransitionTo(TRIGGERED, "Motion Sensor Triggered");
            DisplayColor(ALARM, RED);
            DisplayPrint(ALARM, 0, "TRIGGERED");
		}
	}
}


static void CallbackMotionDetect(void)
{
	motionDetectedFlag = true;
}

static void CallbackButtonPress(void)
{
	buttonPressTime = TimeNow();
}

static void CallbackButtonRelease(void)
{
	buttonHeldTime = TimePassedSince(buttonPressTime);
	if (DEBOUNCE_TIME <= buttonHeldTime && buttonHeldTime <= SHORT_PRESS_TIME) //if button has been pressed for over 3s, transition to Disarmed
	{
		shortPressedFlag = true;
	}
}
