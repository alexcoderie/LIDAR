#ifndef __I2C_COMMUNICATION_H
#define __I2C_COMMUNICATION_H
#include <lpc22xx.h>
#include <stdio.h>
#include <stdint.h>
#include "DEBUG_HEADER.h"

#define LIDAR_ADDRESS 0x62

/***********************************************************************************
To add description
************************************************************************************/

void I2C_init(void);

void Lidar_start_measurement(void);
void Lidar_stop_measurement(void);

uint8_t wait_for_response(uint8_t code);

uint8_t LidarWrite_8bits(uint8_t address, uint8_t data, uint8_t numBytes);
uint8_t LidarRead_8bits(uint8_t address,uint8_t numBytes);

#endif
