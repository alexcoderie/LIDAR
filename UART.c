#include "UART.h"

void initUART(void) {
	PINSEL0 |= UART0_TXD;
	PINSEL0 |= UART0_RXD;
	U0LCR 	 = UART0_8BIT | UART0_1STOP | UART0_DLAB_ENABLE;
	U0LCR 	&= UART0_PARITY_DISABLE;
	U0DLL		 = UART0_9600_BAUD;
	U0DLM		 = 0;
	U0LCR 	&= UART0_DLAB_DISABLE;
}

uint8_t U0Write(char ch) {
	while ( !(U0LSR & THRE ) );
	U0THR = ch;
	return ch;
}

char U0Read() {
	while( !(U0LSR & RDR ) );
	return (U0RBR);
}