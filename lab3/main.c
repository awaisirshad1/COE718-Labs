/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */
#define __USE_LED 0

#include "osObjects.h"                      // RTOS object definitions
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h"
#include <stdio.h>
	
extern int Init_Thread (void);
 


void threadsFinished(void);

/*
 * main: initialize and start the system
 */
int main (void) {
	
	// initialize LED and LCD
#ifdef __USE_LED
	LED_Init();                                /* LED Initialization            */
#endif
#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, (unsigned char *)"     COE718 Lab 3a   ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, (unsigned char *)"       question1.c     ");
  GLCD_SetBackColor(White);
#endif
	osKernelInitialize ();                    // initialize CMSIS-RTOS
 	Init_Thread ();
  osKernelStart ();                         // start thread execution 
	osDelay(osWaitForever);
	threadsFinished();
}


extern void threadsFinished(void){
#ifdef __USE_LCD
	GLCD_ClearLn(5,1);
	GLCD_ClearLn(6,1);
	
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(4,  0, __FI,  (unsigned char *)"Threads Finished");
	GLCD_DisplayString(5,  0, __FI,  (unsigned char *)"Exit");
#endif 
#ifndef __USE_LCD
	char str[] = "threads finished, exiting";
	printf("%s\n", str);
#endif
}