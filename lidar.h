#ifndef __LIDAR_H
#define __LIDAR_H
#include <stdint.h>
#include <stdio.h>
#include <lpc22xx.h>
#include "I2C_COMMUNICATION.h"
#include "DEBUG_HEADER.h"

#define LIDAR_ADDRESS 0x62

/***********************************************************************************
Those functions are implemented in order to get data from the LIDAR sensor.
************************************************************************************/

void Lidar_config(uint8_t config, uint8_t bias);
uint16_t read_distance(void);

#endif
