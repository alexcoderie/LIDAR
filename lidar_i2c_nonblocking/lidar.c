#include "lidar.h"

/***********************************************************************************
This function is used to configure the LIDAR.
Parameters: 
uint8_t config
Case config = 0 : Default mode, balanced performance.
Case config = 1 : Short range, high speed.
Case config = 2 : Default range, higher speed short range.
Case config = 3 : Maximum range.
Case config = 4 : High sensitivity detection, high erroneous measurements.
Case config = 5 : Low sensitivity detection, low erroneous measurements.
Case config = 6 : Short range, high speed, higher error.
uint8_t bias
Case bias = 0 : Take distance deasurement with receiver bias correction
Case bias = 1 : Take distance deasurement without receiver bias correction
************************************************************************************/

void Lidar_config(uint8_t config, uint8_t bias)
{
	uint8_t sigCountMax;
  uint8_t acqConfigReg;
  uint8_t refCountMax;
  uint8_t thresholdBypass;
	switch(bias)
	{
		case 0:
					LidarWrite_8bits(0x00,0x04,1);
		break;
			
		case 1:
					LidarWrite_8bits(0x00,0x03,1);
		break;
		
		default:
					LidarWrite_8bits(0x00,0x04,1);
		break;
	}
  switch (config)
  {
		case 0: 
					sigCountMax     = 0x80;
					acqConfigReg    = 0x08;
					refCountMax     = 0x05;
					thresholdBypass = 0x00;
		break;
	
		case 1: 
					sigCountMax     = 0x1d;
					acqConfigReg    = 0x08;
					refCountMax     = 0x03;
					thresholdBypass = 0x00;
		break;

		case 2:
					sigCountMax     = 0x80;
					acqConfigReg    = 0x00;
					refCountMax     = 0x03;
					thresholdBypass = 0x00;
		break;

		case 3: 
					sigCountMax     = 0xff;
					acqConfigReg    = 0x08;
					refCountMax     = 0x05;
					thresholdBypass = 0x00;
		break;

		case 4: 
					sigCountMax     = 0x80;
					acqConfigReg    = 0x08;
					refCountMax     = 0x05;
					thresholdBypass = 0x80;
		break;

		case 5: 
					sigCountMax     = 0x80;
					acqConfigReg    = 0x08;
					refCountMax     = 0x05;
					thresholdBypass = 0xb0;
		break;

		case 6: 
					sigCountMax     = 0x04;
					acqConfigReg    = 0x01;
					refCountMax     = 0x03;
					thresholdBypass = 0x00;
		break;
		
		default:
					sigCountMax     = 0x80;
					acqConfigReg    = 0x08;
					refCountMax     = 0x05;
					thresholdBypass = 0x00;
		break;
  }

	LidarWrite_8bits(0x02, sigCountMax,1);
	LidarWrite_8bits(0x04, acqConfigReg,1);
	LidarWrite_8bits(0x12, refCountMax,1);
	LidarWrite_8bits(0x1c, thresholdBypass,1);
}

/***********************************************************************************
This function is used to return the measured distance by LIDAR.
Return value: the value of distance on 2 bytes.
************************************************************************************/

uint16_t read_distance()
{
	uint8_t lidar_data;
	uint8_t lidar_data_low;
	uint8_t lidar_data_high;
	uint8_t lidar_address;
	uint16_t lidar_response;
	
	/* Write 0x04 in reg 0x00*/
	lidar_address = 0x00;
	lidar_data = 0x04;
	lidar_response = LidarWrite_8bits(lidar_address,lidar_data,1);
		
	lidar_address = 0x01;
	while(LidarRead_8bits(lidar_address,1)==0x01);
		
	lidar_address = 0x14;
	lidar_data_high = LidarRead_8bits(lidar_address,1);
	if(DEBUG_MODE == 1)
		printf ("High data: %02X\n",lidar_data_high );
		
	lidar_address = 0x15;
	lidar_data_low = LidarRead_8bits(lidar_address,1);
	if(DEBUG_MODE == 1)
		printf ("Low data: %02X\n",lidar_data_low );
		
	/* Compute distance */
	lidar_response = lidar_data_high;
	lidar_response = lidar_response << 8 ;
	lidar_response += lidar_data_low;
	
	return lidar_response;
}
