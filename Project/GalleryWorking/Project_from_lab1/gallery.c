#include "GLCD.h"
#include "LPC17xx.h"  
#include "stdint.h"
#include "IMG_6156.c" // picNum 1
#include "IMG_6157.c" // picNum 2
#include "IMG_6158.c" // picNum 3

// an array that holds the different numbers that represent the pictures in our gallery
uint8_t gallery[3] = {1,2,3};
unsigned char *picNames [] = {
	(unsigned char *) "|     Awais.jpg    |",
	(unsigned char *) "|      MCB.jpg     |",
	(unsigned char *) "|      Code.jpg    |"
};



extern uint8_t currentPic;
// this function clears the current picture and re displays the header and scroll options for our gallery
void clearLCDForGallery(void){
	GLCD_Clear(White);
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(0, 0, 1, ( unsigned char *)"|      Gallery     |");
	GLCD_SetBackColor(Red);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(4, 0, 0, ( unsigned char *)"   down = next picture, up = previous, left = exit   ");
	//GLCD_DisplayString(26, 0, 0, picNames[currentPic-1]);
	GLCD_DisplayString(9, 0, 1, ( unsigned char *)"|                  |");
}

// this function displays a picture in our gallery
void displayPicture(uint8_t picNum){
	// clear the display between each display, so reinitialize the gallery
	clearLCDForGallery();
	switch(picNum){
		case 1:
			GLCD_Bitmap( 90, 58, 150, 147, (unsigned char*) SELFIE_pixel_data);
			break;
		case 2:
			GLCD_Bitmap( 90, 58, 150, 150, (unsigned char*) BOARDPIC_pixel_data);
			break;
		case 3:
			GLCD_Bitmap( 90, 58, 150, 144, (unsigned char*) MYCODEPIC_pixel_data);
			break;
	}
	// display the name of the picture
	GLCD_ClearLn(9, 1);
	GLCD_DisplayString(9, 0, 1, picNames[picNum-1]);
}

// to be called whenever the gallery thread is signalled to execute
void initializeGallery(void){
	currentPic = 1;
	clearLCDForGallery();
	// start the gallery off by displaying the very first 
	displayPicture(currentPic);
}



