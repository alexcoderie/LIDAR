#include "I2C_COMMUNICATION.h"

/***********************************************************************************
To add description
************************************************************************************/

void I2C_init()
{
	PINSEL0 |= 0x00000050;
	I2CONSET = 0x40;
	I2SCLH = 0x28;
	I2SCLL = 0x28;
}

/***********************************************************************************
This function is used to initiate a distance measurement.
************************************************************************************/

void Lidar_start_condition()
{
	I2CONSET |= 1 << 5;	
	while (I2STAT != 8);	/* Wait for ack */
	I2CONCLR=0x28;	/* Address shift and write */
	I2DAT = (LIDAR_ADDRESS << 1);		
	while (I2STAT != 0x18); 	/* Wait for ack */
} 

/***********************************************************************************
This function is used to stop the distance measurement.
************************************************************************************/

void Lidar_stop_condition()
{
	I2CONSET=0x10;
	I2CONCLR=0x8;
} 

/***********************************************************************************
To add description
************************************************************************************/

uint8_t LidarWrite_8bits(uint8_t address, uint8_t data,uint8_t numBytes)
{
	Lidar_start_condition();
	
	I2DAT=address;
	I2CONCLR=0x8;
	
	/* Wait for ack */
	if(wait_for_response(0x28)==0)
	{
		I2CONCLR=0x28;
		I2DAT = data;		
		if(wait_for_response(0x28)==0)
		{
			I2CONCLR=0x28;
		}	
		else
		{
			if(DEBUG_MODE == 1)
				printf ("Ack2 in write not received." );
		}
		
	}
	else
	{
		if(DEBUG_MODE == 1)
			printf ("Ack1 in write not received." );
	}	
	Lidar_stop_condition();
	
	return 0x00;
}

/***********************************************************************************
To add description
************************************************************************************/

uint8_t LidarRead_8bits(uint8_t address,uint8_t numBytes)
{
	uint8_t data_read;
	
	Lidar_start_condition(); 
	
	I2DAT=address;
	I2CONCLR=0x8;
	
	/* Wait for ack */
	if(wait_for_response(0x28)==0)
	{
	
		/* Stop */
		I2CONSET=0x10;
		I2CONCLR=0x8;
		
		/* Start */
		I2CONSET |= 1 << 5;
		
		/* Wait for ack */
		if(wait_for_response(0x08)==0)
		{
				/* Read from address*/
				I2CONCLR=0x28;
				I2DAT = (LIDAR_ADDRESS << 1) | 1;		
				
				/* Wait for ack */
			if(wait_for_response(0x40)==0)
			{
				I2CONCLR=0x28;
				
				/* Wait for ack */
				if(wait_for_response(0x58)==0)
				{
				/* Save data */	
					data_read = I2DAT;
				}
				else
				{
				if(DEBUG_MODE == 1)
					printf ("Ack4 in read not received." );
				}
			}
			else
			{
				if(DEBUG_MODE == 1)
					printf ("Ack3 in read not received." );
			}
		}
		else
		{
			if(DEBUG_MODE == 1)
				printf ("Ack2 in read not received." );
		}
	}
	else
	{
		if(DEBUG_MODE == 1)
			printf ("Ack1 in read not received." );
	}
	Lidar_stop_condition();
	
	return data_read;
}

/***********************************************************************************
This function is used to wait for the ack. 
Parameter: code - the expected ack code.
Return : 
Response_code = 0 : akc received.
Response_code = 1 : akc not received.
************************************************************************************/

uint8_t wait_for_response(uint8_t code)
{
	int i=0;
	uint8_t response_code=0x00; 
	while(i<1000000 && I2STAT != code)
	{
		i++;
	}
	if(i>1000000)
		response_code = 0x01;
	
	return response_code;
}
