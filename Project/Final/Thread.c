#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include "KBD.h"
#include "mainMenu.h"
#include "gallery.h"
#include "game.h"
#include "usbdmain.h"


void MainMenuThread( void const *argument);
void GalleryThread(  void const *argument);
void MP3PlayerThread( void const *argument);
void GameThread(  void const *argument);

osThreadId mainMenuId;
osThreadDef(MainMenuThread, osPriorityAboveNormal, 1, 0);

osThreadId galleryThreadId;
osThreadDef(GalleryThread, osPriorityNormal, 1, 0);

osThreadId mp3PlayerThreadId;
osThreadDef(MP3PlayerThread, osPriorityNormal, 1, 0);

osThreadId gameThreadId;
osThreadDef(GameThread, osPriorityNormal, 1, 0);

// global variable for current picture in gallery
uint8_t currentPic;

int Init_Thread (void) {
	// create all our threads
  mainMenuId = osThreadCreate (osThread(MainMenuThread), NULL);
	galleryThreadId = osThreadCreate (osThread(GalleryThread), NULL);
	mp3PlayerThreadId = osThreadCreate (osThread(MP3PlayerThread), NULL);
	gameThreadId = osThreadCreate (osThread(GameThread), NULL);
	// if any of the threads not created, return -1 (failure)
  if((!mainMenuId) || (!galleryThreadId) || (!mp3PlayerThreadId) || (!gameThreadId)) return(-1); 
  return(0);
}

void delay(void);

void MainMenuThread( void const *argument){
	displayMainMenu();
	uint32_t joyStick;
	//uint32_t joyStickPrev = 0U;
	unsigned int currentSelection = 1;
	unsigned int previousSelection = 1;
	unsigned int appSelectedBoolean = 0;
	selectCursor(currentSelection);
	
	while (1) {                                /* Loop forever                  */
		if(appSelectedBoolean==0){
			joyStick = get_button(); 
			
			// these cases handle navigating the main menu
			if(joyStick == KBD_DOWN){
				if(previousSelection!=3){
					if(previousSelection!=currentSelection){
						previousSelection = currentSelection;
					}
					currentSelection+=1;
					clearCursor(previousSelection);
					selectCursor(currentSelection);
				}
				else{
					selectCursor(3);
				}
			}
			else if(joyStick == KBD_UP){
				if(previousSelection!=1){
					if(previousSelection!=currentSelection){
						previousSelection = currentSelection;
					}
					currentSelection-=1;
					clearCursor(previousSelection);
					selectCursor(currentSelection);
				}
				else{
					selectCursor(1);
				}
				
			}
			// this case handles selecting an app
			else if(joyStick == KBD_SELECT){
				appSelectedBoolean = 1;
			}
		}
		else{
			// switch to the given app based on currentSelection, raise that thread's priority to high, set mainMenu thread to normal
			// and wait to receive a signal back from that thread. Once that app is exited, it will set its own priority back to normal,
			// set the mainMenu thread to high and signal to it.
			switch(currentSelection){
				// Gallery, switch to thread with flag 0x01
				case 1:
					osThreadSetPriority(galleryThreadId, osPriorityHigh);
					osSignalSet(galleryThreadId, 0x01);
					osSignalWait(0x04, osWaitForever);
					// once we've returned from the given app, set its priority back to normal
					osThreadSetPriority(galleryThreadId, osPriorityNormal);
					break;
				// MP3 Player, switch to thread with flag 0x02
				case 2:
					osThreadSetPriority(mp3PlayerThreadId, osPriorityHigh);
					osSignalSet(mp3PlayerThreadId, 0x02);
					osSignalWait(0x04, osWaitForever);
					// once we've returned from the given app, set its priority back to normal
					osThreadSetPriority(mp3PlayerThreadId, osPriorityNormal);
					break;
				// Game, switch to thread with flag 0x03
				case 3:
					osThreadSetPriority(gameThreadId, osPriorityHigh);
					osSignalSet(gameThreadId, 0x03);
					osSignalWait(0x04, osWaitForever);
					// once we've returned from the given app, set its priority back to normal
					osThreadSetPriority(gameThreadId, osPriorityNormal);
					break;
			}
			
			appSelectedBoolean = 0;
			displayMainMenu();
		}
	}
}

void GalleryThread(  void const *argument){
	uint8_t exitApp = 0;
	uint32_t joyStick;
	uint8_t clearImg = 1;
	uint8_t selectedImg = 1;
	currentPic = 1;
	initializeGallery();
	while(1){
		if(exitApp==1){
			// then exit this app, reinitialize it if we return
			exitApp = 0; // set this (software) flag back to 0, 
			//if we return, we don't want the infinite loop to go back to the main menu right away, which is what will happen if it remains 1
			osSignalSet(mainMenuId, 0x04); 
			osSignalWait(0x01, osWaitForever);
			initializeGallery();
		}
		else{
			joyStick = get_button();
			// if the clearImg flag is set to 1, it means in the previous iteration of the while loop
			// the joystick was used to navigate to either the next or previous picture, therefore
			// set the flag back to 0 and display the picture that's value corresponds to currentPic
			// which is adjusted by the joystick input handling below
			if(clearImg==1){
				clearImg = 0;
				displayPicture(currentPic);
			}
			// exit the gallery
			if(joyStick==KBD_LEFT) {
				exitApp = 1;
			}
			// JOYSTICK INPUT HANDLING AS MENTIONED ABOVE
			// next picture
			else if(joyStick == KBD_DOWN){
				// the if block here is to handle the value of currentPic staying between 1 and 3
				if(currentPic == 3){
					currentPic = 1;
				}
				else{
					currentPic += 1;
				}
				clearImg = 1;
			}
			// previous picture
			else if(joyStick == KBD_UP){
				// the if block here is to handle the value of currentPic staying between 1 and 3
				if(currentPic == 1){
					currentPic = 3;
				}
				else{
					currentPic -= 1;
				}
				clearImg = 1;
			}
			// call this function so that the while loop detecting joystick inputs doesn't quickly
			// scroll through the pictures
			delay();
		}
		
	}
}
	
void MP3PlayerThread( void const *argument){
	uint8_t exitApp = 0;
	uint8_t audioRunning = 0;
	uint32_t joyStick;
	while(1){
		if(exitApp==1){
			// then exit this app, reinitialize it if we return
			exitApp = 0; // set this (software) flag back to 0, 
			//if we return, we don't want the infinite loop to go back to the main menu right away, which is what will happen if it remains 1
			osSignalSet(mainMenuId, 0x04);
			osSignalWait(0x02, osWaitForever);
			
		}
		else{
			joyStick = get_button();
			if(joyStick==KBD_LEFT){ 
				exitApp = 1;
				//suspendUsbAudio();
			}
			if(exitApp == 0 && audioRunning==0) {
				audioRunning = usbAudio();
			}
		}
	}
}
	
void GameThread(  void const *argument){
	uint8_t exitApp = 0;
	uint32_t joyStick;
	while(1){
		if(exitApp==1){
			// then exit this app, reinitialize it if we return
			exitApp = 0; // set this (software) flag back to 0, 
			//if we return, we don't want the infinite loop to go back to the main menu right away, which is what will happen if it remains 1
			osSignalSet(mainMenuId, 0x04);
			osSignalWait(0x03, osWaitForever);
		}
		else{
			joyStick = get_button();
			if(joyStick == KBD_SELECT) exitApp = 1;
			//int gameReturn = game();
			if(game()==0) exitApp = 1;
		}
	}
}


void delay(void){
	int i,j;
	for(i=0; i<4000; i++){
		for(j=0; j<500; j++){}
	}
}
