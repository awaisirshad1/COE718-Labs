/*----------------------------------------------------------------------------
	* Name: KBD.c
	* Purpose: MCB1700 low level Joystick
	* Version: V2.0
*----------------------------------------------------------------------------
	* This file is part of the uVision/ARM development tools.
	* This software may only be used under the terms of a valid, current,
	* end user licence from KEIL for a compatible version of KEIL software
	* development tools. Nothing else gives you the right to use this software.
	*
	* This software is supplied "AS IS" without warranties of any kind.
	*
	* Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
*----------------------------------------------------------------------------
	* History:
	* V2.0 - updated by Anita Tino for LPC1768
*----------------------------------------------------------------------------*/

#include <LPC17xx.H> /* LPC17xx definitions */
#include "KBD.h"
#include "GLCD.h"
#include "LED.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

//ITM Stimulus Port definitions for printf //////////////////
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

// put char in file pointed to by f, if DEMCR and TRCENA are not 0
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

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;

uint32_t KBD_val = 0;

/*----------------------------------------------------------------------------
initialize Joystick
*----------------------------------------------------------------------------*/
void KBD_Init (void) {

	LPC_SC->PCONP |= (1 << 15); /* enable power to GPIO & IOCON */

/* P1.20, P1.23..26 is GPIO (Joystick) */
	LPC_PINCON->PINSEL3 &= ~((3<< 8)|(3<<14)|(3<<16)|(3<<18)|(3<<20));

/* P1.20, P1.23..26 is input */
	LPC_GPIO1->FIODIR &= ~((1<<20)|(1<<23)|(1<<24)|(1<<25)|(1<<26));
}

/*----------------------------------------------------------------------------
Get Joystick value.. part of get_button
*----------------------------------------------------------------------------*/
uint32_t KBD_get (void) {
	uint32_t kbd_val;
	kbd_val = (LPC_GPIO1->FIOPIN >> 20) & KBD_MASK;
	return (kbd_val);
}
/*----------------------------------------------------------------------------
Get Joystick value
*----------------------------------------------------------------------------*/
uint32_t get_button (void) {
	uint32_t val = 0;
	val = KBD_get(); /* read Joystick state */
	val = (~val & KBD_MASK); /* key pressed is read as a non '0' value*/
	return (val);
}
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t  res;

  LED_Init();                                /* LED Initialization            */
	KBD_Init();
	
#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */
	
  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       KBD.c     ");
  GLCD_DisplayString(2, 0, __FI, "  Press or move joystick ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(6, 0, __FI, "Joystick direction:            ");
#endif

  //SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */
		res = get_button();

   

    /* Print message with AD value every 10 ms                               */
    if (clock_ms) {
      clock_ms = 0;
			
    }
  }
}
/*
int main(void){
	KBD_Init();
	while(1){
		
	}
	
}
*/
