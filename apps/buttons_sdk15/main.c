// Buttons app
//
// Use buttons and switches to control LEDs

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"


#define LED0 NRF_GPIO_PIN_MAP(0,16)
#define LED1 NRF_GPIO_PIN_MAP(0,17)
#define LED2 NRF_GPIO_PIN_MAP(0,19)

#define BTN0 NRF_GPIO_PIN_MAP(0,22)
#define BTN1 NRF_GPIO_PIN_MAP(0,23)
#define SW0  NRF_GPIO_PIN_MAP(0,24)
#define SW1  NRF_GPIO_PIN_MAP(0,25)

int main(void) {


  // Initialize.
  uint32_t leds[3] = {LED0, LED1, LED2};
  for (int i=0; i<3; i++) {
    nrf_gpio_cfg_output(leds[i]);
    nrf_gpio_pin_set(leds[i]);
  }
  uint32_t btns[4] = {BTN0, BTN1, SW0, SW1};
  nrf_gpio_pin_pull_t pulls[4] = {NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_NOPULL};
  for (int i=0; i<4; i++) {
    nrf_gpio_cfg_input(btns[i], pulls[i]);
  }

  // Enter main loop.
  while (1) {
    if (nrf_gpio_pin_read(BTN0) || nrf_gpio_pin_read(BTN1)) {
      nrf_gpio_pin_clear(LED0);
    } else {
      nrf_gpio_pin_set(LED0);
    }

    if (nrf_gpio_pin_read(SW0)) {
      nrf_gpio_pin_clear(LED1);
    } else {
      nrf_gpio_pin_set(LED1);
    }

    if (nrf_gpio_pin_read(SW1)) {
      nrf_gpio_pin_clear(LED2);
    } else {
      nrf_gpio_pin_set(LED2);
    }
  }
}

