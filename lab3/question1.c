#include <time.h>
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "LPC17xx.h"
#include "GLCD.h"
#include "LED.h"

/*
#ifndef __USE_LCD

#define __USE_LCD   0										// Uncomment to use the LCD 
#define __FI        1                      // Font index 16x24               

#endif
*/

#define __FI        1

unsigned int task1 = 0;
unsigned int task2 = 0;
unsigned int task3 = 0;

void Thread1 (void const *argument);
void Thread2 (void const *argument);
void Thread3 (void const *argument);

void delay_ms(unsigned int milli);

char text[20];

osThreadId tid1_Thread; // thread id
osThreadDef (Thread1, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid2_Thread; // thread id
osThreadDef (Thread2, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid3_Thread; // thread id
osThreadDef (Thread3, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread (void) {

  tid1_Thread = osThreadCreate (osThread(Thread1), NULL);
	tid2_Thread = osThreadCreate (osThread(Thread2), NULL);
	tid3_Thread = osThreadCreate (osThread(Thread3), NULL);
	
  if(!tid1_Thread) return(-1); //check if thread creation for thread1 is successful
	
//#ifdef __USE_LCD
	
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(4,  0, __FI,  (unsigned char *)"Threads Defined");
	GLCD_DisplayString(5,  0, __FI,  (unsigned char *)"Current thread:");
			
//#endif
	
  return(0);
}

void Thread1(void const *argument){
	LED_On(2);
	// for thread 1
	// if task1 gets to 5 000 000, delete this thread
//#ifdef __USE_LCD
	
	GLCD_ClearLn(6, __FI);
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(6,  5, __FI,  (unsigned char *)"Thread 1");
			
//#endif
	for(;;){
		if(task1 >= 5000000){
			break;
		}
		else{
			task1+= 1;
			osDelay(15000);
		}
	//	delay_ms(10);
	}
	LED_Off(2);
	osThreadTerminate(tid1_Thread);
}

void Thread2(void const *argument){
	
	// for thread 2, add 1 to task2 and multiply by itself. 
	// if task2 gets bigger than 500 000 000, delete this thread
	
	LED_On(3);
//#ifdef __USE_LCD
	
	GLCD_ClearLn(6, __FI);
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(6,  5, __FI,  (unsigned char *)"Thread 2");
			
//#endif
/*
	for(;;){
		if(task2 > 500000000){
			break;
		}
		else{
				if(task2 == 0){
					task2 += 1;
				}
				else{
					task2 *= task2 + 1;
				}
				delay_ms(100);
		}
	}
	*/
	for(;;){
		if(task2 >= 5000000){
			break;
		}
		else{
			task2+= 1;
			osDelay(15000);
		}
//delay_ms(10);
	}
	LED_Off(3);
	osThreadTerminate(tid2_Thread);
	
}

void Thread3(void const *argument){
	// for thread 3, if task3 is 0, add 3 to it. If it is not zero, multiply it by (task3 - 1)
	// if task3 becomes bigger than 100 000 000, delete this thread
	
	LED_On(4);
//#ifdef __USE_LCD
	
	GLCD_ClearLn(6, __FI);
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(6,  5, __FI,  (unsigned char *)"Thread 3");
			
//#endif
	
/*	
	for(;;){
		if(task3 > 100000000){
			break;
		}
		else{
			if(task3 == 0){
				task3 += 3;
			}
			else{
				task3 *= task3 - 1;
				
			}
			delay_ms(100);
		}
	}
	*/
	for(;;){
		if(task3>= 5000000){
			break;
		}
		else{
			task3+= 1;
			osDelay(15000);
		}
		//delay_ms(10);
	}
	LED_Off(4);
	osThreadTerminate(tid3_Thread);
	
}


void delay_ms(unsigned int milli){
	//unsigned int milliseconds = 1000*milli;
	clock_t start_time = clock();
	while(clock() < start_time + milli);
}

