/*
 * Button_Driver.h
 *
 *  Created on: Jan , 2025
 *      Author: user
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// User Button -> PA0 means Port A, Pin 0
#define EXTI0_IRQ_NUMBER 6
// #define BUTTON_PORT GPIOA
// #define BUTTON_PIN	GPIO_PIN_NUM_0

#define PRESSED 	1
#define NOT_PRESSED 0

void Init_Button(void);

void EnableButtonClock();

void DisableClock();

bool Button_State();

void Init_Button_InterruptMode();

#endif /* BUTTON_DRIVER_H_ */
