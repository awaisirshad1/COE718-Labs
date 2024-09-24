#include <stdio.h>
#include "LPC17xx.h"                    // Device header
//#include "KBD.h"
//#include "GLCD.h"


#define __FI        1                      /* Font index 16x24               */
//#define __USE_LCD   0										/* Uncomment to use the LCD */


//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
/////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void){

	uint16_t ledForBitband = 3U;
	uint16_t ledForConditional = 7U;
	uint16_t ledForBarrelShifting = 0U;
	
	printf("LPC_GPIO_BASE: 0x%lX", LPC_GPIO_BASE);
	
	
#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Lab 2   ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       lab_assignment.c     ");
  GLCD_DisplayString(2, 0, __FI, "    Use joystick   ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Black);
  GLCD_DisplayString(4, 0, __FI, " direction: ");
#endif


	
	
	return 0;
}