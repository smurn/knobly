/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

/** INCLUDES *******************************************************/
#include "usb.h"
#include "usb_device_hid.h"
#include "kbly_hid.h"
#include "kbly_matrix.h"
#include "kbly_config.h"
#include "device_config.h"
#include <string.h>

void init(void){
    // Configure Timer2 we use its interrupt this to scan the inputs
    T2CONbits.TMR2ON = 1;   // Timer2 ON
    T2CONbits.T2CKPS0 = 0;  // Prescale /16
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2OUTPS0 = 0; // Postscale /1
    T2CONbits.T2OUTPS1 = 0;
    T2CONbits.T2OUTPS2 = 0;
    T2CONbits.T2OUTPS3 = 0;
    PR2 = KBLY_SCAN_INTERVAL * 12 / 16;
    PIE1bits.TMR2IE = 1;    // Enable Interrupt
    IPR1bits.TMR2IP = 0;    // Low-Priority
    PIR1bits.TMR2IF = 0;    // Clear the interrupt
    
#if KBLY_SCAN_INTERVAL * 12 / 16 >= 255
#error "scan interval too long. Adjust prescaler."
#endif

    // Enable interrupt handling
    RCONbits.IPEN = 1;      // High and low priority interrupts.
    INTCONbits.GIEL = 1;    // Enable low prio interrupts.
    INTCONbits.GIEH = 1;    // Enable high-prio interrupts. 
}

void main(void)
{
    kblyhid_init();
    kblymatrix_init();
    init();

    USBDeviceInit();
    USBDeviceAttach();

    while(1)
    {
        #if defined(USB_POLLING)
            // Interrupt or polling method.  If using polling, must call
            // this function periodically.  This function will take care
            // of processing and responding to SETUP transactions
            // (such as during the enumeration process when you first
            // plug in).  USB hosts require that USB devices should accept
            // and process SETUP packets in a timely fashion.  Therefore,
            // when using polling, this function should be called
            // regularly (such as once every 1.8ms or faster** [see
            // inline code comments in usb_device.c for explanation when
            // "or faster" applies])  In most cases, the USBDeviceTasks()
            // function does not take very long to execute (ex: <100
            // instruction cycles) before it returns.
            USBDeviceTasks();
        #endif

        /* If the USB device isn't configured yet, we can't really do anything
         * else since we don't have a host to talk to. So jump back to the
         * top of the while loop. */
        if( USBGetDeviceState() < CONFIGURED_STATE )
        {
            /* Jump back to the top of the while loop. */
            continue;
        }

        /* If we are currently suspended, then we need to see if we need to
         * issue a remote wakeup. In either case, we shouldn't process any
         * keyboard commands since we aren't currently communicating to the host
         * thus just continue back to the start of the while loop. */
        if( USBIsDeviceSuspended() == true )
        {
            /* Jump back to the top of the while loop. */
            continue;
        }
            
        //send_report();
            
    }//end while
}//end main

/*******************************************************************************
 End of File
*/

void interrupt low_priority low_prio_interrupt(void){
    if (PIR1bits.TMR2IF){        
        kblyhid_ontransfer();
        kblymatrix_scan();
        PIR1bits.TMR2IF = 0;
    }
}



void interrupt SYS_InterruptHigh(void)
{
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif
}
