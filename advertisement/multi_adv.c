#include <stdint.h>

#include "nrf_error.h"
#include "nordic_common.h"
#include "app_timer.h"

#include "multi_adv.h"

// Keep track of the function calls that setup the various advertisements
static multi_adv_configure_f adv_config_functions[MULTI_ADV_MAX_CONFIG_FUNCTIONS] = {NULL};
static uint8_t adv_config_len = 0;

// Current index of advertisement to advertise.
static uint8_t adv_config_index = 0;

// Save the switching interval
static uint32_t multi_adv_interval_ms = 1000;

// Timer state
APP_TIMER_DEF(multi_adv_timer);

// Timer callback for when it's time to switch advertisements.
static void multi_adv_timer_handler (void* p_context) {
	// Increment the index
	adv_config_index = (adv_config_index + 1) % adv_config_len;

	// Call that function to update the advertisement in the softdevice
	adv_config_functions[adv_config_index]();
}


// Initialize the multi_adv_module. Basically setup a timer.
uint32_t multi_adv_init (uint32_t switch_interval_ms) {
	uint32_t err;

	// Save this parameter
	multi_adv_interval_ms = switch_interval_ms;

	err = app_timer_create(&multi_adv_timer,
	                       APP_TIMER_MODE_REPEATED,
	                       multi_adv_timer_handler);
	return err;
}

// Register a new advertisement to rotate through.
// This function takes a function that will configure the nRF with the new
// advertisement.
uint32_t multi_adv_register_config (multi_adv_configure_f config_function) {
	// Check that we haven't hit max advertisements yet
	if (adv_config_len == MULTI_ADV_MAX_CONFIG_FUNCTIONS) {
		return NRF_ERROR_NO_MEM;
	}

	// Add this as a advertisement configure function
	adv_config_functions[adv_config_len] = config_function;
	adv_config_len++;

	return NRF_SUCCESS;
}

// Enable switching advertisements
uint32_t multi_adv_start () {
	uint32_t err;

	err = app_timer_start(multi_adv_timer,
	                      APP_TIMER_TICKS(multi_adv_interval_ms, 0),
	                      NULL);
	return err;
}

// Stop switching advertisements
uint32_t multi_adv_stop () {
	return app_timer_stop(multi_adv_timer);
}
