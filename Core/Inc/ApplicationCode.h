/*
 * ApplicationCode.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Xavion
 */

#include "LCD_Driver.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "Scheduler.h"
#include "Button_Driver.h"
#include "RNG_Driver.h"
#include "core_cm4.h"
#include "Gyro.h"

#include <stdio.h>
#include <stdbool.h>

#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_

void ApplicationInit(void);
void LCD_Visual_Demo(void);

#if (COMPILE_TOUCH_FUNCTIONS == 1)
void LCD_Touch_Polling_Demo(void);
void GamePlay_1P();
void GamePlay_2P();
void GameScreen();
void Draw_MainMenu();
void DrawCoin();
void MoveCoin();
void MoveCoin_Gyro(int16_t gyro_val);
void Computer_Turn();
void Computer_Drop();
void DropCoin();
bool Connect4();
bool IsTie();
void DisplayFinalScreen(uint8_t winner);
void Reset_Game();
void applicationInitGyro();

#endif // (COMPILE_TOUCH_FUNCTIONS == 1)

#endif /* INC_APPLICATIONCODE_H_ */
