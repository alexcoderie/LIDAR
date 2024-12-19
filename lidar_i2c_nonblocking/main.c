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

static uint8_t status = 0;

volatile int cntOperation = 0, cntWrite = 0, cntRead = 0, cntIndex = 0;
volatile int doneInit = 0;
uint8_t regDat[] = {0x00, 0x04};
uint8_t lenWrite = 0, lenRead = 1;
uint8_t measurements[2];
uint16_t results;


void pollingWrite(uint8_t adresa, uint8_t valoare)
{
	uint8_t stat;
	stat = I2STAT;
	switch(stat)
	{
		case 0x08: // start
		{
			cntOperation = 0;
			I2DAT = 0xC4;
			I2CONCLR = 0x20;
			I2CONSET = 0x04;
			break;
		}
		case 0x18: // enter data ACK received
		{
			cntOperation++;
			I2DAT = adresa;
			break;
		}
		case 0x20: // enter data NACK received
		{
			I2CONSET = 0x14;
			break;
		}
		case 0x28: // data sent with ACK
		{
			cntOperation++;
			I2DAT = valoare;
			if(cntOperation == 3)
			{
				I2CONSET = 0x20;
				if(cntWrite < lenWrite)
				{
					cntWrite++;
				}
				else
				{
					doneInit = 1;
				}
			}		
			break;
		}
		case 0x30: // data sent with NACK
		{
			I2CONSET = 0x14;
			break;
		}
		default:
		{
			break;
		}
	}
	if (stat != 0xF8)
	{
		I2CONCLR = 0x08;
	}
}

void pollingReadStatus(uint8_t adresa)
{
	uint8_t stat;
	stat = I2STAT;
	switch(stat)
	{
		case 0x08: // start
		{
			I2DAT = 0xC4;
			I2CONCLR = 0x20;
			I2CONSET = 0x04;
			break;
		}
		case 0x10: // repeated start
		{
			I2DAT = 0xC5;
			I2CONCLR = 0x20;
			I2CONSET = 0x04;
			break;
		}
		case 0x18: // enter data ACK received
		{
			I2DAT = adresa;
			break;
		}
		case 0x20: // enter data NACK received
		{
			I2CONSET = 0x14;
			break;
		}
		case 0x28: // data sent with ACK
		{
			I2CONSET = 0x20;
			break;
		}
		case 0x30: // data sent with NACK
		{
			I2CONSET = 0x14;
			break;
		}
		case 0x40: // address received with ACK
		{
			I2CONCLR = 0x3C; // genereaza data + NACK
			break;
		}
		case 0x48: //  address received with NACK
		{
			I2CONSET = 0x20;
			break;
		}
		case 0x50: // data received with ACK
		{
			break; 
		}
		case 0x58: // data received with NACK
		{
			status = I2DAT;
			cntRead++;
			I2CONSET = 0x14;
			break;
		}
		default:
		{
			break;
		}
	}
	if (stat != 0xF8)
	{
		I2CONCLR = 0x08;
	}
}

void pollingRead(uint8_t adresa)
{
	uint8_t stat;
	stat = I2STAT;
	switch(stat)
	{
		case 0x08: // start
		{
			I2DAT = 0xC4;
			I2CONCLR = 0x20;
			I2CONSET = 0x04;
			break;
		}
		case 0x10: // repeated start
		{
			I2DAT = 0xC5;
			I2CONCLR = 0x20;
			I2CONSET = 0x04;
			break;
		}
		case 0x18: // enter data ACK received
		{
			I2DAT = adresa;
			break;
		}
		case 0x20: // enter data NACK received
		{
			I2CONSET = 0x14;
			break;
		}
		case 0x28: // data sent with ACK
		{
			I2CONSET = 0x20;
			break;
		}
		case 0x30: // data sent with NACK
		{
			I2CONSET = 0x14;
			break;
		}
		case 0x40: // address received with ACK
		{
			I2CONSET = 0x04; // genereaza data + ACK
			break;
		}
		case 0x48: //  address received with NACK
		{
			I2CONSET = 0x20;
			break;
		}
		case 0x50: // data received with ACK
		{
			measurements[cntIndex++] = I2DAT;
			cntRead++;
			if((cntRead - 1) == lenRead)
			{
				I2CONCLR = 0x3C; // genereaza data + NACK
			}
			break; 
		}
		case 0x58: // data received with NACK
		{
			measurements[cntIndex++] = I2DAT;
			I2CONSET = 0x14;
			cntRead++;
			cntIndex = 0;
			break;
		}
		default:
		{
			break;
		}
	}
	if (stat != 0xF8)
	{
		I2CONCLR = 0x08;
	}
}

void processMeasurements(uint8_t *measurements, uint16_t *result)
{
	*result = ((uint16_t)(measurements[0]) << 8) | ((uint16_t)(measurements[1]));
}

void resetI2C(void)
{
	I2CONCLR = 0x000000FF;
	I2CONSET = 0x00000040;
	I2CONSET = 0x00000020;
}


void startSensor(void)
{
	resetI2C();
}

void irq_handler (void) __irq
{
	if(doneInit == 0) // initializare
	{
		pollingWrite(0x00, 0x04); // pas 1
	}
	else // citire
	{
		if(cntRead == 0) // pas 2
		{
			pollingReadStatus(0x01);
			cntRead++;
		}
		else
		{
			if(status == 0x01) // reiau pasul 1 si 2
			{
				doneInit = 0;
				cntRead = 0;
			}
			else // pot citi
			{
				if((cntRead - 1) < lenRead)
				{
					pollingRead(0x14);
				}
				else {
					
					processMeasurements(measurements, &results);
					printf ("Distance in hex: %04X\n",results );
					printf ("Distance in dec: %d\n",results );
					cntRead = 0;
					startSensor();
				}
			}
		}
	}
	T0IR |= 1;
	VICVectAddr = 0x00000000;
}


int main(void)
{	
	startSensor();
	setup_function();
	//Lidar_config(1,0);
	
	while(1)
	{
		
//		distance_lidar_hex = read_distance();
//		if(DEBUG_MODE == 1)
//		{
//			printf ("Distance in hex: %04X\n",distance_lidar_hex );
//			printf ("Distance in dec: %d\n",distance_lidar_hex );
//		}
//		for (i = 0; i < 145760; i++);	
	}
	return 0;
}

