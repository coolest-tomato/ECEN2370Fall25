/*
 * InterruptControl.c
 *
 *  Created on: Feb 16, 2025
 *      Author: user
 */

#include "InterruptControl.h"

void Enable_IRQ(uint8_t IRQ_Number){
	if (IRQ_Number < 32) {
		*NVIC_ISER0 |= (1 << IRQ_Number);
	}
}

void Disable_IRQ(uint8_t IRQ_Number){
	if (IRQ_Number < 32) {
		*NVIC_ISER0 |= (1 << IRQ_Number);
		}
}

void Clear_IRQ(uint8_t IRQ_Number){
	if (IRQ_Number < 32) {
		*NVIC_ICPR0 |= (1 << IRQ_Number);
		}
}

void Set_IRQ(uint8_t IRQ_Number){
	if (IRQ_Number < 32) {
		*NVIC_ISPR0 |= (1 << IRQ_Number);
	}
}

void Clear_EXTI_Pending(uint8_t PinNumber){
	EXTI->PR |= (1 << PinNumber);
}
