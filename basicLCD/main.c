#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h"
#include "Thread.h"

int main(void){

	
	GLCD_Init();
	
	GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, 1, "     LCD Demo   ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, 1, "       testing     ");
	
	for(int i=0; i<10000000; i++){
	}
	
	GLCD_DisplayString(2, 0, 1, " after delay");
	
		for(int j=0; j<10000000; j++){
		}
	GLCD_ClearLn(2,1);
	GLCD_DisplayString(2, 0, 1, "after more delay");

	clearFromOtherFunction();
		
	return 0;
	
	
}