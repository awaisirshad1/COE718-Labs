#include "LPC17xx.h"                       
#include "GLCD.h"

#define __FI        1 


// 1 - Gallery, 2 - MP3 Player, 3 - Game
uint8_t currentSelection;
uint8_t previousSelection;

void displayMainMenu(void){
	currentSelection = 3;
	
	GLCD_Clear(White);                         
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, ( unsigned char *)"COE718 Media Centre ");
  GLCD_DisplayString(1, 0, __FI, ( unsigned char *)"   Navigate Menu:   ");
	
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Black);
	GLCD_DisplayString(2, 0, __FI, ( unsigned char *)"--------------------");
  GLCD_DisplayString(3, 0, __FI, ( unsigned char *)"|      Gallery     |");
	GLCD_DisplayString(4, 0, __FI, ( unsigned char *)"--------------------");
	GLCD_DisplayString(5, 0, __FI, ( unsigned char *)"|     MP3 Player   |");
	GLCD_DisplayString(6, 0, __FI, ( unsigned char *)"--------------------");
	GLCD_DisplayString(7, 0, __FI, ( unsigned char *)"|       Game       |");
	GLCD_DisplayString(8, 0, __FI, ( unsigned char *)"--------------------");
	GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
	GLCD_DisplayString(9, 0, __FI, ( unsigned char *)"                     ");
}


// this select cursor only updates the 2 lines of the main menu that we need to, minimizing LCD operations
void selectCursor(unsigned int num){
	switch(num){
		case 1:
			// Gallery
			GLCD_ClearLn(3,__FI);
			GLCD_SetBackColor(Blue);
			GLCD_SetTextColor(White);
			GLCD_DisplayString(3, 0, __FI, ( unsigned char *)"|      Gallery     |");
			break;
		case 2:
			// MP3 player
			GLCD_ClearLn(5,__FI);
			GLCD_SetBackColor(Blue);
			GLCD_SetTextColor(White);
			GLCD_DisplayString(5, 0, __FI, ( unsigned char *)"|     MP3 Player   |");
			break;
		case 3:
			// Game
			GLCD_ClearLn(7,__FI);
			GLCD_SetBackColor(Blue);
			GLCD_SetTextColor(White);
			GLCD_DisplayString(7, 0, __FI, ( unsigned char *)"|       Game       |");
			break;
	}
}

void clearCursor(unsigned int num){
	switch(num){
		case 1:
			// Take cursor off Gallery
			GLCD_ClearLn(3,__FI);
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(3, 0, __FI, ( unsigned char *)"|      Gallery     |");
			break;
		case 2:
			// Take cursor off MP3 Player
			GLCD_ClearLn(5,__FI);
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(5, 0, __FI, ( unsigned char *)"|     MP3 Player   |");
			break;
		case 3:
			// Take cursor off Game
			GLCD_ClearLn(7,__FI);
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(7, 0, __FI, ( unsigned char *)"|       Game       |");
			break;
	}
}
