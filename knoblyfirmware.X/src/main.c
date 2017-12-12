/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */


#include "usb.h"
#include "usb_device_hid.h"
#include "kbly_hid.h"
#include "kbly_matrix.h"
#include "kbly_config.h"
#include "device_config.h"
#include <string.h>
#include <xc.h>

void init(void){
    // Configure Timer2, we use its interrupt this to scan the inputs
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
    
    // Configure Timer0, we its interrupts to periodically send reports,
    // even if they haven't changed.
    T0CONbits.T0CS = 0;     // Timer mode
    T0CONbits.PSA = 0;      // Enable pre-scaler
    T0CONbits.TMR0ON = 1;   // Timer0 ON
    T0CONbits.T0PS2 = 1;    // /256 prescaler
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 1;
    T0CONbits.T08BIT = 0;   // 16-bit-counter 
    INTCONbits.TMR0IF = 0;  // Clear interrupt
    INTCON2bits.TMR0IP = 0; // Low-priority
    INTCONbits.TMR0IE = 1;  // Enable interrupt

    // Enable interrupt handling
    RCONbits.IPEN = 1;      // High and low priority interrupts.
    INTCONbits.GIEL = 1;    // Enable low prio interrupts.
    INTCONbits.GIEH = 1;    // Enable high-prio interrupts. 
}

void main(void){
    kblyhid_init();
    kblymatrix_init();
    init();

    USBDeviceInit();
    USBDeviceAttach();

    while(1){
        #if defined(USB_POLLING)
            USBDeviceTasks();
        #endif

        if( USBGetDeviceState() < CONFIGURED_STATE ){
            continue;
        }

        if( USBIsDeviceSuspended() == true ){
            continue;
        }
    }
}


void interrupt low_priority low_prio_interrupt(void){
    if (PIR1bits.TMR2IF){        
        kblymatrix_scan();
        kblyhid_flush();
        PIR1bits.TMR2IF = 0;
    }
    if (INTCONbits.TMR0IF){ 
        kblyhid_loop();
        INTCONbits.TMR0IF = 0;
    }
}



void interrupt SYS_InterruptHigh(void)
{
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif
}
