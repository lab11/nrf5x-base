#ifndef __SIMPLE_TIMER_H
#define __SIMPLE_TIMER_H

#include <stdint.h>
#include "app_timer.h"

/*******************************************************************************
 * USAGE
 *
 *   static void timer_handler (void* p_context) {
 *     ...
 *   }
 *
 *   void start_timer () {
 *     simple_timer_init();
 *     simple_timer_start(1000, timer_handler);
 *   }
 *
 */


// Call this once to init the timer subsystem
// This only needs to be called if you are NOT calling simple_ble_init If you
//  are using simple_ble, calling this again will still work, but wastes ~500
//  bytes of RAM
void simple_timer_init ();

// Call this to create and start a timer at the given period
uint32_t simple_timer_start (uint32_t milliseconds,
                             app_timer_timeout_handler_t callback);

#endif
