/* Blink with simple timer
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nordic_common.h"
#include "softdevice_handler.h"

#include "simple_timer.h"

// Need pin number for LED
#define LED0 18
#define LED1 19
#define LED2 20


// Timer callback
static void timer_handler0 (void* p_context) {
    led_toggle(LED0);
}
static void timer_handler1 (void* p_context) {
    led_toggle(LED1);
}
static void timer_handler2 (void* p_context) {
    led_toggle(LED2);
}

int main(void) {

    // Initialize.
    led_init(LED0);
    led_init(LED1);
    led_init(LED2);
    led_off(LED0);
    led_off(LED1);
    led_off(LED2);

    // Need to set the clock to something
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION, false);

    // Call the simple timer API
    simple_timer_init();
    simple_timer_start(1000, timer_handler0);
    simple_timer_start(500,  timer_handler1);
    simple_timer_start(250,  timer_handler2);

    // Enter main loop.
    while (1) {
        sd_app_evt_wait();
    }
}
