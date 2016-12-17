/* Blink
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nrf.h"
#include "nrf_sdm.h"
#include "app_timer.h"
#include "softdevice_handler.h"


#define LED 13

// Some constants about timers
#define BLINK_TIMER_PRESCALER              0  // Value of the RTC1 PRESCALER register.
#define BLINK_TIMER_OP_QUEUE_SIZE          4  // Size of timer operation queues.

// How long before the timer fires.
#define BLINK_RATE     APP_TIMER_TICKS(500, BLINK_TIMER_PRESCALER) // Blink every 0.5 seconds

// Timer data structure
APP_TIMER_DEF(blink_timer);


// Timer callback
static void timer_handler (void* p_context) {
    led_toggle(LED);
}

// Setup timer
static void timer_init(void)
{
    uint32_t err_code;

    // Initialize timer module.
    APP_TIMER_INIT(BLINK_TIMER_PRESCALER,
                   BLINK_TIMER_OP_QUEUE_SIZE,
                   false);

    // Create a timer
    err_code = app_timer_create(&blink_timer,
                                APP_TIMER_MODE_REPEATED,
                                timer_handler);
    APP_ERROR_CHECK(err_code);
}

// Start the blink timer
static void timer_start(void) {
    uint32_t err_code;

    // Start application timers.
    err_code = app_timer_start(blink_timer, BLINK_RATE, NULL);
    APP_ERROR_CHECK(err_code);
}

int main(void) {

    // Initialize.
    led_init(LED);
    led_off(LED);

    // Need to set the clock to something
    nrf_clock_lf_cfg_t clock_lf_cfg = {
        .source        = NRF_CLOCK_LF_SRC_RC,
        .rc_ctiv       = 16,
        .rc_temp_ctiv  = 2,
        .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM};

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    timer_init();
    timer_start();

    // Enter main loop.
    while (1) {
        sd_app_evt_wait();
    }
}
