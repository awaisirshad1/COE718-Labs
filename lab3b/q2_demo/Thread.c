#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include "LPC17xx.h" 

/*
#define LED28 (*((volatile unsigned long *)0x233806F4))
#define LED31 (*((volatile unsigned long *)0x233806F0))
#define LED2 (*((volatile unsigned long *)0x23380A88))
#define LED3 (*((volatile unsigned long *)0x23380ABA))
*/



/* Global Variables for Each Task */
unsigned int memAccessCounter = 0;
unsigned int cpuAccessCounter = 0;
unsigned int appCounter = 0;
unsigned int devCounter = 0;
unsigned int userCounter = 0;
char logger[100];

/* Mutex for Logger Access */
osMutexId loggerMutex;
osMutexDef(loggerMutex);

/* Function Prototypes */
void MemManager(void const *argument);
void CPUManager(void const *argument);
void AppInterface(void const *argument);
void DeviceManager(void const *argument);
void UserInterface(void const *argument);

/* Thread Definitions */
osThreadDef(MemManager, osPriorityRealtime, 1, 0);
osThreadDef(CPUManager, osPriorityRealtime, 1, 0);
osThreadDef(AppInterface, osPriorityHigh, 1, 0);
osThreadDef(DeviceManager, osPriorityHigh, 1, 0);
osThreadDef(UserInterface, osPriorityLow, 1, 0);

/* Thread IDs */
osThreadId tid_Mem, tid_CPU, tid_App, tid_Dev, tid_User;

/* Thread Initialization */
int Init_Thread(void) {
    loggerMutex = osMutexCreate(osMutex(loggerMutex));
    if (!loggerMutex) return -1;
    tid_Mem = osThreadCreate(osThread(MemManager), NULL);
    tid_CPU = osThreadCreate(osThread(CPUManager), NULL);
    tid_App = osThreadCreate(osThread(AppInterface), NULL);
    tid_Dev = osThreadCreate(osThread(DeviceManager), NULL);
    tid_User = osThreadCreate(osThread(UserInterface), NULL);

    return (tid_Mem && tid_CPU && tid_App && tid_Dev && tid_User) ? 0 : -1; 
}

/* Memory Management Task */
void MemManager(void const *argument) {
	volatile unsigned long *bitband = (unsigned long *)0x42480100;
	char buffer[100];
	while (1) {
		memAccessCounter++;
		//bit band computation
		
		*bitband = memAccessCounter % 2;
		sprintf(buffer, "Bit: %d", *bitband);
		osThreadYield();
		osDelay(100); // Optional delay
	}

}

/* CPU Management Task */
void CPUManager(void const *argument) {
	char buffer[100];
	unsigned int value;
  while (1) {
	cpuAccessCounter++;
// Simulate CPU-specific operations (e.g., barrel shift)
		value = cpuAccessCounter << 2;
		sprintf(buffer,"Value: %d", value);
		osThreadYield();
		osDelay(100); // delay
	}

}

/* Application Interface Task */
void AppInterface(void const *argument) {
	while (1) {
		appCounter++;
		osMutexWait(loggerMutex, osWaitForever);
		strcpy(logger, "Hello");
		osMutexRelease(loggerMutex);
		osThreadYield();
		osDelay(100);
	}
}

/* Device Management Task */
void DeviceManager(void const *argument) {
  char buffer[20];
	while (1) {
		devCounter++;
    osMutexWait(loggerMutex, osWaitForever);
    strcat(logger, " world");
		osMutexRelease(loggerMutex);
		sprintf(buffer,"Device Mgmt ctr: %d", devCounter);
		// Yield control after processing
    osThreadYield();
    osDelay(100);
	}
}

/* User Interface Task */
void UserInterface(void const *argument) {
  char buffer[20];  
	while (1) {
		userCounter++;
		sprintf(buffer, "Num Users: %d", userCounter);
	// Yield control after processing
    osThreadYield();
    osDelay(100);
	}
}
