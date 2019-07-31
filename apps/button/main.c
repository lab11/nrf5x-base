// Buttons app
//
// Use buttons and switches to control LEDs

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin definitions
#define LED NRF_GPIO_PIN_MAP(0,17)
#define BTN NRF_GPIO_PIN_MAP(0,13)

int main(void) {

  // Initialize.
  nrf_gpio_cfg_output(LED);
  nrf_gpio_pin_set(LED);
  nrf_gpio_cfg_input(BTN, NRF_GPIO_PIN_PULLUP);

  // Enter main loop.
  while (1) {
    if (nrf_gpio_pin_read(BTN)) {
      nrf_gpio_pin_set(LED);
    } else {
      nrf_gpio_pin_clear(LED);
    }
  }
}

