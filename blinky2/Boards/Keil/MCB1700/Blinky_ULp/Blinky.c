/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): __USE_LCD   - enable Output on LCD, uncomment #define in code to use
 *  				for demo (NOT for analysis purposes)
 *----------------------------------------------------------------------------
 * Copyright (c) 2008-2011 Keil - An ARM Company.
 * Name: Anita Tino
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "Blinky.h"
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h"
#include "Board_ADC.h" 
#include "KBD.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

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

char text[10];
char text_l[10];

static volatile uint16_t AD_dbg;
static volatile uint32_t JS_dbg;

uint16_t ADC_last;                      // Last converted value
/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  /**int32_t  res;
  uint32_t AD_sum   = 0U;
  uint32_t AD_cnt   = 0U;
  uint32_t AD_value = 0U;
  uint32_t AD_print = 0U;**/
	uint32_t joystick_motion;

  LED_Init();                                /* LED Initialization            */
  ADC_Initialize();                                /* ADC Initialization            */
	KBD_Init();

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Purple);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Lab 1   ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       Blinky.c     ");
  GLCD_DisplayString(2, 0, __FI, "    Move Joystick   ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(5, 0, __FI, " Joystick Direction: ");
#endif

  //SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */

    /* AD converter input                                                     */
    // AD converter input
    /*res = ADC_GetValue();
    if (res != -1) {                     // If conversion has finished
      ADC_last = (uint16_t)res;
      
      AD_sum += ADC_last;                // Add AD value to sum
      if (++AD_cnt == 16U) {             // average over 16 values
        AD_cnt = 0U;
        AD_value = AD_sum >> 4;          // average devided by 16
        AD_sum = 0U;
      }
    }

    if (AD_value != AD_print) {
      AD_print = AD_value;               // Get unscaled value for printout
      AD_dbg   = (uint16_t)AD_value;

      sprintf(text, "0x%04X", AD_value); // format text for print out*/
			
		    
		joystick_motion = get_button(); 
		//joystick_motion = JS_dbg;	
		sprintf(text, "0x%08X", joystick_motion); // format text for print out*/
     //GLCD_SetTextColor(Red);
    //GLCD_DisplayString(6,  9, __FI,  (unsigned char *)text);
			//GLCD_SetTextColor(Green);
    //  GLCD_Bargraph (144, 7*24, 176, 20, (AD_value >> 2)); /* max bargraph is 10 bit */

		if (joystick_motion == KBD_DOWN){
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Blue);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"DOWN");	
			#endif
			sprintf(text_l, "down");			
			LED_Out(2);
		}
		else if (joystick_motion == KBD_LEFT) {
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"LEFT");		
			#endif
			sprintf(text_l, "left");					
			LED_Out(4);
		}
		else if (joystick_motion == KBD_RIGHT) {
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Green);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"RIGHT");	
			#endif			
			sprintf(text_l, "right");					
			LED_Out(8);
		}
		else if (joystick_motion == KBD_UP) {
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Magenta);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"UP");
			#endif
			sprintf(text_l, "up");										
			LED_Out(16);			
		}
		else if (joystick_motion == KBD_SELECT) {
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Purple);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"SELECT");
			#endif
			sprintf(text_l, "select");										
			LED_Out(32);
		}
		else {
			sprintf(text_l, "null");
		}
    /* Print message with AD value every 10 ms                               */
    if (clock_ms) {
      clock_ms = 0;
      printf("KBD value: %s\r\n", text);
			printf("Joystick direction: %s\r\n", text_l);
    }
		
  }
}