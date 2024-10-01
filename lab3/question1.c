#define USELCD   0										// Uncomment to use the LCD 
#define __FI        1                      // Font index 16x24  

#include <time.h>
#include <stdio.h>
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "LPC17xx.h"
#include "GLCD.h"
#include "LED.h"
#include "main.h"  

unsigned int task1 = 0;
unsigned int task2 = 0;
unsigned int task3 = 0;

void Thread1 (void const *argument);
void Thread2 (void const *argument);
void Thread3 (void const *argument);

void updateLCD(unsigned int threadNum);

char text1[20];
char text2[20];
char text3[20];

osThreadId tid1_Thread; // thread id
osThreadDef (Thread1, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid2_Thread; // thread id
osThreadDef (Thread2, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid3_Thread; // thread id
osThreadDef (Thread3, osPriorityNormal, 1, 0);                   // thread object

void threadsInitialized(void);

int Init_Thread (void) {

  tid1_Thread = osThreadCreate (osThread(Thread1), NULL);
	tid2_Thread = osThreadCreate (osThread(Thread2), NULL);
	tid3_Thread = osThreadCreate (osThread(Thread3), NULL);
	
  if(!tid1_Thread) return(-1); //check if thread creation for thread1 is successful
	
	threadsInitialized();
	
  return(0);
}

void Thread1(void const *argument){
	// for thread 1
	// if task1 gets to 5 000 000, delete this thread
	for(;;){
		
	#ifdef USELCD	
		updateLCD(1);
	
		LED_Off(4);
		LED_Off(5);
		LED_On(2);
		
		if(task1 >= 5000000){
			break;
		}
		else{
			task1+= 1;
			sprintf(text1, "task1: %d", task1);
		}
	#endif
		
	#ifndef USELCD
		if(task1 > 100000000){
			break;
		}
		else{
			task1+=1;
			printf("task1: %d", task1);
		}
	#endif
	}
	
#ifdef USELCD
	LED_Off(2);
#endif
	osThreadTerminate(tid1_Thread);
}

void Thread2(void const *argument){
	
	// for thread 2, add 1 to task2 and multiply by itself. 
	// if task2 gets bigger than 5 000 000, delete this thread
	
	
	for(;;){
		
	#ifdef USELCD	
		updateLCD(2);
	#endif
		LED_Off(4);
		LED_Off(2);
		LED_On(5);
		if(task2 >= 5000000){
			break;
		}
		else{
			task2+= 1;
			task2*= task2;
			sprintf(text2, "task2: %d", task2);
		}
		
	#ifndef USELCD
		if(task2 > 100000000){
			break;
		}
		else{
			task2+=1;
			printf("task2: %d", task2);
		}
	#endif
	}
#ifdef USELCD
	LED_Off(5);
#endif
	osThreadTerminate(tid2_Thread);
	
}

void Thread3(void const *argument){
	// for thread 3, if task3 is 0, add 3 to it. If it is not zero, multiply it by (task3 - 1)
	// if task3 becomes bigger than 100 000 000, delete this thread

	for(;;){
		
	#ifdef USELCD	
		updateLCD(3);
	
		LED_Off(5);
		LED_Off(2);
		LED_On(4);
		if(task3>= 100000000){
			break;
		}
		else{
			if(task3==0){
				task3+=3;
			}
			else{
				task3 *= (task3-1);
			}
			sprintf(text3, "task3: %d", task3);
		}
	#endif
		
		//for debugging mode only, let all 3 tasks be simply incrementing a counter
	#ifndef USELCD
		if(task3 > 100000000){
			break;
		}
		else{
			task3+=1;
			printf("task3: %d", task3);
		}
	#endif
		
		
	}
#ifdef USELCD
	LED_Off(4);
#endif
	osThreadTerminate(tid3_Thread);
	
}

void threadsInitialized(void){
#ifdef USELCD
	GLCD_SetTextColor(Blue);
	GLCD_DisplayString(2,  0, __FI,  (unsigned char *)"Threads Defined");
	GLCD_DisplayString(3,  0, __FI,  (unsigned char *)"Current thread:");
	GLCD_SetBackColor(Yellow);
	GLCD_SetTextColor(Red);
#endif
}

void updateLCD(unsigned int threadNum){
	#ifdef USELCD
		switch(threadNum){
			case(1):
				GLCD_ClearLn(4,1);
				GLCD_DisplayString(4,  0, __FI,  (unsigned char *)text1);
			case(2):
				GLCD_ClearLn(5,1);
				GLCD_DisplayString(5,  0, __FI,  (unsigned char *)text2);
			case(3):
				GLCD_ClearLn(6,1);
				GLCD_DisplayString(6,  0, __FI,  (unsigned char *)text3);
			break;
		}
	#endif
} 	 	