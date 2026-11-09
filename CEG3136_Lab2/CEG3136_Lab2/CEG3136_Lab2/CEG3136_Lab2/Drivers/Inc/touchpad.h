/*
 * touchpad.h
 *
 *  Created on: Nov 2, 2026
 *      Author: tmorr015
 */

#ifndef TOUCHPAD_H_
#define TOUCHPAD_H_

#include <stdint.h>
#include <stdbool.h>
#include "display.h"

//single press touchpad
typedef enum { NONE=-1, MIN=0, N0=0, N1=1, N2=2, N3=3, N4=4, N5=5, N6=6, N7=7, N8=8, N9=9, SHIFT=10, NEXT=11, MAX=11 } Press_t;

//numeric entry value
typedef uint32_t Entry_t;

void TouchEnable(void);
Press_t TouchInput(Page_t page);
bool TouchEntry(Page_t page, Entry_t *num);

void ScanTouchpad(void); //housekeeping: check for tp input
void ClearTouchpad(void); //discard input buffer


#endif /* TOUCHPAD_H_ */
