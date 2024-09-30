#include "cmsis_os.h"                                           // CMSIS RTOS header file

unsigned int task1 = 0;
unsigned int task2 = 0;
unsigned int task3 = 0;

void Thread1 (void const *argument);
void Thread2 (void const *argument);
void Thread3 (void const *argument);


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
  
  return(0);
}

void Thread1(void const *argument){
	// for thread 1
	// if task1 gets to 5 000 000, delete this thread
	
	for(;;){
		if(task1 >= 5000000){
			break;
		}
		else{
			task1+= 1;
		}
	}
	osThreadTerminate(tid1_Thread);
}

void Thread2(void const *argument){
	// for thread 2, add 1 to task2 and multiply by itself. 
	// if task2 gets bigger than 5 000 000, delete this thread
	/*
	for(;;){
		if(task2 > 5000000){
			break;
		}
		else{
				if(task2 == 0){
					task2 += 1;
				}
				else{
					task2 *= task2 + 1;
				}
				osDelay(5U);
		}
	}
	*/
	//for testing
	for(;;){
		if(task2 >= 5000000){
			break;
		}
		else{
			task2+= 1;
		}
	}
	
	
	osThreadTerminate(tid2_Thread);
	
}

void Thread3(void const *argument){
	// for thread 3, if task3 is 0, add 3 to it. If it is not zero, multiply it by (task3 - 1)
	// if task3 becomes bigger than 100 000 000, delete this thread
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
			osDelay(10U);
		}
	}
	*/
	//for testing
	for(;;){
		if(task3 >= 5000000){
			break;
		}
		else{
			task3+= 1;
		}
	}
	osThreadTerminate(tid3_Thread);
	
}