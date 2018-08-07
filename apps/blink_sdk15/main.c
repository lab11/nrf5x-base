// Blink app
//
// Blinks all pins on Buckler

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"


#define LED0 NRF_GPIO_PIN_MAP(0,16)
#define LED1 NRF_GPIO_PIN_MAP(0,17)
#define LED2 NRF_GPIO_PIN_MAP(0,19)

int main(void) {

  uint32_t leds[3] = {LED0, LED1, LED2};

  // Initialize.
  for (int i=0; i<3; i++) {
    uint32_t LED = leds[i];
    nrf_gpio_cfg_output(LED);
    nrf_gpio_pin_set(LED);
  }

  // Enter main loop.
  while (1) {
    for (int i=0; i<3; i++) {
      uint32_t LED = leds[i];
      nrf_gpio_pin_toggle(LED);
      nrf_delay_ms(500);
    }
  }
}

