/*
 * Button_Driver.c
 *
 *  Created on: Jan , 2025
 *      Author: user
 */

#include "Button_Driver.h"

GPIO_InitTypeDef button = {0};
GPIO_InitTypeDef button_interrupt = {0};

void Init_Button(void){
	button.Pin = GPIO_PIN_0;
	button.Mode = GPIO_MODE_INPUT;
	button.Pull = GPIO_PULLDOWN;
	button.Speed = GPIO_SPEED_FREQ_HIGH;
//	button.Alternate = GPIO_MODE_AF_OD;
	EnableButtonClock();
	HAL_GPIO_Init(GPIOA,&button);
	}

void EnableButtonClock(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
}
void DisableClock(){
	__HAL_RCC_GPIOA_CLK_DISABLE();
}

bool Button_State(void){
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == PRESSED){
		return true;
		}
	else {
		return false;
	}
}

void Init_Button_InterruptMode(){
	button_interrupt.Pin = GPIO_PIN_0;
	button_interrupt.Mode = GPIO_MODE_IT_FALLING;
	button_interrupt.Pull = GPIO_PULLDOWN;
	button_interrupt.Speed = GPIO_SPEED_FREQ_HIGH;
	button_interrupt.Alternate = GPIO_MODE_AF_OD;
	EnableButtonClock();
	HAL_GPIO_Init(GPIOA,&button_interrupt);
	HAL_NVIC_EnableIRQ(EXTI0_IRQ_NUMBER);
	}
