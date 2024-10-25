#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <math.h>

/*----------------------------------------------------------------------------
 *      Sample threads
 *---------------------------------------------------------------------------*/
unsigned int counta=0;
unsigned int countb=0;
unsigned int i,j,k;

#define PI 3.141592

volatile char* current_thread; //Global var to track thread
void TaskA (void const *argument); // thread function
void TaskB (void const *argument); //
void TaskC (void const *argument); //
void TaskD (void const *argument); //
void TaskE (void const *argument); //


osThreadId tid_Thread; // thread id
osThreadDef (TaskA, osPriorityAboveNormal, 1, 0);                   // thread object

osThreadId tid2_Thread; // thread id
osThreadDef (TaskB, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid3_Thread; // thread id
osThreadDef (TaskC, osPriorityHigh, 1, 0);                   // thread object

osThreadId tid4_Thread; // thread id
osThreadDef (TaskD, osPriorityAboveNormal, 1, 0);                   // thread object

osThreadId tid5_Thread; // thread id
osThreadDef (TaskE, osPriorityNormal, 1, 0);                   // thread object

unsigned int factorial(unsigned int n){
if (n == 1 || n == 0){
return 1;
}
return n *factorial(n-1);
}


int Init_Thread (void) {

  tid_Thread = osThreadCreate (osThread(TaskA), NULL);
tid2_Thread = osThreadCreate (osThread(TaskB), NULL);
tid3_Thread = osThreadCreate (osThread(TaskC), NULL);
tid4_Thread = osThreadCreate (osThread(TaskD), NULL);
tid5_Thread = osThreadCreate (osThread(TaskE), NULL);

  if(!tid_Thread|| !tid2_Thread || !tid3_Thread || !tid4_Thread || !tid5_Thread)
return(-1);
 
  return(0);
}


void TaskA (void const *argument){
i =0;
char buffer[20];
int A_Result = 0;
for (i = 0; i<255; i++){
A_Result += (i*(i+2));
sprintf(buffer, "A = %d", A_Result);
osDelay(500);
}
osThreadYield();
}


void TaskB (void const *argument){
int i = 1;
double j = 2;
char buffer[20];
float B_Result = 0;
for (i = 1; i<16; i++){
B_Result += (pow(2,i)/factorial(i));
sprintf(buffer, "B = %.3lf", B_Result);
osDelay(500);
}
osThreadYield();
}

void TaskC (void const *argument){
char buffer[20];
float C_Result = 0;
for (i = 1; i<16; i++){
C_Result += ((i+1)/i);
sprintf(buffer, "C = %.3lf", C_Result);
osDelay(500);
}
osThreadYield();
}


void TaskD (void const *argument) {
char buffer[20];
float D_Result = 0;

for (int i =0; i <50; i++){
D_Result = 1 + (5/factorial(1)) + (pow(5,2)/factorial(2)) + (pow(5,3)/factorial(3)) + (pow(5,4)/factorial(4)) + (pow(5,5)/factorial(5)) +  
sprintf(buffer, "D = %.3lf", D_Result);
osDelay(500);
}
osThreadYield();
}


void TaskE (void const *argument) {
float radius = 5.6;
float E_Result = 0;
char buffer[20];
  for (int i = 1; i<13; i++){
E_Result += (i*PI*pow(radius, 2));
sprintf(buffer, "E = %.3lf", E_Result);
osDelay(500);
}
osThreadYield();
}