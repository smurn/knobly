/*
 * File:   kbly_matrix.c
 * Author: stefan
 *
 * Created on 11. Juni 2016, 11:50
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "kbly_config.h"
#include "kbly_hid.h"

#define N_STEPS 28

// We want enough states for KBLY_DEBOUNCE_DELAY us. We have one scan
// every KBLY_SCAN_INTERVAL us. Since we cycle through the rows, we get
// one sample each KBLY_SCAN_INTERVAL * KBLY_MATRIX_ROWS.
#define N_STATES (KBLY_DEBOUNCE_DELAY / KBLY_SCAN_INTERVAL / N_STEPS)
#if N_STATES > 10
#error "TOO MANY DEBOUNCE STATES"
#endif


// We store the last N_STATES for all 7 button rows.
static uint8_t debounce_states[14][N_STATES];

static uint8_t current_state = 0;
//static uint8_t next_state[14];
    

//
// Debounces 8 buttons.
// Has to be called at a regular interval.
//
// @param raw: Unbounced button state.
// @param group: Button group id. Used to access the internal state
//    of the debouncer.
static uint8_t debounce_btn_group(uint8_t raw, uint8_t previous, uint8_t group){
    
    //return raw;
    
    debounce_states[group][current_state] = raw;
    
    uint8_t all_on = 0xFF;
    uint8_t one_on = 0x00;
    for(uint8_t i = 0; i < N_STATES; i++){
        uint8_t s = debounce_states[group][i];
        all_on &= s;
        one_on |= s;
    }
    //uint8_t debounced = all_on | (previous & one_on);
    
    return all_on;
}

static uint8_t states[3];
static uint8_t Aprimes[3];
static uint8_t Bprimes[3];
static uint8_t CWarmeds[3];
static uint8_t CCWarmeds[3];

static void encoder(uint8_t A, uint8_t B, uint8_t count_half, uint8_t group, uint8_t* dest_cw, uint8_t* dest_ccw, uint8_t* dest_dirty){

    uint8_t S = states[group];
    uint8_t Ap = Aprimes[group];
    uint8_t Bp = Bprimes[group];
    uint8_t CW_ARMED = CWarmeds[group];
    uint8_t CCW_ARMED = CCWarmeds[group];
    
    // Has there been a half rotation?
    uint8_t HALF_R = ~(A^B) & (A^S) & (Ap^Bp);
    
    // 1 for CW, 0 for CCW (only makes sense if R)
    uint8_t D = S ^ Bp;
    
     // Next state
    S = A&B | S&(A^B);
    
    // Has there been a half CW rotation?
    uint8_t HALF_CW = HALF_R & D;
    
    // Has there been a half CCW rotation?
    uint8_t HALF_CCW = HALF_R & ~D;
    
    // Have we made a full CW rotation?
    uint8_t FULL_CW = HALF_CW & CW_ARMED;
    CW_ARMED = CW_ARMED & ~HALF_R | HALF_CW & ~CW_ARMED;
    
    // Have we made a full CCW rotation?
    uint8_t FULL_CCW = HALF_CCW & CCW_ARMED;
    CCW_ARMED = CCW_ARMED & ~HALF_R | HALF_CCW & ~CCW_ARMED;   
            
    // Either full or half rotations, depending on encoder type.
    uint8_t ACTUAL_CW = FULL_CW | HALF_CW & count_half;
    uint8_t ACTUAL_CCW = FULL_CCW | HALF_CCW & count_half;
    uint8_t ACTUAL_R = ACTUAL_CW | ACTUAL_CCW;
    
    // Toggle the button on those that have rotated.
    *dest_cw = *dest_cw ^ ACTUAL_CW;
    *dest_ccw = *dest_ccw ^ ACTUAL_CCW;
    
    *dest_dirty |= ACTUAL_R;
    
    states[group] = S;
    Aprimes[group] = A;
    Bprimes[group] = B;
    CWarmeds[group] = CW_ARMED;
    CCWarmeds[group] = CCW_ARMED;
}


void kblymatrix_init(){
    
    // PORT A: Row0  Row1  Row2  Row3  Row4  Row5  n.c.  n.c.
    // PORT C: n.c.  Row6  n.c.  n.c.  n.c.  n.c.  n.c.  n.c.
    // PORT E: Enc0  Enc1  Enc2  n.c.  n.c.  n.c.  n.c.  n.c.
    
    // PORT B: Col0  Col2  Col4  Col6  Col8  Col10 Col12 Col14
    // PORT D: Col1  Col3  Col5  Col7  Col9  Col11 Col13 Col15

    ADCON1 = 0xFF;  // Disable A/D converters)
    CMCON = 0x07;   // Disable comparators (RA0-3) 

    // Configure Port A
    TRISAbits.TRISA0 = 0;   // Row0
    TRISAbits.TRISA1 = 0;   // Row1
    TRISAbits.TRISA2 = 0;   // Row2
    TRISAbits.TRISA3 = 0;   // Row3
    TRISAbits.TRISA4 = 0;   // Row4
    TRISAbits.TRISA5 = 0;   // Row5
    LATA = 0xFF;
    
    // Configure PORT B
    TRISB = 0xFF;           // All inputs (even columns)
    INTCON2bits.RBPU = 0;   // Enable pull-ups
    
    // Configure Port C
    TRISCbits.TRISC1 = 0;   // Row6
    TRISCbits.TRISC6 = 0;   // debug output
    LATCbits.LATC1 = 1;
    
    // Configure PORT D
    TRISD = 0xFF;           // All inputs (odd columns)
    PORTEbits.RDPU = 1;     // Enable pull-ups

    // Configure Port E
    TRISEbits.TRISE0 = 0;   // Enc0
    TRISEbits.TRISE1 = 0;   // Enc1
    TRISEbits.TRISE2 = 0;   // Enc2
    LATEbits.LATE0 = 1;
    LATEbits.LATE1 = 1;
    LATEbits.LATE2 = 1;
    
    // Enable Row0
    LATAbits.LATA0 = 0;
}

static uint8_t next_row = 99;  // first row should be row 0

void kblymatrix_scan(){
    
    next_row++;
    if (next_row >= N_STEPS){
        next_row = 0;
    }
    
    // read complete row (row should already be activated.)
     uint8_t raw_a = ~PORTB;
     uint8_t raw_b = ~PORTD;
     
     switch(next_row){

        case 0: // row0            
            current_state++;
            if (current_state >= N_STATES) current_state = 0;
            
            kblyhid_joystick0.a = debounce_btn_group(raw_a, kblyhid_joystick0.a, 0);
            kblyhid_joystick0.b = debounce_btn_group(raw_b, kblyhid_joystick0.b, 1);
            LATAbits.LATA0 = 1; // Disable Row0
            LATEbits.LATE0 = 0; // Enable Enc0
            break;

        case 1: // enc0
            encoder(raw_a, raw_b, 0xFF, 0, &kblyhid_joystick3.c, &kblyhid_joystick3.d, &kblyhid_js3_dirty);
            LATEbits.LATE0 = 1; // Disable Enc0
            LATEbits.LATE1 = 0; // Enable Enc1
            break;

        case 2: // enc1
            encoder(raw_a, raw_b, 0x30, 1, &kblyhid_joystick4.a, &kblyhid_joystick4.b, &kblyhid_js4_dirty);
            LATEbits.LATE1 = 1; // Disable Enc1
            LATEbits.LATE2 = 0; // Enable Enc2
            break;

        case 3: // enc2
            encoder(raw_a, raw_b, 0xFC, 2, &kblyhid_joystick4.c, &kblyhid_joystick4.d, &kblyhid_js4_dirty);
            LATEbits.LATE2 = 1; // Disable Enc2
            LATAbits.LATA1 = 0; // Enable Row1
            break;

        case 4: // row1
            kblyhid_joystick0.c = debounce_btn_group(raw_a, kblyhid_joystick0.c, 2);
            kblyhid_joystick0.d = debounce_btn_group(raw_b, kblyhid_joystick0.d, 3);
            LATAbits.LATA1 = 1; // Disable Row1
            LATEbits.LATE0 = 0; // Enable Enc0
            break;

        case 5: // enc0
            encoder(raw_a, raw_b, 0xFF, 0, &kblyhid_joystick3.c, &kblyhid_joystick3.d, &kblyhid_js3_dirty);
            LATEbits.LATE0 = 1; // Disable Enc0
            LATEbits.LATE1 = 0; // Enable Enc1
            break;

        case 6: // enc1
            encoder(raw_a, raw_b, 0x30, 1, &kblyhid_joystick4.a, &kblyhid_joystick4.b, &kblyhid_js4_dirty);
            LATEbits.LATE1 = 1; // Disable Enc1
            LATEbits.LATE2 = 0; // Enable Enc2
            break;

        case 7: // enc2
            encoder(raw_a, raw_b, 0xFC, 2, &kblyhid_joystick4.c, &kblyhid_joystick4.d, &kblyhid_js4_dirty);
            LATEbits.LATE2 = 1; // Disable Enc2
            LATAbits.LATA2 = 0; // Enable Row2
            break;

        case 8: // row2
            kblyhid_joystick1.a = debounce_btn_group(raw_a, kblyhid_joystick1.a, 4);
            kblyhid_joystick1.b = debounce_btn_group(raw_b, kblyhid_joystick1.b, 5);
            LATAbits.LATA2 = 1; // Disable Row2
            LATEbits.LATE0 = 0; // Enable Enc0
            break;

        case 9: // enc0
            encoder(raw_a, raw_b, 0xFF, 0, &kblyhid_joystick3.c, &kblyhid_joystick3.d, &kblyhid_js3_dirty);
            LATEbits.LATE0 = 1; // Disable Enc0
            LATEbits.LATE1 = 0; // Enable Enc1
            break;

        case 10: // enc1
            encoder(raw_a, raw_b, 0x30, 1, &kblyhid_joystick4.a, &kblyhid_joystick4.b, &kblyhid_js4_dirty);
            LATEbits.LATE1 = 1; // Disable Enc1
            LATEbits.LATE2 = 0; // Enable Enc2
            break;

        case 11: // enc2
            encoder(raw_a, raw_b, 0xFC, 2, &kblyhid_joystick4.c, &kblyhid_joystick4.d, &kblyhid_js4_dirty);
            LATEbits.LATE2 = 1; // Disable Enc2
            LATAbits.LATA3 = 0; // Enable Row3
            break;

        case 12: // row3
            kblyhid_joystick1.c = debounce_btn_group(raw_a, kblyhid_joystick1.c, 6);
            kblyhid_joystick1.d = debounce_btn_group(raw_b, kblyhid_joystick1.d, 7);
            LATAbits.LATA3 = 1; // Disable Row3
            LATEbits.LATE0 = 0; // Enable Enc0
            break;

        case 13: // enc0
            encoder(raw_a, raw_b, 0xFF, 0, &kblyhid_joystick3.c, &kblyhid_joystick3.d, &kblyhid_js3_dirty);
            LATEbits.LATE0 = 1; // Disable Enc0
            LATEbits.LATE1 = 0; // Enable Enc1
            break;

        case 14: // enc1
            encoder(raw_a, raw_b, 0x30, 1, &kblyhid_joystick4.a, &kblyhid_joystick4.b, &kblyhid_js4_dirty);
            LATEbits.LATE1 = 1; // Disable Enc1
            LATEbits.LATE2 = 0; // Enable Enc2
            break;

        case 15: // enc2
            encoder(raw_a, raw_b, 0xFC, 2, &kblyhid_joystick4.c, &kblyhid_joystick4.d, &kblyhid_js4_dirty);
            LATEbits.LATE2 = 1; // Disable Enc2
            LATAbits.LATA4 = 0; // Enable Row4
            break;

        case 16: // row4
            kblyhid_joystick2.a = debounce_btn_group(raw_a, kblyhid_joystick2.a, 8);
            kblyhid_joystick2.b = debounce_btn_group(raw_b, kblyhid_joystick2.b, 9);
            LATAbits.LATA4 = 1; // Disable Row4
            LATEbits.LATE0 = 0; // Enable Enc0
            break;

        case 17: // enc0
            encoder(raw_a, raw_b, 0xFF, 0, &kblyhid_joystick3.c, &kblyhid_joystick3.d, &kblyhid_js3_dirty);
            LATEbits.LATE0 = 1; // Disable Enc0
            LATEbits.LATE1 = 0; // Enable Enc1
            break;

        case 18: // enc1
            encoder(raw_a, raw_b, 0x30, 1, &kblyhid_joystick4.a, &kblyhid_joystick4.b, &kblyhid_js4_dirty);
            LATEbits.LATE1 = 1; // Disable Enc1
            LATEbits.LATE2 = 0; // Enable Enc2
            break;

        case 19: // enc2
            encoder(raw_a, raw_b, 0xFC, 2, &kblyhid_joystick4.c, &kblyhid_joystick4.d, &kblyhid_js4_dirty);
            LATEbits.LATE2 = 1; // Disable Enc2
            LATAbits.LATA5 = 0; // Enable Row5
            break;

        case 20: // row5
            kblyhid_joystick2.c = debounce_btn_group(raw_a, kblyhid_joystick2.c, 10);
            kblyhid_joystick2.d = debounce_btn_group(raw_b, kblyhid_joystick2.d, 11);
            LATAbits.LATA5 = 1; // Disable Row5
            LATEbits.LATE0 = 0; // Enable Enc0
            break;

        case 21: // enc0
            encoder(raw_a, raw_b, 0xFF, 0, &kblyhid_joystick3.c, &kblyhid_joystick3.d, &kblyhid_js3_dirty);
            LATEbits.LATE0 = 1; // Disable Enc0
            LATEbits.LATE1 = 0; // Enable Enc1
            break;

        case 22: // enc1
            encoder(raw_a, raw_b, 0x30, 1, &kblyhid_joystick4.a, &kblyhid_joystick4.b, &kblyhid_js4_dirty);
            LATEbits.LATE1 = 1; // Disable Enc1
            LATEbits.LATE2 = 0; // Enable Enc2
            break;

        case 23: // enc2
            encoder(raw_a, raw_b, 0xFC, 2, &kblyhid_joystick4.c, &kblyhid_joystick4.d, &kblyhid_js4_dirty);
            LATEbits.LATE2 = 1; // Disable Enc2
            LATCbits.LATC1 = 0; // Enable Row6
            break;

        case 24: // row6
            kblyhid_joystick3.a = debounce_btn_group(raw_a, kblyhid_joystick3.a, 12);
            kblyhid_joystick3.b = debounce_btn_group(raw_b, kblyhid_joystick3.b, 13);
            LATCbits.LATC1 = 1; // Disable Row6
            LATEbits.LATE0 = 0; // Enable Enc0
            break;

        case 25: // enc0
            encoder(raw_a, raw_b, 0xFF, 0, &kblyhid_joystick3.c, &kblyhid_joystick3.d, &kblyhid_js3_dirty);
            LATEbits.LATE0 = 1; // Disable Enc0
            LATEbits.LATE1 = 0; // Enable Enc1
            break;

        case 26: // enc1
            encoder(raw_a, raw_b, 0x30, 1, &kblyhid_joystick4.a, &kblyhid_joystick4.b, &kblyhid_js4_dirty);
            LATEbits.LATE1 = 1; // Disable Enc1
            LATEbits.LATE2 = 0; // Enable Enc2
            break;

        case 27: // enc2
            encoder(raw_a, raw_b, 0xFC, 2, &kblyhid_joystick4.c, &kblyhid_joystick4.d, &kblyhid_js4_dirty);
            LATEbits.LATE2 = 1; // Disable Enc2
            LATAbits.LATA0 = 0; // Enable Row0
            break;



     }
     
     
/*
    switch(next_row){
        case 0: // Row0
            current_state++;
            if (current_state >= N_STATES) current_state = 0;
            kblyhid_joystick0.a = debounce_btn_group(raw_a, kblyhid_joystick0.a, 0);
            kblyhid_joystick0.b = debounce_btn_group(raw_b, kblyhid_joystick0.b, 1);
            
            LATAbits.LATA0 = 1; // Disable Row0
            LATAbits.LATA1 = 0; // Enable Row1
            break;
        case 1: // Row1
            kblyhid_joystick0.c = debounce_btn_group(raw_a, kblyhid_joystick0.c, 2);
            kblyhid_joystick0.d = debounce_btn_group(raw_b, kblyhid_joystick0.d, 3);
            
            LATAbits.LATA1 = 1; // Disable Row1
            LATAbits.LATA2 = 0; // Enable Row2
            break;
        case 2: // Row2
            kblyhid_joystick1.a = debounce_btn_group(raw_a, kblyhid_joystick1.a, 4);
            kblyhid_joystick1.b = debounce_btn_group(raw_b, kblyhid_joystick1.b, 5);
            
            LATAbits.LATA2 = 1; // Disable Row2
            LATAbits.LATA3 = 0; // Enable Row3
            break;
        case 3: // Row3
            kblyhid_joystick1.c = debounce_btn_group(raw_a, kblyhid_joystick1.c, 6);
            kblyhid_joystick1.d = debounce_btn_group(raw_b, kblyhid_joystick1.d, 7);
            
            LATAbits.LATA3 = 1; // Disable Row3
            LATAbits.LATA4 = 0; // Enable Row4
            break;
        case 4: // Row4
            kblyhid_joystick2.a = debounce_btn_group(raw_a, kblyhid_joystick2.a, 8);
            kblyhid_joystick2.b = debounce_btn_group(raw_b, kblyhid_joystick2.b, 9);
            LATAbits.LATA4 = 1; // Disable Row4
            LATAbits.LATA5 = 0; // Enable Row5
            break;
        case 5: // Row5
            kblyhid_joystick2.c = debounce_btn_group(raw_a, kblyhid_joystick2.c, 10);
            kblyhid_joystick2.d = debounce_btn_group(raw_b, kblyhid_joystick2.d, 11);
            LATAbits.LATA5 = 1; // Disable Row5
            LATCbits.LATC1 = 0; // Enable Row6
            break;
        case 6: // Row6
            kblyhid_joystick3.a = debounce_btn_group(raw_a, kblyhid_joystick3.a, 12);
            kblyhid_joystick3.b = debounce_btn_group(raw_b, kblyhid_joystick3.b, 13);
            LATCbits.LATC1 = 1; // Disable Row6
            LATEbits.LATE0 = 0; // Enable Enc0
            break;      
        case 7: // Enc0
            encoder(raw_a, raw_b, 0xFF, 0, &kblyhid_joystick3.c, &kblyhid_joystick3.d, &kblyhid_js3_dirty);
            LATEbits.LATE0 = 1; // Disable Enc0
            LATEbits.LATE1 = 0; // Enable Enc1
            break;
        case 8: // Enc1
            encoder(raw_a, raw_b, 0x30, 1, &kblyhid_joystick4.a, &kblyhid_joystick4.b, &kblyhid_js4_dirty);
            LATEbits.LATE1 = 1; // Disable Enc1
            LATEbits.LATE2 = 0; // Enable Enc2
            break;  
        case 9: // Enc2
            encoder(raw_a, raw_b, 0xFC, 2, &kblyhid_joystick4.c, &kblyhid_joystick4.d, &kblyhid_js4_dirty);
            LATEbits.LATE2 = 1; // Disable Enc0
            LATAbits.LATA0 = 0; // Enable Row0
            break;  
    }
*/
    
}


