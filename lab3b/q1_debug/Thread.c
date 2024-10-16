
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <math.h>
#include "LPC17xx.h"
#define USE_LED 0

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
unsigned int factorial(unsigned int n);
double pi_r_squared(int r);
void delay(int value);

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
	
  if(!tid_A_Thread || !tid_B_Thread || !tid_C_Thread || !tid_D_Thread || !tid_E_Thread) return(-1); // if any thread not created, return -1 (failure)
  
  return(0);
}

// Task A

void ThreadA (void const *argument) {
	unsigned int a = 0;
	unsigned int x = 0;
	for(;;) {
		// condition upon which we want to terminate the loop
		if(x==256) break;
		
  	a += x + (x+2);
		x++;
		// delay a bit
		//delay(100);
  }
	//sprintf(text, "final result for thread A: a = %d", a);
	//printf("final result for thread A: a = %d", a);
	// try yielding instead of terminating
	osThreadYield();
}

// Task B

void ThreadB (void const *argument) {
	double b = 0;
	unsigned int n = 1;
	for(;;) {
		// condition upon which we want to terminate the loop
		if(n == 16) break;
		
		b += (pow(2, n))/(factorial(n));
		n += 1;
		//delay(100);
  }     
	//sprintf(text, "final result for Thread B: b = %f", b);
	//printf("final result for thread B: b = %f", b);
	// try yielding instead of terminating
	osThreadYield();
	
}

// Task C

void ThreadC (void const *argument) {
	double c = 0;
	unsigned int n = 1;
	for(;;) {
		// condition upon which we want to terminate the loop
  	if(n == 16) break;
		c += (n+1)/n;
		n += 1;
		//delay(100);
  }
	//sprintf(text, "final result for Thread C: c = %f", c);
	//printf("final result for Thread C: c = %f", c);
	// try yielding instead of terminating
	osThreadYield();
}

// Task D

void ThreadD (void const *argument) {
	double d;
	//delay(100);
	d = 1 + 5/factorial(1) + pow(5,2)/factorial(2) + pow(5,3)/factorial(3) + pow(5,4)/factorial(4) + pow(5,5)/factorial(5);
	//sprintf(text, "final result for Thread D: d = %f", d);
	//printf("final result for Thread D: d = %f", d);
	
	// try yielding instead of terminating
	osThreadYield();
}

// Task E

void ThreadE (void const *argument) {
	double e = 0;
	
	unsigned int r;
	unsigned int i;
	for (r=1; r<=20; r++){
		for ( i = 1; i < 13; i++){
			e += i*pi_r_squared(r);
		}
		//delay(200);
	}
	//sprintf(text, "final result for Thread E: e = %f", e);
	//printf("final result for Thread E: e = %f", e);
	
	osThreadYield();
	
}



