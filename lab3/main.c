/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h"
 
extern int Init_Thread (void);
 
#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

/*
 * main: initialize and start the system
 */
int main (void) {
	
	// initialize LED and LCD
	LED_Init();                                /* LED Initialization            */
	
//s#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Lab 3a   ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       question1.c     ");
  //GLCD_DisplayString(2, 0, __FI, "       ");
  GLCD_SetBackColor(White);
//#endif
	osKernelInitialize ();                    // initialize CMSIS-RTOS
 	Init_Thread ();
  osKernelStart ();                         // start thread execution 
	osDelay(osWaitForever);
}
