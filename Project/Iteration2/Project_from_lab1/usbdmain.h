#include "LPC17xx.h"                        /* LPC17xx definitions */
#include "type.h"
#include "stdint.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbaudio.h"

extern  void SystemCoreClockUpdate(void);
extern uint32_t SystemCoreClock;  

int usbAudio(void);
void suspendUsbAudio(void);