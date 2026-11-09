// System timer (SysTick) driver

#include "systick.h"

#include <stdbool.h>
#include <stdio.h>
#include "mcu.h"

#define SYSCLK_FREQ  48e6  // 48MHz clock
#define SYSTICK_US   1000  // 1ms tick

#define TIME_MAX (Time_t)(-1)

static volatile Time_t sysTime;       // Current system time
static bool hasTicked = false;        // Already ticked since time we waited
static bool clockConfigured = false;  // SYSCLK already configured

// Configure the System Clock (SYSCLK)
void ConfigureSysClk (void) {
	if (!clockConfigured) {
		// Multi-speed oscillator (MSI)
		RCC->CR |= RCC_CR_MSION | RCC_CR_MSIRDY | RCC_CR_MSIRGSEL;
		RCC->CR = (RCC->CR & ~RCC_CR_MSIRANGE_Msk) | 0x6 << RCC_CR_MSIRANGE_Pos;

		// Phase-locked loop (PLL)
		RCC->CR &= ~RCC_CR_PLLON;                       // Disable PLL
		RCC->PLLCFGR =   24 << RCC_PLLCFGR_PLLN_Pos     // 2MHz x 24 = 48MHz
					 | 0b01 << RCC_PLLCFGR_PLLSRC_Pos;  // Select MSI clock input
		RCC->CR |= RCC_CR_PLLON;    // Enable PLL
		while ((RCC->CR & RCC_CR_PLLRDY) == 0) { }      // Wait for PLL to lock
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;             // Enable PLL clock output

		// Add flash read wait states to account for faster SYSCLK
		FLASH->ACR |= 0x2 << FLASH_ACR_LATENCY_Pos;

		// Select PLL as System Clock (SYSCLK) source
		RCC->CFGR |= 0x3 << RCC_CFGR_SW_Pos;           // Select new source
		while (!(RCC->CFGR & RCC_CFGR_SWS_Msk)) { }    // Wait for switch

		// Send a dummy NUL character to flush out any glitches due to SYSCLK change
		ITM_SendChar('\0');

		clockConfigured = true;
	}
}

// Start the System Timer (SysTick)
void StartSysTick (void) {
	sysTime = 0;
	hasTicked = false;

    ConfigureSysClk();
	SysTick->LOAD = (uint32_t)(SYSCLK_FREQ/SYSTICK_US - 1);  // Set reload register value
	SCB->SHPR[12+SysTick_IRQn] = 7 << 5;                     // Set interrupt priority
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
				    SysTick_CTRL_TICKINT_Msk   |
				    SysTick_CTRL_ENABLE_Msk;

	printf("SysTick started\n");
}

// Interrupt handler
void SysTick_Handler (void) {
	sysTime++;
	hasTicked = true;
}

// Wait for system time to change (at least one tick)
void WaitForSysTick (void) {
	while (!hasTicked)
		// Instruction to keep CPU asleep until next interrupt
		__asm volatile ("wfi");

	hasTicked = false;
}

// Obtain the current system time
Time_t TimeNow (void) {
	return sysTime;
}

// Calculate the elapsed system time since a previous event
Time_t TimePassedSince (Time_t previous) {
	Time_t now = TimeNow();
	if (now >= previous)
		return now - previous;
	else
		return now + 1 + TIME_MAX - previous;
}

// Blocking delay - Incompatible with cooperative multitasking!
void DelayFor (Time_t duration) {
    Time_t start = TimeNow();
    while (TimePassedSince(start) < duration)
        __asm volatile ("wfi");
}
