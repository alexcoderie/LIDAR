#ifndef __UART_H
#define __UART_H

#include <lpc22xx.h>
#include <stdint.h>
#include <stdio.h>
#include "DEBUG_HEADER.h"


//at 14.7456 pclk the divizor value for 9600 baud rate is 96

#define UART0_TXD							0x00000001
#define UART0_RXD							0x00000004
#define UART0_8BIT						0x00000003
#define UART0_1STOP						0x00000000
#define UART0_PARITY_DISABLE	~(1 << 3)
#define UART0_DLAB_ENABLE			0x00000080
#define UART0_9600_BAUD				96
#define UART0_DLAB_DISABLE		~(1 << 7)

#define THRE	0x00000020
#define RDR		0x00000001

#define CR		((unsigned char) 0x00001101)

void initUART(void);
uint8_t U0Write(char ch);
char U0Read(void);

#endif
