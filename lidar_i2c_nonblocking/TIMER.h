#ifndef __TIMER_H
#define __TIMER_H

#include <lpc22xx.h>
#include "DEBUG_HEADER.h"
#include <stdio.h>
#include <stdint.h>

void initTimer(void);
void delayMS(unsigned int milliseconds);

#endif