/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

/* Static variables */

extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
static uint16_t current_location_x = 120;
// static uint16_t current_location_y = 125;
static uint16_t x_mov = 30;
static uint16_t current_player = LCD_COLOR_RED;
static uint8_t board[6][7] = {0};
static uint16_t placed_rows[7] = {5, 5, 5, 5, 5, 5, 5};
static uint32_t events_to_run;
static uint8_t gameplay = 0;
// static bool status = false;

static uint8_t red_wins = 0;
static uint8_t blue_wins = 0;
static uint32_t start_time = 0;

static uint16_t col;  // col 0~6
static uint16_t row;  // Current col where coin dropped
static uint16_t drop_y;

static uint8_t winner = 0;
// static int count = 0;

static const uint16_t val = 300; // threshold if left or right


#endif // COMPILE_TOUCH_FUNCTIONS

void ApplicationInit(void)
{
	// initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);

    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	EnableButtonClock();
	Init_Button_InterruptMode();
	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	#endif // COMPILE_TOUCH_FUNCTIONS
}

void LCD_Visual_Demo(void)
{
	visualDemo();
}

#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);
		} else {
			/* Touch not pressed */
			printf("Not Pressed\n\n");
			LCD_Clear(0, LCD_COLOR_GREEN);
		}
	}
}

void Draw_MainMenu() {
	HAL_Delay(300);
	LCD_Clear(0, LCD_COLOR_WHITE);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);
	LCD_Draw_Circle_Fill(120, 80, 60, LCD_COLOR_RED); // 1 player mode button circle
	LCD_Draw_Circle_Fill(120, 240, 60, LCD_COLOR_BLUE); // 2 players mode button circle
	// Print 1P on red, 2P on blue on LCD
	LCD_DisplayChar(105,75,'1');
	LCD_DisplayChar(120,75,'P');
	LCD_DisplayChar(105,235,'2');
	LCD_DisplayChar(120,235,'P');

	// Touch Logic
	while(1){
    	// If red is pressed, go to GamePlay_1P
		if((returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) & (StaticTouchData.x >= 60) & (StaticTouchData.x <= 180) & (StaticTouchData.y >= 180) & (StaticTouchData.y <= 300)){
			// go to GamePlay_1P Logic
			if (gameplay == 2){
				red_wins = 0;
				blue_wins = 0;
			}
			GameScreen();
			GamePlay_1P();
		}
		// If blue is pressed, go to GamePlay_2P
		else if((returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) & (StaticTouchData.x >= 60) & (StaticTouchData.x <= 180) & (StaticTouchData.y >= 20) & (StaticTouchData.y <= 140)){
			if (gameplay == 1){
				red_wins = 0;
				blue_wins = 0;
			}
			GameScreen();
			// go to GamePlay_2P Logic
			GamePlay_2P();
		}
	}
}

void GameScreen(){
	// 7 columns x 6 rows board
	LCD_Clear(0, LCD_COLOR_WHITE);
	for(uint16_t i=15; i<=240; i+=30){
		LCD_Draw_Vertical_Line(i, 140, 180, LCD_COLOR_BLACK);
	}
	for(uint16_t i=140; i<=320; i+=30){
		LCD_Draw_Horizontal_Line(15, i, 210, LCD_COLOR_BLACK);
	}
	// DrawCoin();
}

 // Game logic of 1P mode
void GamePlay_1P(){
	gameplay = 1;
	// start_timer
	start_time = HAL_GetTick();

	while(1){
			addSchedulerEvent(TURN_OVER);
			events_to_run = getScheduledEvents();
			DrawCoin();

			while(events_to_run & TURN_OVER){
				  MoveCoin();
				  events_to_run = getScheduledEvents();
				  if(events_to_run & DROP_COIN){
					  DropCoin();
					  DrawCoin();
					  HAL_Delay(300);
					  removeSchedulerEvent(DROP_COIN);
					  // check if game concludes
					  if (Connect4() == true){
						  DisplayFinalScreen(winner);
					  }
					  else if (IsTie() == true){
						  DisplayFinalScreen(winner);
					  }

					  Computer_Turn();
					  DrawCoin();
					  HAL_Delay(300);

					  // check if game concludes
					  if (Connect4() == true){
						  DisplayFinalScreen(winner);
					  }
					  else if (IsTie() == true){
						  DisplayFinalScreen(winner);
				  }
			  }
			}
		}
}

