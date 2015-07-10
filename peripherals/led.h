// LED Library

// set LEDS_ACTIVE_LOW to 0 for active high

#include <stdint.h>
#include "nrf_gpio.h"

void led_init (uint32_t pin_number);
void led_on (uint32_t pin_number);
void led_off (uint32_t pin_number);
void led_toggle (uint32_t pin_number);
