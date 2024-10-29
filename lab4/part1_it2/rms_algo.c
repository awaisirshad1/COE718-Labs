/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
//#define PRESCALE (25000-1)										// 25k PCLK clock cycles to increment TC by 1
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <math.h>
#include "Board_LED.h"                  // ::Board Support:LED
#include <LPC17xx.h>

//#define DEBUGMODE 0   								//uncomment for debug mode

void t_processA(void const *argument);		// process A
void t_processB(void const *argument);		// process B
void t_processC(void const *argument);		// process C

osThreadDef (t_processA, osPriorityNormal, 1, 0);							// thread for process A
osThreadDef (t_processB, osPriorityAboveNormal, 1, 0);				// thread for process B
osThreadDef (t_processC, osPriorityHigh, 1, 0);								// thread for process C


// delay function
void delay_ms(unsigned int ms);
void initDelayTimer(void);
void do_work(uint32_t delay);

/*###########################################################
	Virtual Timer declaration and call back method
############################################################*/

osThreadId threadA_id;
osThreadId threadB_id;
osThreadId threadC_id;

// variable to track the execution of a period of task C
int taskC_period_complete = 0;


// callback function to handle all 3 timers
void callback(void const *param){
	switch((uint32_t) param){
		case 0: // process A, LED 0
		#ifndef DEBUGMODE
			LED_On(5); LED_Off(6); LED_Off(7); 
		#endif
			osDelay(100); 
		#ifndef DEBUGMODE
		  LED_Off(5); LED_Off(6); LED_Off(7);
		#endif
			break;
		case 1: // process B, LED 2
		#ifndef DEBUGMODE
			LED_Off(5); LED_On(6); LED_Off(7); 
		#endif
			osDelay(100); 
		#ifndef DEBUGMODE
		  LED_Off(5); LED_Off(6); LED_Off(7);
		#endif
			break;
		case 2: // process C, LED 4
			taskC_period_complete = 1;
		#ifndef DEBUGMODE
			LED_Off(5); LED_Off(6); LED_On(7);
		#endif		
			osDelay(100); 
		#ifndef DEBUGMODE
			LED_Off(5); LED_Off(6); LED_Off(7);
		#endif	
			break;	
	}
}

osTimerDef(timerA_handle, callback);
osTimerDef(timerB_handle, callback);
osTimerDef(timerC_handle, callback);

//#############################################################
/*
The following is an execution diagram of our processes in a given 40000 ms time period:

A				|                                                            --------------------                    ------------------------------------------------------------

B				|                    ----------------------------------------

C				|--------------------                                                            --------------------

Period 	|---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
																																										     |                                                                               |
                                                                                       20000 ms                                                                         40000 ms

Therefore, task C executes first (highest priority) for 5000 ms, then 15000 ms of its period remains, during which it sleeps (using osDelay). The next thread with the highest
priority is task B, which executes for the next 10000 ms, and sleeps for remaining 25000 ms of its period (40000 ms period, 5000 ms used by C, then 10000 by B, therefore 
40000 - 5000 - 10000 = 25000) and after it finishes executing, 5000 ms remain till one period of task C is complete. While both B and C sleep, A is ready and executes for 
5000 ms, before it is interrupted by the higher priority task C being ready again. It preempts A, and executes for 5000 ms, then goes back to sleep again for the next 15000 ms.
Since 5000 ms of task A executed already, there is 15000 ms remaining, and it executes during this time where C sleeps. This is the flow of execution for one 40000 ms period, 
during which two periods of task C take place. This entire flow repeats across each 40000 ms period of time.
*/
//#############################################################
void t_processA(void const *argument){
	for(;;){
		// thread A is always waiting on flag 0x03
		osSignalWait(0x03, osWaitForever);
	#ifndef DEBUGMODE
		LED_On(0);
	#endif
		
		
		if(!taskC_period_complete){
			delay_ms(5000);
		}
		else{
			delay_ms(15000);
			taskC_period_complete = 0;
		}
		
	#ifndef DEBUGMODE
		LED_Off(0);
	#endif
		osSignalSet(threadC_id, 0x01);
	}
}

void t_processB(void const *argument){
	for(;;){
		// thread B is always waiting on flag 0x02
		osSignalWait(0x02, osWaitForever);
	#ifndef DEBUGMODE
		LED_On(2);
	#endif
		delay_ms(10000);
	#ifndef DEBUGMODE
		LED_Off(2);
	#endif
		osSignalSet(threadA_id, 0x03);
	}
}

void t_processC(void const *argument){
	for(;;){
		// thread C is always waiting on flag 0x01
		osSignalWait(0x01, osWaitForever);
	#ifndef DEBUGMODE
		LED_On(4);
	#endif
		delay_ms(5000);
//		taskC_period_complete = 1;
		
	#ifndef DEBUGMODE
		LED_Off(4);
	#endif
		/*
		if(taskC_period_complete){
			osSignalSet(threadA_id, 0x03);
			taskC_period_complete = 0;
		}
		else{
			osSignalSet(threadB_id, 0x02); 
		}
		*/
	}
}

void do_work(uint32_t delay){
	for(int i=0; i<10*delay; i++)
		for(int j=0; j<10*delay; j++);
}

/*void initDelayTimer(void){
	LPC_SC -> PCONP |= (1<<1);
	LPC_SC -> PCLKSEL0 &= ~(0x3<<3);
	
	LPC_TIM0 -> CTCR = 0x0;
	LPC_TIM0 -> PR = PRESCALE;
	
	LPC_TIM0 -> TCR = 0x02;
}
*/
// delay function that uses timer of the peripheral clock
void delay_ms(unsigned int ms){
	/*
#ifndef DEBUGMODE
	LPC_TIM0 -> TCR = 0x02;
	LPC_TIM0 -> TCR = 0x01;
	
	while(LPC_TIM0-> TC <ms);
	LPC_TIM0 -> TCR = 0x00;
#endif
	*/
#ifdef DEBUGMODE
	osDelay(ms);
	
#endif
	for(int i=0; i<ms; i++)
		for(int j=0; j<ms; j++);
}

int main(void){
	// initialize for delay function
#ifndef DEBUGMODE
	//initDelayTimer();
#endif
	osKernelInitialize();
	// virtual timers and start threads
	osTimerId timerA = osTimerCreate(osTimer(timerA_handle), osTimerPeriodic, (void *)0);
	osTimerId timerB = osTimerCreate(osTimer(timerB_handle), osTimerPeriodic, (void *)1);
	osTimerId timerC = osTimerCreate(osTimer(timerC_handle), osTimerPeriodic, (void *)2);
	
	// for demo purposes, use the provided timers
#ifndef DEBUGMODE
	LED_Initialize();


	osTimerStart(timerA, 40000);
	osTimerStart(timerB, 40000);
	osTimerStart(timerC, 20000);
#endif	
	// for debug purposes, use shorter periods
#ifdef DEBUGMODE
	osTimerStart(timerA, 20);
	osTimerStart(timerB, 20);
	osTimerStart(timerC, 10);
#endif
	threadA_id = osThreadCreate(osThread(t_processA), NULL);
	threadB_id = osThreadCreate(osThread(t_processB), NULL);
	threadC_id = osThreadCreate(osThread(t_processC), NULL);
	osKernelStart();
	osDelay(osWaitForever);
	
	for(;;);
}
