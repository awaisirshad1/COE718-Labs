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

#define __FI        1                     //  Font index 16x24               
#define __USE_LCD   0										// Uncomment to use the LCD 

// ITM Stimulus Port definitions for printf //////////////////
// ITM is the debug trace varible
// take an argument n, using the base memory address  0xE0000000, offset it by 4*n spaces, cast it to a type volatile unsigned char 
// and access the value at that address by referencing. 
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n))) 
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

// declare macro under the name DEMCR, long integer, probably 32 bits, at the memory address 0xE000EDFC
// 0xE000EDFC is the debug exception and monitor control register of an ARM Cortex-M processors
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
// is the trace enable register for ARM Cortex-M processors
#define TRCENA          0x01000000

// define a struct of 
struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

// send single character to output, in this case the debug output
int fputc(int ch, FILE *f) {
	
  if (DEMCR & TRCENA) { //bitwise AND operation between DEMCR and TRCENA, earlier we specified TRCENA to be the 7th hex digit 
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
/////////////////////////////////////////////////////////

char text[10]; //text to be sent to output
char text_l[10]; //text previously contained in output

static volatile uint16_t AD_dbg; //variable to set the AD value using debugger

uint16_t ADC_last;                      // Last converted value
uint16_t direction_last;								// Last displayed direction
// Import external variables from IRQ.c file                                  
extern uint8_t  clock_ms;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t  res;
	int32_t joyStick;
  uint32_t AD_sum   = 0U;
  uint32_t AD_cnt   = 0U;
  uint32_t AD_value = 0U;
  uint32_t AD_print = 0U;

  LED_Init();                               //  LED Initialization            
  ADC_Initialize();                                // ADC Initialization           
	KBD_Init();

#ifdef __USE_LCD
  GLCD_Init();                               // Initialize graphical LCD (if enabled 

  GLCD_Clear(White);                         // Clear graphical LCD display   
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       Blinky.c     ");
  GLCD_DisplayString(2, 0, __FI, "  Turn pot for LEDs ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(6, 0, __FI, "AD value:            ");
#endif

  //SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/100);        //Generate interrupt each 10 ms 

  while (1) {                                // Loop forever                  

    // AD converter input                                                     
    // AD converter input
    res = ADC_GetValue();
		joyStick = get_button();
 /*   if (res != -1) {                     // If conversion has finished
      ADC_last = (uint16_t)res;
      
      AD_sum += ADC_last;                // Add AD value to sum
      if (++AD_cnt == 16U) {             // average over 16 values
        AD_cnt = 0U;
        AD_value = AD_sum >> 4;          // average devided by 16
        AD_sum = 0U;
      }
    }
		*/
		if(joyStick != -1){
			direction_last = (uint16_t)joyStick; 
		}

 /*   if (AD_value != AD_print) {
      AD_print = AD_value;               // Get unscaled value for printout
      AD_dbg   = (uint16_t)AD_value;

      sprintf(text, "0x%04X", AD_value); // format text for print out
    
			
#ifdef __USE_LCD
      GLCD_SetTextColor(Red);
      GLCD_DisplayString(6,  9, __FI,  (unsigned char *)text);
			GLCD_SetTextColor(Green);
      GLCD_Bargraph (144, 7*24, 176, 20, (AD_value >> 2)); // max bargraph is 10 bit 
#endif
    }
		*/
		fprintf(stdout, "joyStick: %c",joyStick);

    // Print message with AD value every 10 ms                               
    if (clock_ms) {
      clock_ms = 0;

      printf("AD value: %s\r\n", text);
    }
		
		switch(joyStick){
			case KBD_SELECT:
				printf("selected");
				break;
			case KBD_UP:
				printf("UP");
				break;
			case KBD_DOWN:
				printf("DOWN");
				break;
			case KBD_LEFT:
				printf("LEFT");
				break;
			case KBD_RIGHT:
				printf("RIGHT");
				break;
				
		}
  }
}
