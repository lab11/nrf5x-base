#pragma once

#include <stdint.h>

// Max number of advertisements to iterate through
#ifndef MULTI_ADV_MAX_CONFIG_FUNCTIONS
#define MULTI_ADV_MAX_CONFIG_FUNCTIONS 3
#endif

// Function call that configures the new advertisement content
typedef void (*multi_adv_configure_f)();

uint32_t multi_adv_init (uint32_t switch_interval_ms);
uint32_t multi_adv_register_config (multi_adv_configure_f config_function);
uint32_t multi_adv_start ();
uint32_t multi_adv_stop ();