// Game Logic of 2p mode
void GamePlay_2P(){
	gameplay = 2;
	// start_timer
	start_time = HAL_GetTick();
	while(1){
		addSchedulerEvent(TURN_OVER);
		events_to_run = getScheduledEvents();
		DrawCoin();

		while(events_to_run & TURN_OVER){
			  Gyro_Sample();
			  MoveCoin();
			  events_to_run = getScheduledEvents();
			  if(events_to_run & DROP_COIN){

				  DropCoin();
				  DrawCoin();
				  HAL_Delay(300);

				  removeSchedulerEvent(DROP_COIN);
				  // check if game concludes
				  if (Connect4() == true){
					  DisplayFinalScreen(winner);
				  }
				  else if (IsTie() == true){
					  DisplayFinalScreen(winner);
			  }
		  }
		}
	}

}

void DropCoin(){
	    col = (current_location_x - 30) / 30;  // col 0~6

		if (col <= 0) col = 0;
	    if (col >= 6) col = 6;

	    if (placed_rows[col] >= 0) {
	    	row = placed_rows[col];  // Current col where coin dropped
 	        drop_y = 140 + (row * 30) + 15;
 	        if((placed_rows[col] == 0) && ((board[row][col] == 1) || (board[row][col] == 2))){
 	        	return;
 	         }
 	            else {
	            LCD_Draw_Circle_Fill(30 + col * 30, drop_y, 14, current_player);
 	         }

	            // board
	            if (current_player == LCD_COLOR_RED) {
					board[row][col] = 1;  // RED
			   }
	            else {
					board[row][col] = 2;  // BLUE
			   }

	            	if (placed_rows[col] == 0){
	            		if (current_player == LCD_COLOR_RED) {
	            			current_player = LCD_COLOR_BLUE;
	            		} else {
	            			current_player = LCD_COLOR_RED;
	            		}
	            		 return;
	            	}
	            	else{
	            	// Turn over after Dropping
	            			placed_rows[col]--;
	            		    if (current_player == LCD_COLOR_RED) {
	            		    	current_player = LCD_COLOR_BLUE;
	            		    } else {
	            		    	current_player = LCD_COLOR_RED;
	            		    }
	            	}
			}

	    else {
	       return;
	    }
}

void DrawCoin(){
	LCD_Draw_Circle_Fill(current_location_x, 125, 14, LCD_COLOR_WHITE);
	current_location_x = 120;
	LCD_Draw_Circle_Fill(current_location_x, 125, 14, current_player);
}

void MoveCoin(){
	// Touch left
	if((returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) & (StaticTouchData.x <= 100)){
		current_location_x -= x_mov;
		if (current_location_x < 30){
			current_location_x = 30;
		}
		if (current_location_x > 210){
			current_location_x = 210;
		}
		LCD_Draw_Circle_Fill(current_location_x + x_mov, 125, 14, LCD_COLOR_WHITE);
		LCD_Draw_Circle_Fill(current_location_x, 125, 14, current_player);
		HAL_Delay(100);
	}
	// Touch right
	if((returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) & (StaticTouchData.x >= 140)){
		current_location_x += x_mov;
		if (current_location_x < 30){
			current_location_x = 30;
		}
		if (current_location_x > 210){
			current_location_x = 210;
		}
		LCD_Draw_Circle_Fill(current_location_x - x_mov, 125, 14, LCD_COLOR_WHITE);
		LCD_Draw_Circle_Fill(current_location_x, 125, 14, current_player);
		HAL_Delay(100);
		}
	}

void MoveCoin_Gyro(int16_t gyro_val) {
		// move to left
		if(gyro_val < -val){
			current_location_x -= x_mov;
			if (current_location_x < 30){
				current_location_x = 30;
			}
			if (current_location_x > 210){
				current_location_x = 210;
			}
			LCD_Draw_Circle_Fill(current_location_x + x_mov, 125, 14, LCD_COLOR_WHITE);
			LCD_Draw_Circle_Fill(current_location_x, 125, 14, current_player);
			HAL_Delay(300);
		}
		// move to right
		else if(gyro_val > val){
			current_location_x += x_mov;
			if (current_location_x < 30){
				current_location_x = 30;
			}
			if (current_location_x > 210){
				current_location_x = 210;
			}
			LCD_Draw_Circle_Fill(current_location_x - x_mov, 125, 14, LCD_COLOR_WHITE);
			LCD_Draw_Circle_Fill(current_location_x, 125, 14, current_player);
			HAL_Delay(300);
			}
}

