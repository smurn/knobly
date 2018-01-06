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

// Next report to send in round robin fashion.
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


// Sends a report to the host. If there is a dirty report, we'll send that one.
// If not, then we send either send a report in round-robin fashin or we don't
// send a report at all.
static void send_report(bool round_robin){

    if (USBDeviceState < CONFIGURED_STATE){
        // USB is not ready for transfers yet.
        return;
    }

    if (USBSuspendControl == 1){
        // Device is suspended. Host won't receive data.
        return;
    }

    if (HIDTxHandleBusy(prev_transmission)){
        // The previous transfer is still in progress.
        return;
    }
    
    // Find a dirty report to send.
    if (kblyhid_js0_dirty){
        report_buffer = kblyhid_joystick0;
        kblyhid_js0_dirty = 0x00;
    }else if (kblyhid_js1_dirty){
        report_buffer = kblyhid_joystick1;
        kblyhid_js1_dirty = 0x00;
    }else if (kblyhid_js2_dirty){
        report_buffer = kblyhid_joystick2;
        kblyhid_js2_dirty = 0x00;
    }else if (kblyhid_js3_dirty){
        report_buffer = kblyhid_joystick3;
        kblyhid_js3_dirty = 0x00;
    }else if (kblyhid_js4_dirty){
        report_buffer = kblyhid_joystick4;
        kblyhid_js4_dirty = 0x00;
        
    }else if (round_robin){
        
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
        
    }else{
        return; // Nothing to send
    }

    // Transmit. 
    prev_transmission = HIDTxPacket(JOYSTICK_EP, 
            (uint8_t*)&report_buffer, sizeof(kblyhid_Report));

}

void kblyhid_loop(void){
    send_report(true);
}

void kblyhid_flush(void){
    
    // Send the first dirty report. If there are multiple, kblyhid_ontransfer()
    // will send those one after the other.
    send_report(false);
}

// Sends a dirty report to the host.
// Invoked when a previous transfer has finished (EREVENT_TRANSFER) or
// after we scanned the button matrix (via kblyhid_flush).
void kblyhid_ontransfer(void){
    
    // Send out the next dirty report, if there is one.
    send_report(false);
}


