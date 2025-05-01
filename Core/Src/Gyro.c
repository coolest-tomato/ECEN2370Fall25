/*
 * Gyro.c
 *
 *  Created on: Apr 1, 2025
 *      Author: user
 */

#include "Gyro.h"

static HAL_StatusTypeDef HAL_status = ENABLE;
static SPI_HandleTypeDef HSPI;

void Gyro_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = SPI5_SCK_Pin | SPI5_MISO_Pin | SPI5_MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin =  GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

	SPI_InitTypeDef SPI_InitStruct = {0};

	HSPI.Instance = SPI5;
	SPI_InitStruct.Mode = SPI_MODE_MASTER;
	SPI_InitStruct.Direction = SPI_DIRECTION_2LINES;
	SPI_InitStruct.DataSize = SPI_DATASIZE_8BIT;
	SPI_InitStruct.CLKPolarity = SPI_POLARITY_HIGH;
	SPI_InitStruct.CLKPhase = SPI_PHASE_2EDGE;
	SPI_InitStruct.NSS = SPI_NSS_SOFT;
	SPI_InitStruct.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	SPI_InitStruct.FirstBit = SPI_FIRSTBIT_MSB;
	SPI_InitStruct.TIMode = SPI_TIMODE_DISABLE;
	SPI_InitStruct.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SPI_InitStruct.CRCPolynomial = 0;

	HSPI.Init = SPI_InitStruct;
	HAL_SPI_Init(&HSPI);
}

//void Gyro_GetID(void){
//	uint8_t foo = Gyro_Read(GYRO_WHO_AM_I);
//	printf("Device ID is %x \n", foo);
//}

void Gyro_Power(void){
	uint8_t temp = Gyro_Read(GYRO_CTRL_REG1);
	temp |= (1 << 3);
	Gyro_Write(GYRO_CTRL_REG1, temp);
	}

//void Gyro_GetTemp(void){
//	uint8_t foo = Gyro_Read(GYRO_OUT_TEMP);
//	printf("Temperature: %d Celsius degrees. \n", foo);
//}

void Gyro_Configure(void){
	Gyro_Write(GYRO_CTRL_REG1,POWER_EN_AND_XYZ_EN);
	Gyro_Write(GYRO_CTRL_REG4,FIVE_HUNDRED_DPS);
	Gyro_Write(GYRO_CTRL_REG5,FIFO_EN_REBOOT_EN);
	Gyro_Write(GYRO_FIFO_CTRL_REG,STREAM_MODE);
}

uint8_t Gyro_Read(uint8_t register_adddress){
	uint8_t sent_command = (0xc0 | register_adddress);
	uint16_t received_data = 0x00;
	Gyro_Enable_Slave();
	while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1) != GPIO_PIN_RESET);
	HAL_status = HAL_SPI_TransmitReceive(&HSPI, &sent_command,(uint8_t*) &received_data, 2, TESTING_TIMEOUT);
	received_data = (0xFF00 & received_data) >> 8;
	Gyro_Disable_Slave();
	Gyro_SPI_Check(); // to check communication worked correctly
	return received_data;
}

void Gyro_Write(uint8_t register_address, uint8_t value){
	uint8_t sent_command = (G_WRITE_BIT | register_address)|(value << 8);
	Gyro_Enable_Slave();
	while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1) != GPIO_PIN_RESET);
	HAL_status = HAL_SPI_Transmit(&HSPI, &sent_command, 2, TESTING_TIMEOUT);
	Gyro_Disable_Slave();
	Gyro_SPI_Check();
}


void Gyro_SPI_Check(void){
	if(HAL_status != HAL_OK){
		for(;;);
	}
}

void Gyro_Enable_Slave(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
}

void Gyro_Disable_Slave(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
}

int16_t Gyro_Get_Velocity() {
	uint8_t y_l = Gyro_Read(GYRO_OUT_Y_L);
	uint8_t y_h = Gyro_Read(GYRO_OUT_Y_H);

	int16_t y_val = (int16_t)((y_h << 8) | y_l);
	return y_val;
}

void Gyro_Sample() {
	int16_t gyro_val = Gyro_Get_Velocity();
	MoveCoin_Gyro(gyro_val);
}




