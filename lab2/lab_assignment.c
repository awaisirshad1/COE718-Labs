#include <stdio.h>
#include "LPC17xx.h"                    // Device header
//#include "KBD.h"
#include "GLCD.h"


#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */


//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))

#define GPIO_PORT1_LED30 (*((volatile unsigned long *)0x233806F8))
#define GPIO_PORT2_LED4 (*((volatile unsigned long *)0x23380A90))

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

//delay
void delay_ms(unsigned int milliseconds){
	for (volatile unsigned int i=0; i<milliseconds; i++){
		for(volatile unsigned int j=0; j<1000; j++);
	}
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void){

	uint16_t ledForBitband = 3U;
	uint16_t ledForConditional = 7U;
	uint16_t ledForBarrelShifting = 0U;
	
	
	
	printf("LPC_GPIO_BASE: 0x%lX\n", LPC_GPIO_BASE);
	printf("LPC_GPIO1_BASE: 0x%lX\n", LPC_GPIO1_BASE);
	printf("LPC_GPIO_BASE2: 0x%lX\n", LPC_PINCON_BASE);
	
	LPC_SC->PCONP     |= (1 << 15);            /* enable power to GPIO & IOCON  */

  LPC_GPIO1->FIODIR |= 0xB0000000;           /* LEDs on PORT1 are output      */
  LPC_GPIO2->FIODIR |= 0x0000007C;           /* LEDs on PORT2 are output      */
	
	// pointers to addresses of port 1 pin 29 and port 2 pin 3 for functional mode
	volatile unsigned long * GPIO_PORT1_LED29 = &BitBand(&LPC_GPIO1 -> FIOPIN1, 29);
	volatile unsigned long * GPIO_PORT2_LED3 = &BitBand(&LPC_GPIO2 -> FIOPIN0, 3);
	
#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Lab 2   ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       lab_assignment.c     ");
  GLCD_DisplayString(2, 0, __FI, "    	Barrel Shift operation   ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Black);
  GLCD_DisplayString(4, 0, __FI, " operation: ");
#endif
	
	int R1 = 1;
	int R2 = 1;
	int R3 = 2;
	while(1){
		if((R1-R2)<R3){
			//masking, so port 1 bit 28 and port 2 bit 2
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"Masking");		
			#endif
			
			LPC_GPIO1 -> FIOPIN |= (1 << 29);
			LPC_GPIO2 -> FIOPIN |= (1 << 2);
			R1 += 2;
			printf("masking");
		}
		else if((R1-R2)>R3){
			//functional, so port 1 bit 29 and port 2 bit 3
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"Function");		
			#endif
			*GPIO_PORT1_LED29 = 1;
			*GPIO_PORT2_LED3 = 1;
			R1 -= 3;
			printf("functional mode");
		}
		else{
			//direct bit banding, so port 1 bit 30 and port 2 bit 4
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"Direct BB");		
			#endif
			GPIO_PORT1_LED30 = 1;
			GPIO_PORT2_LED4 = 1;
			R1 +=1;
			printf("bit banding");
		}
		//stall
		delay_ms(2000U);
		
		//now clear all the bits using the exact same methods we used above
		//clear port1 bit 28 and port 2 bit 2 with masking:
		LPC_GPIO1 -> FIOPIN = ~(1 << 29);
		LPC_GPIO2 -> FIOPIN = ~(1 << 2);
		
		//clear port 1 bit 29 and port 2 bit 3 with function
		*GPIO_PORT1_LED29 = 0;
		*GPIO_PORT2_LED3 = 0;
		
		//clear port 1 bit 30 and port 2 bit 4 directly
		GPIO_PORT1_LED30 = 0;
		GPIO_PORT2_LED4 = 0;
		
		//Now, barrel perform addition and multiplication that uses barrel shifter, print initial to LCD and print result to LCD as well
		int r1 = 1, r2 = 0, r3 = 5;
	
		while(r2 <= 0x18){
			#ifdef __USE_LCD
				GLCD_ClearLn(6,__FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(6,  9, __FI,  (unsigned char *)"Barrel");		
			#endif
			if((r1 - r2) > 0){
				r1 = r1 + 2;
				r2 = r1 + (r3*4);
				r3 = r3/2;
			}
			else{
				r2 = r2 + 1;
			}
			sprintf(text, "r1:%d,r2:%d,r3:%d", r1,r2,r3);
			#ifdef __USE_LCD
				GLCD_ClearLn(7,__FI);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(7,  9, __FI,  (unsigned char *)text);		
			#endif
			delay_ms(500U);
		}
		delay_ms(2000U);
	}
	
	
	return 0;
}


