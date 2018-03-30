/* Blink
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nrf.h"
#include "nrf_delay.h"


#define LED NRF_GPIO_PIN_MAP(0,5)

int main(void) {

    // Initialize.
    led_init(LED);
    led_off(LED);

    // Enter main loop.
    while (1) {
      nrf_delay_ms(500);
      led_toggle(LED);
    }
}
