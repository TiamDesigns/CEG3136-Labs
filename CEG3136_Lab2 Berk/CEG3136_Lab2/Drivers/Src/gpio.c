// General-purpose input/output (GPIO) driver

#include "gpio.h"

#include "mcu.h"

// --------------------------------------------------------
// Initialization
// --------------------------------------------------------

// Enable the specified GPIO pin
void GPIO_Enable (Pin_t pin) {
	GPIO_PortEnable(pin.port);
}

// Enable the peripheral clock for a GPIO port
void GPIO_PortEnable (Port_t port) {
	int PortNum = ((uint32_t)port - (uint32_t)GPIOA) / 0x400;
	RCC -> AHB2ENR |= 1 << PortNum;

}

// Set the operating mode of a GPIO pin
void GPIO_Mode (Pin_t pin, PinMode_t mode) {
	uint32_t regVal;

	regVal = pin.port->MODER;
	regVal &= ~(0b11 << 2*pin.index);
	regVal|= (mode << 2*pin.index);
	pin.port->MODER = regVal;

}


// Configure additional settings for a GPIO pin
void GPIO_Config(Pin_t pin, PinType_t ot, PinSpeed_t osp, PinPUPD_t pupd) {
    // 1. OTYPER (1 bit per pin)
    pin.port->OTYPER = (pin.port->OTYPER & ~(1 << pin.index)) | (ot << pin.index);

    // 2. OSPEEDR (2 bits per pin)
    pin.port->OSPEEDR = (pin.port->OSPEEDR & ~(3 << (2 * pin.index))) | (osp << (2 * pin.index));

    // 3. PUPDR (2 bits per pin)
    pin.port->PUPDR = (pin.port->PUPDR & ~(3 << (2 * pin.index))) | (pupd << (2 * pin.index));
}

// Select which alternate function is to be used in AF mode
void GPIO_AltFunc(Pin_t pin, int af) {
    int reg = pin.index / 8; // 0 for AFRL (pins 0-7), 1 for AFRH (pins 8-15)
    int shift = (pin.index % 8) * 4; // 4 bits per alternate function field

    pin.port->AFR[reg] = (pin.port->AFR[reg] & ~(0xF << shift)) | (af << shift);
}


// --------------------------------------------------------
// Pin observation and control
// --------------------------------------------------------

// Observe the state of an input pin
PinState_t GPIO_Input (const Pin_t pin) {
	if (pin.port->IDR & (1<<pin.index))
		return HIGH;
	else
		return LOW;

}

// Control the state of an output pin
void GPIO_Output (Pin_t pin, const PinState_t state) {
	if (state == HIGH)
		pin.port->BSRR = 1 <<pin.index;
	else
		pin.port->BSRR = 1 << (pin.index + 16);

}

// Toggle the state of an output pin
void GPIO_Toggle (Pin_t pin) {
	pin.port->ODR^=1 <<pin.index;

}

// --------------------------------------------------------
// Interrupt handling
// --------------------------------------------------------

// Array of callback function pointers
// Bits 0 to 15 (each can select one port GPIOA to GPIOH)
// Rising and falling edge triggers for each
static void (*callbacks[16][2]) (void);

// Register a function to be called when an interrupt occurs
void GPIO_Callback(Pin_t pin, void (*func)(void), PinEdge_t edge)
{
	callbacks[pin.index][edge] = func;

	// Enable interrupt generation
	if (edge == RISE)
	  EXTI->RTSR1 |= 1 << pin.index;
	else     // FALL
	  EXTI->FTSR1 |= 1 << pin.index;

	int portNum = ((uint32_t)pin.port - (uint32_t)GPIOA) / 0x400;
	EXTI->EXTICR[pin.index / 4] |= portNum << 8*(pin.index % 4);
	EXTI->IMR1 |= 1 << pin.index;

	// Enable interrupt vector
	NVIC->IPR[EXTI0_IRQn + pin.index] = 0;
	__COMPILER_BARRIER();
	NVIC->ISER[(EXTI0_IRQn + pin.index) / 32] = 1 << ((EXTI0_IRQn + pin.index) % 32);
	__COMPILER_BARRIER();
}

// Interrupt handler for all GPIO pins
void GPIO_IRQHandler (int i) {
	// Clear pending IRQ
    NVIC->ICPR[(EXTI0_IRQn + i) / 32] = 1 << ((EXTI0_IRQn + i) % 32);

    // Detect rising edge
	if (EXTI->RPR1 & (1 << i)) {
		EXTI->RPR1 = (1 << i);  // Service interrupt
		callbacks[i][RISE]();   // Invoke callback function
	}

    // Detect falling edge
	if (EXTI->FPR1 & (1 << i)) {
		EXTI->FPR1 = (1 << i);  // Service interrupt
		callbacks[i][FALL]();   // Invoke callback function
	}
}

// Dispatch all GPIO IRQs to common handler function
void  EXTI0_IRQHandler() { GPIO_IRQHandler( 0); }
void  EXTI1_IRQHandler() { GPIO_IRQHandler( 1); }
void  EXTI2_IRQHandler() { GPIO_IRQHandler( 2); }
void  EXTI3_IRQHandler() { GPIO_IRQHandler( 3); }
void  EXTI4_IRQHandler() { GPIO_IRQHandler( 4); }
void  EXTI5_IRQHandler() { GPIO_IRQHandler( 5); }
void  EXTI6_IRQHandler() { GPIO_IRQHandler( 6); }
void  EXTI7_IRQHandler() { GPIO_IRQHandler( 7); }
void  EXTI8_IRQHandler() { GPIO_IRQHandler( 8); }
void  EXTI9_IRQHandler() { GPIO_IRQHandler( 9); }
void EXTI10_IRQHandler() { GPIO_IRQHandler(10); }
void EXTI11_IRQHandler() { GPIO_IRQHandler(11); }
void EXTI12_IRQHandler() { GPIO_IRQHandler(12); }
void EXTI13_IRQHandler() { GPIO_IRQHandler(13); }
void EXTI14_IRQHandler() { GPIO_IRQHandler(14); }
void EXTI15_IRQHandler() { GPIO_IRQHandler(15); }

