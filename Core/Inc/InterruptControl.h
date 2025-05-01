/*
 * InterruptControl.h
 *
 *  Created on: Feb 16, 2025
 *      Author: user
 */

#ifndef INTERRUPTCONTROL_H_
#define INTERRUPTCONTROL_H_
#include <stdint.h>
#include "stm32f4xx_hal.h"

#define EXTI0_IRQ_NUMBER 6

#define NVIC_ISER0 ((volatile uint32_t*) 0xE000E100) //Set-Enable
#define NVIC_ICER0 ((volatile uint32_t*) 0xE000E180) //Clear-Enable
#define NVIC_ISPR0 ((volatile uint32_t*) 0xE000E200) //Set-Pending
#define NVIC_ICPR0 ((volatile uint32_t*) 0xE000E280) //Clear-Pending

void Enable_IRQ(uint8_t IRQ_Number);

void Disable_IRQ(uint8_t IRQ_Number);

void Clear_IRQ(uint8_t IRQ_Number);

void Set_IRQ(uint8_t IRQ_Number);

void Clear_EXTI_Pending(uint8_t PinNumber);

#endif /* INTERRUPTCONTROL_H_ */

