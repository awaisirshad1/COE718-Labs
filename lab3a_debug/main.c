/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
 
extern int Init_Thread (void);
 
#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

/*
 * main: initialize and start the system
 */
int main (void) {
	
	osKernelInitialize ();                    // initialize CMSIS-RTOS
 	Init_Thread ();
  osKernelStart ();                         // start thread execution 
	osDelay(osWaitForever);
}