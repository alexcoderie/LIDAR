#include "TIMER.h"

// pclk = 60 000 000 / 4 = 15 000 000 hz => 15 000 000 clk = 1s => 15 000 clk = 1ms

static int state = 0;
int x = 0;

/***********************************************************************************
To add description
************************************************************************************/

void T0isr (void) __irq
{
	state=!state;
	if(state == 1)
	{
		IO0SET = (1 << 30);
	}	
	else
	{
		IO0CLR = (1 << 30);
	}
	T0IR |= 0x00000001;
	VICVectAddr = 0x00000000;
}

/***********************************************************************************
To add description
************************************************************************************/

void initTimer()
{
	PINSEL0 |= 0x00000800;
  T0PR = 0x0000001E;
	T0TCR = 0x00000002;
	T0MCR = 0x00000003;
	T0MR0 = 0x00001000;
	T0TCR = 0x00000001;
	VICIntEnable |= 0x00000010;
	VICIntSelect |= 0x00000010;
}

/***********************************************************************************
To add description
************************************************************************************/

void delayMS(unsigned int milliseconds)
{
	T0TCR = 0x02; 														/* Reset Timer */
	T0TCR = 0x01; 														/* Enable timer */
	while(T0TC < milliseconds); /* Wait until timer counter reaches the desired delay */
	T0TCR = 0x00; 							/* Disable timer */
}
