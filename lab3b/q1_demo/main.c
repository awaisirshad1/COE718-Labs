/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "LPC17xx.h"
#define GPIO_PORT1_LED31 (*((volatile unsigned long *)0x233806FC))
	
extern int Init_Thread (void);
 
 void turnOnLed2(unsigned int ledNum){
	switch(ledNum){
		case 28:
			LPC_GPIO1 -> FIOPIN |= (1<<28);
		case 29:
			LPC_GPIO1 -> FIOPIN |= (1<<29);
		case 2:
			LPC_GPIO2 -> FIOPIN |= (1<<2);
		case 4:
			LPC_GPIO2 -> FIOPIN |= (1<<4);
		case 6:
			LPC_GPIO2 -> FIOPIN |= (1<<6);	
	}
}

void turnOffAllLeds2(void){
	LPC_GPIO1 -> FIOPIN &= ~(1 << 28);
	LPC_GPIO1 -> FIOPIN &= ~(1 << 29);
	LPC_GPIO2 -> FIOPIN &= ~(1 << 2);
	LPC_GPIO2 -> FIOPIN &= ~(1 << 4);
	LPC_GPIO2 -> FIOPIN &= ~(1 << 6);
}
/*
 * main: initialize and start the system
 */
int main (void) {
	osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	turnOffAllLeds2();
	turnOnLed2(29);
  GPIO_PORT1_LED31 = 0;
 	//Init_Thread ();
  //osKernelStart ();                         // start thread execution 
	//osDelay(osWaitForever);
}


