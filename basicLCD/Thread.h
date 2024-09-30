#include <time.h>
#include "LPC17xx.h"
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "GLCD.h"
#include "LED.h"

void delay_ms(unsigned int milli);

void clearFromOtherFunction(void);