/* Blink
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"


#define LED NRF_GPIO_PIN_MAP(0,5)

int main(void) {

    // Initialize.
    nrf_gpio_cfg_output(LED);
    nrf_gpio_pin_set(LED);

    // Enter main loop.
    while (1) {
      nrf_delay_ms(500);
      nrf_gpio_pin_toggle(LED);
    }
}