bool Connect4(){
	for (int row = 0; row < 6; row++) {
	        for (int col = 0; col < 7; col++) {
	            uint8_t player = board[row][col];
	            if (player == 0)
	                continue;

	            // 1. Horizontal
	            if (col <= 3) {
	                if (player == board[row][col] &&
	                    player == board[row][col+1] &&
	                    player == board[row][col+2] &&
						player == board[row][col+3]) {
	                	 if (current_player == LCD_COLOR_BLUE){
	                		            	winner = 1;
	                		            }
	                		            else if(current_player == LCD_COLOR_RED){
	                		            	winner = 2;
	                		            }
	                    return true;
	                }
	            }

	            // 2. Vertical
	            if (row <= 2) {
	                if (player == board[row][col] &&
	                    player == board[row+1][col] &&
	                    player == board[row+2][col] &&
						player == board[row+3][col]) {
	                	 if (current_player == LCD_COLOR_BLUE){
	                		            	winner = 1;
	                		            }
	                		            else if(current_player == LCD_COLOR_RED){
	                		            	winner = 2;
	                		            }
	                    return true;
	                }
	            }

	            // 3 ↘ (Diagonal Down-Right)
	            if (row <= 2 && col <= 3) {
	                if (player == board[row][col] &&
	                    player == board[row+1][col+1] &&
	                    player == board[row+2][col+2] &&
						player == board[row+3][col+3]) {
	                	 if (current_player == LCD_COLOR_BLUE){
	                		            	winner = 1;
	                		            }
	                		            else if(current_player == LCD_COLOR_RED){
	                		            	winner = 2;
	                		            }
	                    return true;
	                }
	            }

	            // 4. ↗ (Diagonal Up-Right)
	            if (row >= 3 && col <= 3) {
	                if (player == board[row][col] &&
	                    player == board[row-1][col+1] &&
	                    player == board[row-2][col+2] &&
						player == board[row-3][col+3]) {
	                	 if (current_player == LCD_COLOR_BLUE){
	                		            	winner = 1;
	                		            }
	                		            else if(current_player == LCD_COLOR_RED){
	                		            	winner = 2;
	                		            }
	                    return true;
	                }
	            }


	        }
	    }

	    return false;  // not found 4 connect
	}


bool IsTie() {
			if (board[0][0] != 0 &&
				board[0][1] != 0 &&
				board[0][2] != 0 &&
				board[0][3] != 0 &&
				board[0][4] != 0 &&
				board[0][5] != 0 &&
				board[0][6] != 0){
				return true;  // all rows filled -> tie
			}
			else {
			    return false;
			}
		}

void Computer_Turn() {
	    // find win condition
	    for (int c = 0; c < 7; c++) {
	        if (placed_rows[c] >= 0) {
	            int r = placed_rows[c];
	            board[r][c] = 2;
	            if (Connect4()) {
	                board[r][c] = 0;
	                col = c;
	                Computer_Drop();
	                return;
	            }
	            board[r][c] = 0;
	        }
	    }

	    // defend connect 4
	    for (int c = 0; c < 7; c++) {
	        if (placed_rows[c] >= 0) {
	            int r = placed_rows[c];
	            board[r][c] = 1;
	            if (Connect4()) {
	                board[r][c] = 0;
	                col = c;
	                Computer_Drop();
	                return;
	            }
	            board[r][c] = 0;
	        }
	    }

	    // defend in case of player's 3 row
	    for (int c = 0; c < 7; c++) {
	        int r = placed_rows[c];
	        if (r < 0) continue;

	        bool threat_detected = false;
	        board[r][c] = 1;

	        // horizontal
	        int count = 0;
	        for (int i = -2; i <= 2 && !threat_detected; i++) {
	            int cc = c + i;
	            if (cc >= 0 && cc < 7) {
	                count = (board[r][cc] == 1) ? count + 1 : 0;
	                if (count >= 2) threat_detected = true;
	            }
	        }

	        // vertical
	        count = 0;
	        for (int i = 1; i <= 2 && !threat_detected; i++) {
	            int rr = r + i;
	            if (rr < 6 && board[rr][c] == 1) {
	                count++;
	                if (count >= 2) threat_detected = true;
	            } else {
	                break;
	            }
	        }

	        // ↘
	        count = 0;
	        for (int i = -2; i <= 2 && !threat_detected; i++) {
	            int rr = r + i;
	            int cc = c + i;
	            if (rr >= 0 && rr < 6 && cc >= 0 && cc < 7) {
	                count = (board[rr][cc] == 1) ? count + 1 : 0;
	                if (count >= 2) threat_detected = true;
	            }
	        }

	        // ↗
	        count = 0;
	        for (int i = -2; i <= 2 && !threat_detected; i++) {
	            int rr = r - i;
	            int cc = c + i;
	            if (rr >= 0 && rr < 6 && cc >= 0 && cc < 7) {
	                count = (board[rr][cc] == 1) ? count + 1 : 0;
	                if (count >= 2) threat_detected = true;
	            }
	        }

	        board[r][c] = 0;

	        if (threat_detected) {
	            col = c;
	            Computer_Drop();
	            return;
	        }
	    }

	    // capture middle places to seek victory
	    int preferred[7] = {3, 2, 4, 1, 5, 0, 6};
	    for (int i = 0; i < 7; i++) {
	        int c = preferred[i];
	        if (placed_rows[c] >= 0) {
	            col = c;
	            Computer_Drop();
	            return;
	        }
	    }
	}

