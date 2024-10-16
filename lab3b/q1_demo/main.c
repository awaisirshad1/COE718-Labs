/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "LPC17xx.h"
#include "LED.h"
	
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
	/*
	LPC_GPIO1 -> FIOPIN &= ~(1 << 28);
	LPC_GPIO1 -> FIOPIN &= ~(1 << 29);
	LPC_GPIO2 -> FIOPIN &= ~(1 << 2);
	LPC_GPIO2 -> FIOPIN &= ~(1 << 4);
	LPC_GPIO2 -> FIOPIN &= ~(1 << 6);
	*/
	LED_Off(28);
	LED_Off(29);
	LED_Off(31);
	LED_Off(2);
	LED_Off(3);
	LED_Off(4);
	LED_Off(5);
	LED_Off(6);
	
}
/*
 * main: initialize and start the system
 */
int main (void) {
	LED_Init();
	osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	turnOffAllLeds2();
	//LED_On(2);
	//turnOnLed2(29);
  
 	Init_Thread ();
  osKernelStart ();                         // start thread execution 
	osDelay(osWaitForever);
}


