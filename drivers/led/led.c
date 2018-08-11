// LED Library

#include <stdint.h>
#include "nrf_gpio.h"

#ifndef LEDS_ACTIVE_LOW
#define LEDS_ACTIVE_LOW 1
#endif


void led_init (uint32_t pin_number) {
    nrf_gpio_cfg_output(pin_number);
#if LEDS_ACTIVE_LOW
    nrf_gpio_pin_set(pin_number);
#else
    nrf_gpio_pin_clear(pin_number);
#endif
}

void led_on (uint32_t pin_number) {
#if LEDS_ACTIVE_LOW
    nrf_gpio_pin_clear(pin_number);
#else
    nrf_gpio_pin_set(pin_number);
#endif
}

void led_off (uint32_t pin_number) {
#if LEDS_ACTIVE_LOW
    nrf_gpio_pin_set(pin_number);
#else
    nrf_gpio_pin_clear(pin_number);
#endif
}

void led_toggle (uint32_t pin_number) {
    nrf_gpio_pin_toggle(pin_number);
}
