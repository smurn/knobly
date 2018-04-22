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

#ifndef KBLY_MUTEX_H_
#define KBLY_MUTEX_H_

#include <stdbool.h>

// We use this mutex implementation when we need protected code sections but 
// cannot affort to diable the interrupts for the entire section.

// Type of mutex.
typedef volatile bool kbly_Mutex;

// Attempts to lock the mutex. It will not bock, but return true if the mutex
// was aquired.
bool kbly_try_lock(kbly_Mutex* mutex);

// Unlocks the mutex
void kbly_unlock(kbly_Mutex* mutex);

#endif
