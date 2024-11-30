/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: Thread initialization upon system startup

 *----------------------------------------------------------------------------
 * Name: Awais Irshad
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "main.h"
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h"
#include "Board_ADC.h" 
#include "KBD.h"
#include "mainMenu.h"
#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;

//void displayMainMenu(void);
extern int Init_Thread (void);

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
// initialize peripherals
  LED_Init();                                /* LED Initialization            */
  ADC_Initialize();                                /* ADC Initialization            */
	KBD_Init();

#ifdef __USE_LCD	
  GLCD_Init();                               // Initialize graphical LCD (if enabled 
#endif
	osKernelInitialize();
	// initialize our threads, the media center will run from here
	
	Init_Thread();
	osKernelStart();
	osDelay(osWaitForever);
	
}

