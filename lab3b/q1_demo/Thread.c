
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <math.h>
#include "LED.h"

unsigned int factorial(unsigned int n);
double pi_r_squared(int r);
void delay(int value);
void turnOffAllLeds(void);
double powerAndFactorial(int base, int exponent, int fact);
extern void turnOnLed(unsigned int ledNum);
double power(int base, int exponent);


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
	turnOffAllLeds();
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
	
	for(;;) {
		turnOffAllLeds();
		turnOnLed(28);
		// condition upon which we want to terminate the loop
		if(x==256) break;
		
  	a += x + (x+2);
		x++;
		// delay a bit
		delay(10000);
  }
	
	// try yielding instead of terminating
	osThreadYield();
}

// Task B

void ThreadB (void const *argument) {

	double b = 0;
	unsigned int n = 1;

	for(;;) {

		turnOffAllLeds();
		turnOnLed(29);
		// condition upon which we want to terminate the loop
		if(n == 16) break;
		
		b += (power(2, n))/(factorial(n));
		n += 1;
		delay(10000);
  }     

	// try yielding instead of terminating
	osThreadYield();
}

// Task C

void ThreadC (void const *argument) {
	double c = 0;
	unsigned int n = 1;
	for(;;) {
		turnOffAllLeds();
		turnOnLed(2);
		// condition upon which we want to terminate the loop
  	if(n == 16) break;
		c += (n+1)/n;
		n += 1;
		delay(10000);
  }
	
	// try yielding instead of terminating
	osThreadYield();
}

// Task D

void ThreadD (void const *argument) {
	double d = 1;
	int i;
	for (i=1; i<=5; i++){
		turnOffAllLeds();	
		turnOnLed(4);
		d += powerAndFactorial(5, i, i);
		delay(10000);
	}	
	
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
			turnOffAllLeds();
			turnOnLed(6);
			e += i*pi_r_squared(r);
		}
		delay(20000);
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
	//LPC_GPIO1 -> FIOPIN &= ~(1 << 28);
	//LPC_GPIO1 -> FIOPIN &= ~(1 << 29);
	//LPC_GPIO2 -> FIOPIN &= ~(1 << 2);
	//LPC_GPIO2 -> FIOPIN &= ~(1 << 4);
	//LPC_GPIO2 -> FIOPIN &= ~(1 << 6);
	LED_Off(28);
	LED_Off(29);
	LED_Off(31);
	LED_Off(2);
	LED_Off(3);
	LED_Off(4);
	LED_Off(5);
	LED_Off(6);
}

double powerAndFactorial(int base, int exponent, int fact){
	return power( base, exponent)/factorial(fact);
}

double power(int base, int exponent){
	int count = 1; 
	if(exponent == 0) return (double) 1;
	while(count<exponent){
		base *= base;
		count++;
	}
	return (double) base;
}

void turnOnLed(unsigned int ledNum){
	LED_On(ledNum);
	/*switch(ledNum){
		case 28:
			LPC_GPIO1 -> FIOPIN |= (1<<28);
		case 29:
			LPC_GPIO1 -> FIOPIN |= (1<<29);
		case 2:
			LPC_GPIO2 -> FIOPIN |= (1<<2);
		case 4:
			LPC_GPIO2 -> FIOPIN |= (1<<4);
		case 6:
			LPC_GPIO2 -> FIOPIN |= (1<<6);	
	}*/
}
