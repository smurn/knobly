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

#ifndef _KBLY_HID_H_
#define _KBLY_HID_H_

#include <stdint.h>

//#include "kbly_config.h"

/*
 * HID report sent to the host.
 * The first byte is the report id and must not be changed.
 * The four bytes a, b, c, and d include the states of the buttons. 
 * Not all 32 buttons are in use. Some are marked as padding in the
 * HID descriptor.
 */
typedef struct kblyhid_Report{
    uint8_t report_id;
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
} kblyhid_Report;

/*
 * HID reports for the five joysticks.
 * Changes are transmitted to the host automatically.
 */
extern kblyhid_Report kblyhid_joystick0;
extern kblyhid_Report kblyhid_joystick1;
extern kblyhid_Report kblyhid_joystick2;
extern kblyhid_Report kblyhid_joystick3;
extern kblyhid_Report kblyhid_joystick4;

/*
 * If non-zero, prioritize sending that report out.
 */
extern uint8_t kblyhid_js0_dirty;
extern uint8_t kblyhid_js1_dirty;
extern uint8_t kblyhid_js2_dirty;
extern uint8_t kblyhid_js3_dirty;
extern uint8_t kblyhid_js4_dirty;

/*
 * Initializes the data structures.
 */
void kblyhid_init(void);

/**
 * Invoke this whenever a transfer has completed.
 */
void kblyhid_ontransfer(void);

/**
 * Send dirty reports to host.
 * Invoke this after making changes to the reports.
 */
void kblyhid_flush(void);

/**
 * Invoke regularly. Interval does not matter much, won't take 
 * more than a few cycles.
 */
void kblyhid_loop(void);

#endif // _KBLY_HID_H_

