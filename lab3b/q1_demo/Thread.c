
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <math.h>
#include "LED.h"

unsigned int factorial(unsigned int n);
double pi_r_squared(int r);
void delay(int value);
void turnOffAllLeds(void);
double powerAndFactorial(int base, int exponent, int fact);

#define GPIO_PORT1_LED28 (*((volatile unsigned long *)0x233806F4))
#define GPIO_PORT1_LED29 (*((volatile unsigned long *)0x233806FC))
#define GPIO_PORT1_LED31 (*((volatile unsigned long *)0x233806F0))
#define GPIO_PORT2_LED2 (*((volatile unsigned long *)0x23380A88))
#define GPIO_PORT2_LED3 (*((volatile unsigned long *)0x23380A8C))
//#define bitBandBase 0x23380680


/*----------------------------------------------------------------------------
 *      Everything to do with our threads
 *---------------------------------------------------------------------------*/
 
  
void ThreadA (void const *argument); // thread for task A
void ThreadB (void const *argument); // thread for task B
void ThreadC (void const *argument); // thread for task C
void ThreadD (void const *argument); // thread for task D
void ThreadE (void const *argument); // thread for task E

// Priority order: C > A, D > B, E




osThreadId tid_B_Thread; // thread id
osThreadDef (ThreadB, osPriorityBelowNormal, 1, 0);                   // thread object

osThreadId tid_E_Thread; // thread id
osThreadDef (ThreadE, osPriorityBelowNormal, 1, 0);                   // thread object

osThreadId tid_D_Thread; // thread id
osThreadDef (ThreadD, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid_A_Thread; // thread id
osThreadDef (ThreadA, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid_C_Thread; // thread id
osThreadDef (ThreadC, osPriorityAboveNormal, 1, 0);                   // thread object

int Init_Thread (void) {

  tid_A_Thread = osThreadCreate (osThread(ThreadA), NULL);
	tid_B_Thread = osThreadCreate (osThread(ThreadB), NULL);
	tid_C_Thread = osThreadCreate (osThread(ThreadC), NULL);
	tid_D_Thread = osThreadCreate (osThread(ThreadD), NULL);
	tid_E_Thread = osThreadCreate (osThread(ThreadE), NULL);
	
  if(!tid_A_Thread) return(-1); // if thread A not created, return -1 (failure)
  
  return(0);
}

// Task A

void ThreadA (void const *argument) {
	unsigned int a = 0;
	unsigned int x = 0;
	LED_On(3);
	
	for(;;) {
		turnOffAllLeds();
		LED_On(3);
		
		// condition upon which we want to terminate the loop
		if(x==256) break;
		
  	a += x + (x+2);
		x++;
		// delay a bit
		//delay(100);
  }
	
	// try yielding instead of terminating
	osThreadYield();
	
	//terminate this thread
	//osThreadTerminate(tid_A_Thread);
}

// Task B

void ThreadB (void const *argument) {

	double b = 0;
	unsigned int n = 1;
	
	turnOffAllLeds();
	LED_On(4);
	for(;;) {

		turnOffAllLeds();
		LED_On(4);
		
		// condition upon which we want to terminate the loop
		if(n == 16) break;
		
		b += (pow(2, n))/(factorial(n));
		n += 1;
		//delay(100);
  }     

	// try yielding instead of terminating
	osThreadYield();
}

// Task C

void ThreadC (void const *argument) {
	double c = 0;
	unsigned int n = 1;
	LED_On(5);
	for(;;) {
		turnOffAllLeds();
		LED_On(5);
		
		// condition upon which we want to terminate the loop
  	if(n == 16) break;
		c += (n+1)/n;
		n += 1;
		//delay(100);
  }
	
	// try yielding instead of terminating
	osThreadYield();
}

// Task D

void ThreadD (void const *argument) {
	double d = 1;
	int i;
	turnOffAllLeds();
	LED_On(6);
	for (i=1; i<=5; i++){
		turnOffAllLeds();
		LED_On(6);
		d += powerAndFactorial(5, i, i);
	}	
	
	// try yielding instead of terminating
	osThreadYield();
}

// Task E

void ThreadE (void const *argument) {
	
	double e = 0;
	
	unsigned int r;
	unsigned int i;
	
	LED_On(7);
	for (r=1; r<=20; r++){
		for ( i = 1; i < 13; i++){
			turnOffAllLeds();
			LED_On(7);
			e += i*pi_r_squared(r);
		}
		//delay(200);
	}
	
	// try yielding instead of terminating
	osThreadYield();
}


unsigned int factorial(unsigned int n){
	unsigned int result = 1;
	if(n==1 || n==0) {
		return result;
	}
	while(n!=1){
		result *= n;
		--n;
	}
	
	return result;
}


double pi_r_squared(int r){
	return (double)(3.14)*(r*r);
}


void delay(int value){
	unsigned int i;
	unsigned int j;
	for (i = 0; i < value; i++){
		for (j = 0; j < value; j++){}
	}
}

void turnOffAllLeds(void){
	int i;
	for (i = 3; i<8; i++){
		LED_Off(i);
	}
}

double powerAndFactorial(int base, int exponent, int fact){
	return pow((double) base, (double) exponent)/factorial(fact);
}
