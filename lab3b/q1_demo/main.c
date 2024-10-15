/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "LED.h"
#define GPIO_PORT1_LED28 (*((volatile unsigned long *)0x233806F4))
#define GPIO_PORT1_LED29 (*((volatile unsigned long *)0x233806FC))
#define GPIO_PORT1_LED31 (*((volatile unsigned long *)0x233806F0))
#define GPIO_PORT2_LED2 (*((volatile unsigned long *)0x23380A88))
#define GPIO_PORT2_LED3 (*((volatile unsigned long *)0x23380A8C))
extern int Init_Thread (void);
 
void turnOffAllLeds2(void);
/*
 * main: initialize and start the system
 */
int main (void) {
	LED_Init();
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	turnOffAllLeds2();
	GPIO_PORT1_LED31 = 1;
 	//Init_Thread ();
  //osKernelStart ();                         // start thread execution 
	//osDelay(osWaitForever);
}

void turnOffAllLeds2(void){
	LED_Off(2);
	LED_Off(3);
	LED_Off(4);
	LED_Off(5);
	LED_Off(6);
	LED_Off(28);
	LED_Off(29);
	LED_Off(31);
	
}