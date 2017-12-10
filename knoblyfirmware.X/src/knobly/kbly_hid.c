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

void kblyhid_init(){
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
/*
void kblyhid_enable(){
    USBEnableEndpoint(JOYSTICK_EP,
            USB_IN_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP); 
    
    //kblyhid_ontransfer();
}

void kblyhid_loop(){
    // We have to kick-start the transfers. We normally start them
    // once the previous one has finished using an interrupt, but somewhere
    // we got to start the first one. 
    kblyhid_ontransfer();
}

static bool flag = false;

*/

void kblyhid_ontransfer(){
    
    
    
    // disable high prio interrupt until we are done here.
    // We cal this method from the USB interrupt as well as from the
    // low-prio timer (as a fall back). Make sure they don't step on each
    // other.
    //INTCONbits.GIEH = 0;
    
    
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
    
    
    // disable high prio interrupt until we are done here.
    //INTCONbits.GIEH = 0;
    

    if (HIDTxHandleBusy(prev_transmission)){
        //INTCONbits.GIEH = 1;
        return;
    }
    
    
    
    
    //flag = !flag;
    //LATCbits.LC6 = flag;

    kblyhid_Report* report = NULL;
    
    // If one is marked as dirty, we send that one.
    
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
    
    // None are marked dirty, send them round-robin
    
    if (report == NULL){
        
        next++;
        if (next >= 5) next = 0;
        
        switch(next){
        case 0:
            report = &kblyhid_joystick0;
            break;    
        case 1:
            report = &kblyhid_joystick1;
            break;   
        case 2:
            report = &kblyhid_joystick2;
            break;   
        case 3:
            report = &kblyhid_joystick3;
            break;   
        case 4:
            report = &kblyhid_joystick4;
            break;   
        }
    }

    report_buffer = *report;

    prev_transmission = HIDTxPacket(JOYSTICK_EP, 
            (uint8_t*)&report_buffer, sizeof(kblyhid_Report));

    // reenable high prio interrupt.
    //INTCONbits.GIEH = 1;
}
