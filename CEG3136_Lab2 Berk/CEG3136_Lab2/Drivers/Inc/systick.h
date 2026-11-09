#ifndef SYSTICK_H_
#define SYSTICK_H_

typedef unsigned int Time_t;

// Driver methods
void ConfigureSysClk();
void StartSysTick();
void WaitForSysTick();
Time_t TimeNow();
Time_t TimePassedSince(Time_t previous);
void DelayFor(Time_t duration);  // Blocking delay!

#endif /* SYSTICK_H_ */
