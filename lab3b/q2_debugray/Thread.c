#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>

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
void Mem_manage(void const *argument);
void CPU_manage(void const *argument);
void App_interface(void const *argument);
void Device_manage(void const *argument);
void User_Interface(void const *argument);

/* Thread Definitions */
osThreadDef(Mem_manage, osPriorityRealtime, 1, 0);
osThreadDef(CPU_manage, osPriorityRealtime, 1, 0);
osThreadDef(App_interface, osPriorityHigh, 1, 0);
osThreadDef(Device_manage, osPriorityHigh, 1, 0);
osThreadDef(User_Interface, osPriorityLow, 1, 0);

/* Thread IDs */
osThreadId tid_Mem, tid_CPU, tid_App, tid_Dev, tid_User;

/* Thread Initialization */
int Init_Thread(void) {
    loggerMutex = osMutexCreate(osMutex(loggerMutex));
    if (!loggerMutex) return -1;

    tid_Mem = osThreadCreate(osThread(Mem_manage), NULL);
    tid_CPU = osThreadCreate(osThread(CPU_manage), NULL);
    tid_App = osThreadCreate(osThread(App_interface), NULL);
    tid_Dev = osThreadCreate(osThread(Device_manage), NULL);
    tid_User = osThreadCreate(osThread(User_Interface), NULL);

    return (tid_Mem && tid_CPU && tid_App && tid_Dev && tid_User) ? 0 : -1; 
}

/* Memory Management Task */
void Mem_manage(void const *argument) {
char buffer[100];
    while (1) {
memAccessCounter++;
      //bit band computation
      volatile unsigned long *bitband = (unsigned long *)0x42480100;
      *bitband = memAccessCounter % 2;
sprintf(buffer, "Bit: %d", *bitband);
      osThreadYield();
      osDelay(100); // Optional delay
    }

}

/* CPU Management Task */
void CPU_manage(void const *argument) {
char buffer[100];
    while (1) {
cpuAccessCounter++;
// Simulate CPU-specific operations (e.g., barrel shift)
      unsigned int value = cpuAccessCounter << 2;
sprintf(buffer,"Value: %d", value);
      osThreadYield();
      osDelay(100); // delay
    }

}

/* Application Interface Task */
void App_interface(void const *argument) {
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
void Device_manage(void const *argument) {
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
void User_Interface(void const *argument) {
  char buffer[20];  
while (1) {
userCounter++;
sprintf(buffer, "Num Users: %d", userCounter);
// Yield control after processing
    osThreadYield();
    osDelay(100);
    }
}
