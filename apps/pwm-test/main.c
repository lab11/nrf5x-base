/* PWM an LED.
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "low_power_pwm.h"

// Need pin number for LED
#define LED0 13

static low_power_pwm_t pwm;

void pwm_init () {
    uint32_t err_code;
    low_power_pwm_config_t low_power_pwm_config;

    APP_TIMER_DEF(timer);
    low_power_pwm_config.active_high = false;
    low_power_pwm_config.period = 255;
    low_power_pwm_config.bit_mask = 1 << LED0;
    low_power_pwm_config.p_timer_id = &timer;

    err_code = low_power_pwm_init(&pwm, &low_power_pwm_config, NULL);
    APP_ERROR_CHECK(err_code);
    err_code = low_power_pwm_duty_set(&pwm, 99);
    APP_ERROR_CHECK(err_code);

    err_code = low_power_pwm_start(&pwm, pwm.bit_mask);
    APP_ERROR_CHECK(err_code);
}

int main(void) {

    // Initialize.
    led_init(LED0);
    led_off(LED0);

    // Need to set the clock to something
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION, false);

    // Start APP_TIMER to generate timeouts.
    APP_TIMER_INIT(0, 3, NULL);

    pwm_init();

    // Enter main loop.
    while (1) {
        sd_app_evt_wait();
    }
}
