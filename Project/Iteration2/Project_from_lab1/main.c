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
#include "main.h"
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h"
#include "Board_ADC.h" 
#include "KBD.h"
#include "mainMenu.h"
#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions

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

// import currentSelection variable from mainMenu.c file
extern int8_t currentSelection;
extern int8_t previousSelection;
//void displayMainMenu(void);
extern int Init_Thread (void);

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	uint32_t joyStick;
	uint32_t joyStickPrev = 0U;
// initialize peripherals
  LED_Init();                                /* LED Initialization            */
  ADC_Initialize();                                /* ADC Initialization            */
	KBD_Init();

#ifdef __USE_LCD	
  GLCD_Init();                               // Initialize graphical LCD (if enabled 
#endif
	osKernelInitialize();
	Init_Thread();
	osKernelStart();
	osDelay(osWaitForever);
  //SystemCoreClockUpdate();
  //SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */
	// uses the system timre to generate interrupts specified by the ticks, reloads the timer upon interrupts using a logic OR, accounts for interrupt priority
	// The SysTick_Config function initializes the system timer to generate periodic interrupts at a specified tick interval. It configures the timer's reload 
	// value, sets the interrupt priority, and enables the timer, returning 0 on success or 1 if the tick value is invalid.
	
  while (1) {                                /* Loop forever                  */
		
		joyStick = get_button(); 
   	
		if(joyStick == KBD_SELECT){
			joyStickPrev = joyStick;
		}
		else if(joyStick == KBD_DOWN){
			joyStickPrev = joyStick;
			if(currentSelection!=7){
				previousSelection = currentSelection;
				currentSelection+=2;
			}
		}
		else if(joyStick == KBD_UP){
			joyStickPrev = joyStick;
			if(currentSelection!=3){
				previousSelection = currentSelection;
				currentSelection-=2;
			}
		}
		/*
		else if(joyStick == KBD_RIGHT){
			joyStickPrev = joyStick;
		}
		else if(joyStick == KBD_LEFT){
			joyStickPrev = joyStick;
		}
		*/
		//switch(joyStick){
		//	case 
		//}
    /* Print message with AD value every 10 ms                               */
    if (clock_ms) {
      clock_ms = 0;
      printf("KBD from get_button: %s\r\n", text);
			printf("direction: %s\r\n", text_l);
    }
		
  }
}

