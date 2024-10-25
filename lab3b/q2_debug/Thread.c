
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <string.h>
/*----------------------------------------------------------------------------
 *      Sample threads
 *---------------------------------------------------------------------------*/
 
//global variables
unsigned int memoryAccessCounter = 0;
unsigned int cpuManagementAccessCounter = 0;
unsigned int appInterfaceCounter = 0;
unsigned int devManagerCounter = 0;
unsigned int userInterfaceCounter = 0;

// global mutex logger for appInterface and deviceManager
osMutexId  loggerMutexId; // Mutex ID
osMutexDef (loggerMutex);    // Declare mutex

// char buffer for logger
char  loggerBuffer[128];

void MemManage (void const *argument); 
void CPUManage (void const *argument); 
void AppInterface (void const *argument); 
void DeviceManager (void const *argument); 
void UserInterface (void const *argument); 
void createLoggerMutex(void);

osThreadId mem_id; // thread id
osThreadDef (MemManage, osPriorityHigh, 1, 0);                   

osThreadId cpu_id; // thread id
osThreadDef (CPUManage, osPriorityHigh, 1, 0);                   

osThreadId appint_id; // thread id
osThreadDef (AppInterface, osPriorityAboveNormal, 1, 0);                   

osThreadId devman_id; // thread id
osThreadDef (DeviceManager, osPriorityAboveNormal, 1, 0);                   

osThreadId ui_id; // thread id
osThreadDef (UserInterface, osPriorityNormal, 1, 0);                   


int Init_Thread (void) {
	
	// create mutex in init method, once the app interface thread is created, it will aquire mutex and tell device manager to wait
	createLoggerMutex();
	
	
	
	mem_id = osThreadCreate (osThread(MemManage), NULL);
	cpu_id = osThreadCreate (osThread(CPUManage), NULL);
	appint_id = osThreadCreate (osThread(AppInterface), NULL);
	devman_id = osThreadCreate (osThread(DeviceManager), NULL);
	ui_id  = osThreadCreate (osThread(UserInterface), NULL);
	
  if(!mem_id || !cpu_id || !appint_id || !devman_id || !ui_id) return(-1);
  
	// create a mutex to ensure app interface executes before device manager
	
	
  return(0);
}


void MemManage (void const *argument){
	// bitband address
	volatile unsigned long *GPIO_PORT1_LED31 = (volatile unsigned long *)0x233806FC;
	memoryAccessCounter += 1;
	// bitband computation
	*GPIO_PORT1_LED31 = 1;
	// pass control to cpu manager
	osThreadYield();
	osDelay(100);
	// terminate
	osThreadTerminate(osThreadGetId());
	
}

void CPUManage (void const *argument){
	// variables for barrel shifting
	int r1 = 1, r2 = 0, r3 = 5;
	// increment its counter
	cpuManagementAccessCounter += 1;
	// conditional execution and barrel shifting
	
	while(r2 <= 0x18){
		if((r1 - r2) > 0){
		  r1 = r1 + 2;
			r2 = r1 + (r3*4);
			r3 = r3/2;
	}
		else{
			r2 = r2 + 1;
		}
	}
	osThreadYield();
	osThreadTerminate(osThreadGetId());
}

void AppInterface (void const *argument){
	const char *firstHalfString = "First half |";
	for(;;){
		osStatus status = osMutexWait(loggerMutexId, osWaitForever);
		if(status == osOK) break;
		osDelay(10);
	}
	strcpy(loggerBuffer, firstHalfString);
	osThreadYield();
	appInterfaceCounter += 1;
}

void DeviceManager (void const *argument){
	const char *secondHalfString = " Second half!";
	for(;;){
		osStatus status = osMutexWait(loggerMutexId, osWaitForever);
		if(status == osOK) break;
		osDelay(10);
	}
	devManagerCounter += 1;
	osDelay(100);
	osThreadTerminate(osThreadGetId());
}

void UserInterface (void const *argument){
	userInterfaceCounter += 1;
	osDelay(100);
	osThreadTerminate(osThreadGetId());
}

void createLoggerMutex(void){
	loggerMutexId = osMutexCreate(osMutex(loggerMutex));
}