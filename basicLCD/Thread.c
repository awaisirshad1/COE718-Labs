#include <time.h>
#include "LPC17xx.h"
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "GLCD.h"
#include "LED.h"

void delay_ms(unsigned int milli);

void clearFromOtherFunction(void);

void clearFromOtherFunction(void){
	GLCD_ClearLn(2,1);
	delay_ms(3000);
	GLCD_DisplayString(2,0,1,"after 3rd delay");
}


void delay_ms(unsigned int milli){
	//unsigned int milliseconds = 1000*milli;
	clock_t start_time = clock();
	while(clock() < start_time + milli);
}