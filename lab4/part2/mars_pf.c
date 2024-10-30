
#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "RTE_Components.h"             // Component selection


void taskA (void const *argument);
void taskB (void const *argument);
void taskC (void const *argument);
// let the availability of R1 be represented by this boolean
bool R1 = true;
// set to true or false depending on whether you want to implement the priority inheritance protocol (solution)
bool ENABLE_PIP = false;
osThreadDef(taskA, osPriorityHigh, 1, 0);           //high priority task
osThreadDef(taskB, osPriorityNormal, 1, 0);         //medium priority task
osThreadDef(taskC, osPriorityBelowNormal, 1,0);     //lower priority task

osThreadId taskA_id, taskB_id, taskC_id, mainThread_id;

void delay_ms(unsigned int milliseconds);


// let each of our tasks execute for 10000 ms total, task C (which acquires R1), then task B, then task A
void taskA(void const *argument){
	// do work that doesnt require resource R1
	delay_ms(400);
	// critical section that requries R1
	if(!R1 && ENABLE_PIP){
		// Priority Inheritance Protocol
		// elevate task C to same priority as task
		osThreadSetPriority(taskC_id, osThreadGetPriority(taskA_id));
		osSignalSet(taskC_id, 0x03);
		osSignalWait(0x01, osWaitForever);
		
		// demote task C priority once signal is received
		osThreadSetPriority(taskC_id, osPriorityBelowNormal);
	}
	
	delay_ms(1000);
	
}

void taskB(void const *argument){
	// all this thread does is some random work
	delay_ms(200);
}
	
void taskC(void const *argument){
	// do a tiny amount of work
	delay_ms(400);
	// critical section:
	// acquire R1 by setting to false
  R1 = false;
	// do critical work for longer, will be preempted by task B
	delay_ms(600);
	// now return R1
  R1 = true;
	// and set the flag for task A
	osSignalSet(taskA_id, 0x01);
	osSignalWait(0x03, osWaitForever);
	// now do non critical section work
	delay_ms(500);
}

void delay_ms(unsigned int milliseconds){
	int i, j;
	for (i=0; i<milliseconds; i++){
		for(j=0; j<1000; j++);
	}
}


int main(void){
	osKernelInitialize();
	mainThread_id = osThreadGetId();
	osThreadSetPriority(mainThread_id, osPriorityHigh);
	
	taskC_id = osThreadCreate(osThread(taskC), NULL);
	// give C some time to do its work until 50 ms
	osDelay(500);
	 
	taskB_id = osThreadCreate(osThread(taskB), NULL);
	// give B some time to do a little bit of its work
	osDelay(100);
	
	taskA_id = osThreadCreate(osThread(taskA), NULL);

	osThreadTerminate(mainThread_id);
	osKernelStart ();    
	for (;;) {}

}