void Computer_Drop() {
	if (col <= 0) col = 0;
	if (col >= 6) col = 6;

	if (placed_rows[col] >= 0) {
		row = placed_rows[col];  // Current col where coin dropped
	 	drop_y = 140 + (row * 30) + 15;
	 	if((placed_rows[col] == 0) && ((board[row][col] == 1) || (board[row][col] == 2))){
	 		return;
	 	} else {
		    LCD_Draw_Circle_Fill(30 + col * 30, drop_y, 14, current_player);

	 	}

	 		board[row][col] = 2;  // BLUE


		if (placed_rows[col] == 0){
			current_player = LCD_COLOR_RED;
		    return;
		}
		else{
			// Turn over after Dropping
			placed_rows[col]--;
			current_player = LCD_COLOR_RED;
		}
	}

	 else {
		 return;
	 }

}

void DisplayFinalScreen(uint8_t winner) {
    LCD_Clear(0, LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);

    uint32_t elapsed_time = (HAL_GetTick() - start_time) / 1000;  // ms -> seconds

    if (winner == 1) {
    	// red win
        red_wins++;
    } else if (winner == 2) {
    	// blue win
        blue_wins++;
    }
    else {

    }
//    else {
//    	return;
//    }

    // print score
    // "RED: x  BLUE: y"
    char score_msg[50];
    sprintf(score_msg, "RED:%d BLUE:%d", red_wins, blue_wins);
    for (int i = 0; score_msg[i] != '\0'; i++) {
        LCD_DisplayChar(30 + i*15, 100, score_msg[i]);
    }

    // print elapsed game time
    // "TIME: x sec"
    char time_msg[30];
    sprintf(time_msg, "TIME: %ld sec", elapsed_time);
    for (int i = 0; time_msg[i] != '\0'; i++) {
        LCD_DisplayChar(30 + i*15, 140, time_msg[i]);
    }



    // Restart button
    LCD_SetTextColor(LCD_COLOR_GREEN);
    LCD_Draw_Circle_Fill(120, 250, 65, LCD_COLOR_GREEN);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);
    LCD_DisplayChar(85, 240, 'R');
    LCD_DisplayChar(100, 240, 'E');
    LCD_DisplayChar(115, 240, 'S');
    LCD_DisplayChar(130, 240, 'E');
    LCD_DisplayChar(145, 240, 'T');

    // wating for touch from the user
    while(1){
        if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed &&
        	(StaticTouchData.x >= 55) &&
			(StaticTouchData.x <= 185) &&
			(StaticTouchData.y >= 15) &&
			(StaticTouchData.y <= 135)) {
//        		NVIC_SystemReset();
        	break;
        }
    }
    Reset_Game();
    Draw_MainMenu();
}

// Game Reset
void Reset_Game(){
	current_location_x = 120;
	current_player = LCD_COLOR_RED;
	for (int i=0; i<6; i++){
		for (int j=0; j<7; j++){
			board[i][j] = 0;
		}
	}
	for(int k=0; k<7; k++){
		placed_rows[k] = 5;
	}

	start_time = 0;

	winner = 0;

	//reset_check = 0;

	// 1p -> 2p, 2p -> 1p : score restart

//	if (reset_check == true){
//		reset_check = false;
//	}
}

void applicationInitGyro(){
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	Gyro_Init();
	HAL_Delay(100);
}


#endif // COMPILE_TOUCH_FUNCTIONS

