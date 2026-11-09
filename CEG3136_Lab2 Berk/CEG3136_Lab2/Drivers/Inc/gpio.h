#ifndef GPIO_H_
#define GPIO_H_

#include "mcu.h"

// Base address for a GPIO port
typedef GPIO_TypeDef * Port_t;

// Structure representing a single GPIO pin
typedef struct {
    Port_t port; // GPIOA, GPIOB, etc.
    int index;    // 0..15
} Pin_t;

// Enumerated types for register field values
typedef enum { INPUT = 0x0, OUTPUT = 0x1, ALTFUNC = 0x2, ANALOG = 0x3 } PinMode_t;
typedef enum { LOW = 0, HIGH = 1 } PinState_t;
typedef enum { FALL = 0, RISE = 1 } PinEdge_t;
typedef enum { PP = 0, OD = 1 } PinType_t;
typedef enum { S0 = 0x0, S1 = 0x1, S2 = 0x2, S3 = 0x3 } PinSpeed_t;
typedef enum { NOPUPD = 0x0, PU = 0x1, PD = 0x2 } PinPUPD_t;

// Driver methods
void GPIO_Enable(Pin_t pin);
void GPIO_PortEnable(Port_t port);
void GPIO_Mode(Pin_t pin, PinMode_t mode);
void GPIO_Config(Pin_t pin, PinType_t ot, PinSpeed_t osp, PinPUPD_t pupd);
void GPIO_AltFunc(Pin_t pin, int af);
PinState_t GPIO_Input(Pin_t pin);
void GPIO_Output(Pin_t pin, PinState_t state);
void GPIO_Toggle(Pin_t pin);
void GPIO_Callback(Pin_t pin, void (*func)(void), PinEdge_t edge);

#endif /* GPIO_H_ */