/*
 * 

 * 
 * We count half-cylces: 00-01-11 gives a +1 during the 01-11 transition.
 * We use the second transition since that's when the encoder snaps into the
 * next detent.
 * 
 * 00-01-11   +1
 * 11-10-00   +1
 * 00-10-11   -1
 * 11-01-00   -1
 * 
 * Left Columns:
 *  - current state: S
 *  - previous input: A', B'
 *  - current input:  A, B
 * 
 * Right Columns:
 *  - next state: S'
 *  - event
 *      +-1: rotation detected
 *      error: rotation too fast or broken encoder
 *      impossible: will not occur, guaranteed by algorithm
 *  
 * 0 00 00      0
 * 0 00 01      0
 * 0 00 10      0
 * 0 00 11      1   error
 * 0 01 00      0
 * 0 01 01      0
 * 0 01 10      0   error
 * 0 01 11      1   +1
 * 0 10 00      0
 * 0 10 01      0   error
 * 0 10 10      0
 * 0 10 11      1   -1
 * 0 11 00      1   impossible
 * 0 11 01      1   impossible
 * 0 11 10      1   impossible
 * 0 11 11      1   impossible
 * 
 * 1 00 00      0   impossible
 * 1 00 01      0   impossible
 * 1 00 10      0   impossible
 * 1 00 11      0   impossible
 * 1 01 00      0   -1
 * 1 01 01      1
 * 1 01 10      1   error
 * 1 01 11      1
 * 1 10 00      0   +1
 * 1 10 01      1   error
 * 1 10 10      1
 * 1 10 11      1
 * 1 11 00      0   error
 * 1 11 01      1
 * 1 11 10      1
 * 1 11 11      1
 * 
 * 
 * Rotation has occured: R = ~(A^B) & (A^S)
 * The first term detects when the current input is either 00 or 11, and
 * the second term checks if that is different from the state we were last in.
 * This will treat 00-11 and 11-00 transitions as rotations. If we don't want
 * that, we can extend it to:
 * Rotation has occured: R = ~(A^B) & (A^S) & (A'^B')
 * 
 * Once we know there is a rotation event, we can decide in which direction:
 * Direction: D = S ^ B'  (true for +1, false for -1)
 * 
 * CW = R & D
 * CCW = R & ~D
 * 
 * Next state: S' = A&B + S&(A^B)
 * 
 * So far, we counted half-cylces. But some encoders have detents only for
 * full cycles. We want to rotate only every second half-cycle. Both
 * half-rotations must be in the same direction.
 * 
 * FULL_CW = CW & CW_ARMED
 * CW_ARMED = CW_ARMED & ~R | CW & ~CW_ARMED
 * ACTUAL_CW = FULL_CW | CW & count_half_cycles
 */

