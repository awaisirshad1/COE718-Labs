#include "LPC17xx.h"                        /* LPC17xx definitions */
#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbaudio.h"

extern  void SystemClockUpdate(void);
extern uint32_t SystemCoreClock;  
void usbAudio (void);
