#include "LPC17xx.h"
#include <stdio.h>

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
//------------------------------------------------------------------- //

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))

volatile unsigned long * bit;
#define ADCR_Bit24   (*((volatile unsigned long *)0x42680060))

int main(void){
	printf("\n ADCR before any operations: 0x%08X \n", LPC_ADC->ADCR);
//mask mode
	printf("\nMask mode\n");
  LPC_ADC->ADCR |=  ( 1 << 24);              // start conversion    
	printf("Addr: 0x%08X \n", LPC_ADC->ADCR);
  LPC_ADC->ADCR &= ~( 7 << 24);              // stop conversion     
	printf("Addr: 0x%08X \n", LPC_ADC->ADCR);
	
	//function mode	
	printf("\nfunction mode\n");
  bit = &BitBand(&LPC_ADC->ADCR, 24);		
 *bit = 0;
	printf("Addr: 0x%08X \n", LPC_ADC->ADCR);	
 *bit = 1;
	printf("Addr: 0x%08X \n", LPC_ADC->ADCR);
 *bit = 0;
	printf("Addr: 0x%08X \n", LPC_ADC->ADCR);
	
	//bit band mode
	printf("\nBit Masking mode\n");
	ADCR_Bit24 = 1;
	printf("Addr: 0x%08X \n", LPC_ADC->ADCR);
	ADCR_Bit24  = 0;
	printf("Addr: 0x%08X \n", LPC_ADC->ADCR);
	
	return 0;
}

