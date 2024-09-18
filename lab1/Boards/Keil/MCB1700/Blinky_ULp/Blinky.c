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
//static volatile uint32_t joyStick_dbg;
uint32_t joyStick_dbg;

uint16_t ADC_last;                      // Last converted value
/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	uint32_t joyStick;
	uint32_t joyStickPrev = 0U;
	uint16_t ledToTurnOn = 3U;

  LED_Init();                                /* LED Initialization            */
  ADC_Initialize();                                /* ADC Initialization            */
	KBD_Init();

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Lab 1   ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       Blinky.c     ");
  GLCD_DisplayString(2, 0, __FI, "    Use joystick   ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Black);
  GLCD_DisplayString(4, 0, __FI, " direction: ");
#endif

  //SystemCoreClockUpdate();
  //SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */
#ifdef __USE_LCD
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(5,5, __FI,(unsigned char *)"NONE");
#endif
  while (1) {                                /* Loop forever                  */

		joyStick = get_button(); 
		//joyStick = joyStick_dbg;	
		sprintf(text, "0x%08X", joyStick); // format text for print out*/
   		
		if(joyStick == KBD_SELECT && joyStick!=joyStickPrev){
			joyStickPrev = joyStick;
			LED_Off(ledToTurnOn);
			ledToTurnOn = 3U;
			#ifdef __USE_LCD
				GLCD_ClearLn(5, __FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(5,5, __FI, (unsigned char *)"SELECT");
			#endif
			
			sprintf(text_l, "SELECT");
		}
		else if(joyStick == KBD_DOWN && joyStick!=joyStickPrev){
			joyStickPrev = joyStick;
			LED_Off(ledToTurnOn);
			ledToTurnOn = 7U;
			#ifdef __USE_LCD
				GLCD_ClearLn(5, __FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(5,5, __FI,(unsigned char *)"DOWN");
			#endif
			
			sprintf(text_l, "DOWN");
		}
		else if(joyStick == KBD_UP && joyStick!=joyStickPrev){
			joyStickPrev = joyStick;
			LED_Off(ledToTurnOn);
			ledToTurnOn = 0U;
			#ifdef __USE_LCD
				GLCD_ClearLn(5, __FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(5,5, __FI, (unsigned char *)"UP");
			#endif
			
			sprintf(text_l, "UP");
		}
		else if(joyStick == KBD_RIGHT && joyStick!=joyStickPrev){
			joyStickPrev = joyStick;
			LED_Off(ledToTurnOn);
			ledToTurnOn = 4U;
			#ifdef __USE_LCD
				GLCD_ClearLn(5, __FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(5,5, __FI, (unsigned char *)"RIGHT");
			#endif
			
			sprintf(text_l, "RIGHT");
		}
		else if(joyStick == KBD_LEFT && joyStick!=joyStickPrev){
			joyStickPrev = joyStick;
			LED_Off(ledToTurnOn);
			ledToTurnOn = 5U;
			#ifdef __USE_LCD
				GLCD_ClearLn(5, __FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(5,5, __FI, (unsigned char *)"LEFT");
			#endif
			
			sprintf(text_l, "LEFT");
		}
		
		LED_On(ledToTurnOn);
		
    /* Print message with AD value every 10 ms                               */
    if (clock_ms) {
      clock_ms = 0;
      printf("KBD from get_button: %s\r\n", text);
			printf("direction: %s\r\n", text_l);
    }
		
  }
}