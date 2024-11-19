#include "GLCD.h"
#include "LPC17xx.h"                       

void initializeGallery(void){
	GLCD_Clear(White);
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(1, 0, 1, ( unsigned char *)"|      Gallery     |");
	//GLCD_WindowMax();
}