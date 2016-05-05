/* Test interrupt on a pin
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "nrf_drv_gpiote.h"
#include "simple_ble.h"


// Need pin number for red LED on nucleum
#define LED_RED 18
#define LED_BLUE 19

// Interrupt pin number
#define INTERRUPT_PIN 23

static void interrupt_handler (nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    if(pin == INTERRUPT_PIN) {led_toggle(LED_BLUE);} // Pin is returned to use as decimal number and not a bitmap. fuck yeah!
    led_toggle(LED_RED);
}

int main(void) {

    // Initialize.
    led_init(LED_RED);
    led_init(LED_BLUE);

    // Initialize GPIOTE driver
    // configuration values can be found in nrf_drv_config.h under GPIOTE_ENABLED
    uint32_t err_code;
    if(!nrf_drv_gpiote_is_init()) {
        err_code = nrf_drv_gpiote_init();
        if(err_code != NRF_SUCCESS) { /* Handle failure case*/ }
    }

    // Initialize interrupt on pin 23.
    nrf_drv_gpiote_in_config_t test_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false);
    test_config.pull = NRF_GPIO_PIN_PULLUP;
    err_code = nrf_drv_gpiote_in_init(23, &test_config, interrupt_handler);
    if(err_code != NRF_SUCCESS) { /* Handle failure case */ }
    nrf_drv_gpiote_in_event_enable(23, true); // enable interrupt

    // Enter main loop.
    while (1) {
        power_manage();
    }
}
