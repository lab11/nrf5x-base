/* Test interrupt on a pin
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "app_gpiote.h"

// Need pin number for LED
#define LED 18

// Interrupt pin number
#define INTERRUPT_PIN 10

// Need this for the app_gpiote library
app_gpiote_user_id_t gpiote_user;


void interrupt_handler (uint32_t pins_l2h, uint32_t pins_h2l) {
    led_toggle(LED);
}

int main(void) {

    // Initialize.
    led_init(LED);
    led_off(LED);

    // For 3 users of GPIOTE
    APP_GPIOTE_INIT(3);

    // Register us as one user
    app_gpiote_user_register(&gpiote_user,
                             1<<INTERRUPT_PIN,   // Which pins we want the interrupt for low to high
                             1<<INTERRUPT_PIN,   // Which pins we want the interrupt for high to low
                             interrupt_handler);

    // Ready to go!
    app_gpiote_user_enable(gpiote_user);

    // Enter main loop.
    while (1) {
        sd_app_evt_wait();
    }
}
