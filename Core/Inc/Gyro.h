/*
 * Gyro.h
 *
 *  Created on: Apr 1, 2025
 *      Author: user
 */

#ifndef INC_GYRO_H_
#define INC_GYRO_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdbool.h>
#include "ApplicationCode.h"

#define GYRO_WHO_AM_I  0x0F
#define GYRO_CTRL_REG1 0x20
#define GYRO_CTRL_REG2 0x21
#define GYRO_CTRL_REG3 0x22
#define GYRO_CTRL_REG4 0x23
#define GYRO_CTRL_REG5 0x24
#define GYRO_REFERENCE_DATA_CAPTURE 0x25
#define GYRO_OUT_TEMP  		0x26
#define GYRO_STATUS_REG 	0x27
#define GYRO_OUT_X_L		0x28
#define GYRO_OUT_X_H		0x29
#define GYRO_OUT_Y_L		0x2A
#define GYRO_OUT_Y_H		0x2B
#define GYRO_OUT_Z_L		0x2C
#define GYRO_OUT_Z_H		0x2D
#define GYRO_FIFO_CTRL_REG	0x2E
#define GYRO_FIFO_SRC_REG	0x2F
#define GYRO_INT1_CFG		0x30
#define GYRO_INT1_SRC		0x31
#define GYRO_INT1_THS_XH	0x32
#define GYRO_INT1_THS_XL	0x33
#define GYRO_INT1_THS_YH	0x34
#define GYRO_INT1_THS_YL	0x35
#define GYRO_INT1_THS_ZH	0x36
#define GYRO_INT1_THS_ZL	0x37
#define GYRO_INT1_DURATION	0x38

// Magic Numbers
// CTRL_REG_1 REG
#define POWER_EN_AND_XYZ_EN 0xF
//#define POWER_EN_AND_XYZ_EN 0x8

// CTRL_REG_4 REG
#define FIVE_HUNDRED_DPS 0x10

// CTRL_REG_5 REG
#define FIFO_EN_OFFSET 0x6
#define BOOT_OFFSET 0x7
#define REBOOT_MEMORY_CONTENT 0x1
#define FIFO_EN_REBOOT_EN 0xC0

// FIFO_CTRL_REG
#define STREAM_MODE 0x40

#define TESTING_TIMEOUT 2000
#define G_WRITE_BIT (0 << 7)
#define G_READ_BIT (1 << 7)


// Macros for the Pin and Port information for the Gyro
#define GYRO_PIN_Vdd_IO 	 1
#define GYRO_PIN_SCL_SPC 	 2
#define GYRO_PIN_SDA_SDI_SDO 3
#define GYRO_PIN_SDO_SA0 	 4
#define GYRO_PIN_CS			 5
#define GYRO_PIN_DRDY_INT2   6
#define GYRO_PIN_INT1        7
#define GYRO_PIN_GND		 13
#define GYRO_PIN_PLLFILT	 14
#define GYRO_PIN_Vdd		 16

#define SPI5_SCK_Pin GPIO_PIN_7
#define SPI5_MISO_Pin GPIO_PIN_8
#define SPI5_MOSI_Pin GPIO_PIN_9

// A prototype to Initialize the Gyro
void Gyro_Init(void);

// A prototype to get the device ID and print it
void Gyro_GetID(void);

// A prototype to power the Gyro
 void Gyro_Power(void);

// A prototype to get the temperature and print it
void Gyro_GetTemp(void);

// A prototype to configure the registers on the Gyro
void Gyro_Configure(void);

// A prototype to read the registers of the Gyro
uint8_t Gyro_Read(uint8_t register_address);

// A prototype to verify the HAL status for the SPI is okay
void Gyro_SPI_Check(void);

// A prototype to manually enable slave communication.
void Gyro_Enable_Slave(void);

// A prototype to manually disable slave communication.
void Gyro_Disable_Slave(void);

void Gyro_Write(uint8_t register_address, uint8_t value);

int16_t Gyro_Get_Velocity();

void Gyro_Sample();


#endif /* INC_GYRO_H_ */
