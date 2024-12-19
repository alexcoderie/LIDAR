#include <stdio.h>
#include <stdint.h>
#include <lpc22xx.h>
#include "UART.h"
#include "TIMER.h"
#include "I2C_COMMUNICATION.h"
#include "Lidar.h"
#include "DEBUG_HEADER.h"
#include <math.h>

/***********************************************************************************
Global variables
************************************************************************************/

volatile int i;
uint16_t distance_lidar_hex;

/***********************************************************************************
This function is executed 1 time at power on or at reset.
************************************************************************************/

void setup_function(void)
{
	initUART();
	initTimer();
	I2C_init();
}

/***********************************************************************************
This function is executed 1 time at power on or at reset.
************************************************************************************/

void blink_led ()
{
		//reg for setting the function of the pin p0.30
		//PINSEL1 &= (1 << 28);
		//PINSEL1 &= (1 << 29);
		//IO0DIR |= 1 << 30; // P0.30 LED
		//Setup for a led to blink
		//IO0SET = 1 << 30;
		//delayMS(1000);
		//IO0CLR = 1 << 30;
		//delayMS(1000);
		//UART echo
		//char c = U0Read();
		//U0Write(c);
		//continue;
}

/***********************************************************************************
Main program
************************************************************************************/

int main(void)
{	
	setup_function();
	Lidar_config(1,0);
	
	while(1)
	{
		
		distance_lidar_hex = read_distance();
		if(DEBUG_MODE == 1)
		{
			printf ("Distance in hex: %04X\n",distance_lidar_hex );
			printf ("Distance in dec: %d\n",distance_lidar_hex );
		}
		for (i = 0; i < 5000000; i++);	
	}
	return 0;
}

