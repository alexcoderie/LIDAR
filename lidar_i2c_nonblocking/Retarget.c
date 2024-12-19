#include <stdio.h>
#include <rt_misc.h>
#include "UART.h"
#pragma import(__use_no_semihosting_swi)


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;


int fputc(int ch, FILE *f) {
  return (U0Write(ch));
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  U0Write(ch);
}


void _sys_exit(int return_code) {
//label:  goto label;  /* endless loop */
	return;
}
