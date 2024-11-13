/*
Main file
- controls all peripherals and core clock configuration.
- controls page number to navigate menu and select different media
- 
*/

#include <stdio.h>
#include "LPC17xx.h" 
#include "core_cm3.h"
#include "GPIO_LPC17xx.h"
#include "Board_Joystick.h"
#include "GLCD.h"
#include "KBD.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;

unsigned int page; 
uint32_t joyStick_value;
uint32_t joyStickPrev = 0U;


void DisplayMainMenu(void);

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	
	//Joystick_Initialize();
	
	KBD_Init();
	
	GLCD_Init();                               
	DisplayMainMenu();
  
	//SystemCoreClockUpdate();
  //SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */
	// uses the system timre to generate interrupts specified by the ticks, reloads the timer upon interrupts using a logic OR, accounts for interrupt priority
	// The SysTick_Config function initializes the system timer to generate periodic interrupts at a specified tick interval. It configures the timer's reload 
	// value, sets the interrupt priority, and enables the timer, returning 0 on success or 1 if the tick value is invalid.
	
	
	
}

void DisplayMainMenu(void){
	GLCD_Clear(White);                         
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, ( unsigned char *)"COE718 Media Centre ");
  GLCD_DisplayString(1, 0, __FI, ( unsigned char *)"   Navigate Menu:   ");
	
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Black);
	GLCD_DisplayString(2, 0, __FI, ( unsigned char *)"--------------------");
  GLCD_DisplayString(3, 0, __FI, ( unsigned char *)"|      Gallery     |");
	GLCD_DisplayString(4, 0, __FI, ( unsigned char *)"--------------------");
	GLCD_DisplayString(5, 0, __FI, ( unsigned char *)"|     MP3 Player   |");
	GLCD_DisplayString(6, 0, __FI, ( unsigned char *)"--------------------");
	GLCD_DisplayString(7, 0, __FI, ( unsigned char *)"|       Games      |");
	GLCD_DisplayString(8, 0, __FI, ( unsigned char *)"--------------------");
	GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
	GLCD_DisplayString(9, 0, __FI, ( unsigned char *)"         \\/         ");
}
