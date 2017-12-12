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

#include <xc.h>
#include <usb.h>
#include <usb_device_hid.h>
#include <stdint.h>

#include "kbly_hid.h"
//#include "kbly_config.h"
#include "device_config.h"

kblyhid_Report kblyhid_joystick0;
kblyhid_Report kblyhid_joystick1;
kblyhid_Report kblyhid_joystick2;
kblyhid_Report kblyhid_joystick3;
kblyhid_Report kblyhid_joystick4;

uint8_t kblyhid_js0_dirty;
uint8_t kblyhid_js1_dirty;
uint8_t kblyhid_js2_dirty;
uint8_t kblyhid_js3_dirty;
uint8_t kblyhid_js4_dirty;

// This is the one th USB stuff is using.
kblyhid_Report report_buffer @ JOYSTICK_DATA_ADDRESS;


// Handle for transmissions to the host.
static USB_VOLATILE USB_HANDLE prev_transmission = 0;

static USB_VOLATILE uint8_t next = 0;

void kblyhid_init(void){
    kblyhid_joystick0.report_id = 1;
    kblyhid_joystick0.a = 0x00;
    kblyhid_joystick0.b = 0x00;
    kblyhid_joystick0.c = 0x00;
    kblyhid_joystick0.d = 0x00;
    
    kblyhid_joystick1.report_id = 2;
    kblyhid_joystick1.a = 0x00;
    kblyhid_joystick1.b = 0x00;
    kblyhid_joystick1.c = 0x00;
    kblyhid_joystick1.d = 0x00;
    
    kblyhid_joystick2.report_id = 3;
    kblyhid_joystick2.a = 0x00;
    kblyhid_joystick2.b = 0x00;
    kblyhid_joystick2.c = 0x00;
    kblyhid_joystick2.d = 0x00;
    
    kblyhid_joystick3.report_id = 4;
    kblyhid_joystick3.a = 0x00;
    kblyhid_joystick3.b = 0x00;
    kblyhid_joystick3.c = 0x00;
    kblyhid_joystick3.d = 0x00;
    
    kblyhid_joystick4.report_id = 5;
    kblyhid_joystick4.a = 0x00;
    kblyhid_joystick4.b = 0x00;
    kblyhid_joystick4.c = 0x00;
    kblyhid_joystick4.d = 0x00;
}

void kblyhid_loop(void){
    
    // Disable interrupts. 
    di();
    
    // Using do {} while (0) as a try/finally construct.
    do{
        
        if (USBDeviceState < CONFIGURED_STATE){
            // USB is not ready for transfers yet.
            break;
        }
        
        if (USBSuspendControl == 1){
            // Device is suspended. Host won't receive data.
            break;
        }
        
        if (HIDTxHandleBusy(prev_transmission)){
            // The previous transfer is still in progress.
            break;
        }
        
        next++;
        if (next >= 5) next = 0;
        
        switch(next){
        case 0:
            report_buffer = kblyhid_joystick0;
            break;    
        case 1:
            report_buffer = kblyhid_joystick1;
            break;   
        case 2:
            report_buffer = kblyhid_joystick2;
            break;   
        case 3:
            report_buffer = kblyhid_joystick3;
            break;   
        case 4:
            report_buffer = kblyhid_joystick4;
            break;   
        }
        
        // Transmit.
        prev_transmission = HIDTxPacket(JOYSTICK_EP, 
                (uint8_t*)&report_buffer, sizeof(kblyhid_Report));
        
    }while (0);
    
    // Enable interrupts.
    ei();
}

void kblyhid_flush(void){
    kblyhid_ontransfer();
}

// Sends a dirty report to the host.
// Invoked when a previous transfer has finished (EREVENT_TRANSFER) or
// after we scanned the button matrix (via kblyhid_flush).
void kblyhid_ontransfer(void){
    // Disable interrupts. 
    di();
    
    // Using do {} while (0) as a try/finally construct.
    do{
        
        if (USBDeviceState < CONFIGURED_STATE){
            // USB is not ready for transfers yet.
            break;
        }
        
        if (USBSuspendControl == 1){
            // Device is suspended. Host won't receive data.
            break;
        }
        
        if (HIDTxHandleBusy(prev_transmission)){
            // The previous transfer is still in progress.
            break;
        }
        
        kblyhid_Report* report = NULL;

        // Find a dirty report to send.
        if (kblyhid_js0_dirty){
            report = &kblyhid_joystick0;
            kblyhid_js0_dirty = 0x00;
        }
        if (kblyhid_js1_dirty){
            report = &kblyhid_joystick1;
            kblyhid_js1_dirty = 0x00;
        }
        if (kblyhid_js2_dirty){
            report = &kblyhid_joystick2;
            kblyhid_js2_dirty = 0x00;
        }
        if (kblyhid_js3_dirty){
            report = &kblyhid_joystick3;
            kblyhid_js3_dirty = 0x00;
        }
        if (kblyhid_js4_dirty){
            report = &kblyhid_joystick4;
            kblyhid_js4_dirty = 0x00;
        }

        if (report == NULL){
            // All reports are clean.
            break;
        }
        
        // Copy the report we've selected to the memory region accessable
        // by the USB controller.
        report_buffer = *report;
        
        // Transmit.
        prev_transmission = HIDTxPacket(JOYSTICK_EP, 
                (uint8_t*)&report_buffer, sizeof(kblyhid_Report));
      
    } while (0);
    
    // Enable interrupts.
    ei();
}
