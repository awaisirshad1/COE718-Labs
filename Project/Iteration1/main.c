/*
Main file
- controls all peripherals and core clock configuration.
- controls page number to navigate menu and select different media
- 
*/

#include <stdio.h>
#include "LPC17xx.h" 
#include "GPIO_LPC17xx.h"
#include "Board_Joystick.h"
#include "Board_GLCD.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;

unsigned int page; 
uint32_t joyStick_value;
uint32_t joyStickPrev = 0U;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	GLCD_Initialize();
	Joystick_Initialize();
	GLCD_SetBackgroundColor(Red);
	//KBD_Init();
	/*
	GLCD_Init();                               

  GLCD_Clear(White);                         
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 3, __FI, ( unsigned char *)"COE718 Media Centre");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 3, __FI, ( unsigned char *)"Navigate Menu:");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Black);
  GLCD_DisplayString(2, 3, __FI, ( unsigned char *)"Gallery");
	GLCD_DisplayString(3, 3, __FI, ( unsigned char *)"Music");
	GLCD_DisplayString(4, 3, __FI, ( unsigned char *)"Games");
	*/
}
