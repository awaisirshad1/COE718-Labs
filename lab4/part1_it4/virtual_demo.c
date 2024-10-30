/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <math.h>
#include "Board_LED.h"                  // ::Board Support:LED

  
void threadA (void const *argument); // thread function
void threadB (void const *argument); // thread function
void threadC (void const *argument);

// other APIs
void delay_ms(unsigned int milliseconds);

osThreadDef (threadA, osPriorityNormal, 1, 0);                   // thread object
osThreadDef (threadB, osPriorityAboveNormal, 1, 0);                   // thread object
osThreadDef (threadC, osPriorityHigh, 1, 0);  

unsigned int threadA_interruptedFlag = 0;
volatile unsigned int i=0;
volatile unsigned int j=0;
/*###########################################################
	Virtual Timer declaration and call back method
############################################################*/


osThreadId threadA_id;
osThreadId threadB_id;	
osThreadId threadC_id;	

// Toggle the LED associated with the timer
void callback(void const *param){
	switch( (uint32_t) param){
		case 0:
			//LED_On(6); LED_Off(7); LED_Off(5);
			//osDelay(1000);
			//delay_ms(1000);
		  //LED_Off(6); LED_Off(7); LED_Off(5);
			osSignalSet(threadA_id, 0x03);
			break;
		case 1:
			//LED_Off(6); LED_On(7); LED_Off(5);
			//delay_ms(1000);
		  //osDelay(1000);
		  //LED_Off(6); LED_Off(7); LED_Off(5);
 			osSignalSet(threadB_id, 0x02);
			break;
		case 2:
			//LED_Off(6); LED_Off(7); LED_On(5);
			//delay_ms(1000);
			//LED_Off(6); LED_Off(7); LED_Off(5);
			threadA_interruptedFlag = 1;
			osSignalSet(threadC_id, 0x01);
			break;
	}
}
osTimerDef(timerA_handle, callback);
osTimerDef(timerB_handle, callback);
osTimerDef(timerC_handle, callback);
//#############################################################


/*#############################################################
  Flash LED 0, signal to thread 2, wait for 3 to finish
*#############################################################*/
void threadA (void const *argument) {
	for (;;) {
		osSignalWait (0x03,osWaitForever);
		//LED_On(0);
		//osDelay(5000);
		delay_ms(6000);
		//osSignalSet	(threadB_id,0x01);
		//LED_Off(0); 
		osSignalSet(threadC_id, 0x01);
	}
}

/*################################################################
  Flash LED 2, signal to thread 3, wait for thread 1 to finish
*################################################################*/
void threadB (void const *argument) {
	for (;;) 	{
		//LED_Off(2);	
		osSignalWait (0x02,osWaitForever);
		//LED_On(2);
    //osDelay(5000);
		delay_ms(3000);
		//LED_Off(2);
		osSignalSet	(threadA_id,0x03);
	}
}


/*################################################################
  Flash LED 4, signal to thread 1, wait for thread 2 to finish
*################################################################*/
void threadC (void const *argument){
	for (;;) 	{
		//LED_Off(4);
		osSignalWait (0x01,osWaitForever);
		
		//LED_On(4); 
		delay_ms(1500);
		//osDelay(5000);
		//LED_Off(4);
		if(threadA_interruptedFlag==1){
			osSignalSet	(threadB_id,0x02);
//			osSignalSet	(threadA_id,0x03);
		}
		threadA_interruptedFlag = 0;
	}
}

void delay_ms(unsigned int milliseconds){

	for (i=0; i<milliseconds; i++){
		for(j=0; j<1000; j++);
	}
}

/*###################################################################
  Create and start threads
 *###################################################################*/
int main (void) {
		osKernelInitialize (); 
	//Virtual timer create and start
	osTimerId timer_A = osTimerCreate(osTimer(timerA_handle), osTimerPeriodic, (void *)0);	
	osTimerId timer_B = osTimerCreate(osTimer(timerB_handle), osTimerPeriodic, (void *)1);	
	osTimerId timer_C = osTimerCreate(osTimer(timerC_handle), osTimerPeriodic, (void *)2);
  //LED_Initialize ();
	
	osTimerStart(timer_C, 1000);
	osTimerStart(timer_B, 2000);
	osTimerStart(timer_A, 4000);	
		
	
	//Signal and wait threads
	threadA_id = osThreadCreate(osThread(threadA), NULL);
	threadB_id = osThreadCreate(osThread(threadB), NULL);
  threadC_id = osThreadCreate(osThread(threadC), NULL);
	
	 osKernelStart ();      
	osDelay(osWaitForever);               						
	
	for (;;);
}
