/* Blink with simple timer
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "nrf_drv_spi.h"
#include "app_gpiote.h"

#include "board.h"
#include "adxl362.h"

// Need pin number for LED
#define LED0 25

#define ACCELEROMETER_INTERRUPT_PIN 5

static nrf_drv_spi_t _spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);

app_gpiote_user_id_t gpiote_user_acc;


static void acc_interrupt_handler (uint32_t pins_l2h, uint32_t pins_h2l) {
    if (pins_h2l & (1 << ACCELEROMETER_INTERRUPT_PIN)) {
        // High to low transition
        led_toggle(LED0);
    }
}

static void gpio_init (void) {
    // Need one user: accelerometer
    APP_GPIOTE_INIT(1);

    // Register the accelerometer
    app_gpiote_user_register(&gpiote_user_acc,
                             1<<ACCELEROMETER_INTERRUPT_PIN,   // Which pins we want the interrupt for low to high
                             1<<ACCELEROMETER_INTERRUPT_PIN,   // Which pins we want the interrupt for high to low
                             acc_interrupt_handler);

    // Enable the interrupt!
    app_gpiote_user_enable(gpiote_user_acc);
}

int main(void) {

    // Initialize.
    led_init(LED0);
    led_off(LED0);

    led_init(13);
    led_off(13);

    adxl362_accelerometer_init(&_spi, adxl362_NOISE_NORMAL, true, false, false);

    uint16_t act_thresh = 0x0222;
    adxl362_set_activity_threshold(act_thresh);
    uint16_t inact_thresh = 0x0096;
    adxl362_set_inactivity_threshold(inact_thresh);

    uint8_t a_time = 4;
    adxl362_set_activity_time(a_time);
    uint8_t ia_time = 30;
    adxl362_set_inactivity_time(ia_time);

    adxl362_interrupt_map_t intmap_2;

    intmap_2.DATA_READY = 0;
    intmap_2.FIFO_READY = 0;
    intmap_2.FIFO_WATERMARK = 0;
    intmap_2.FIFO_OVERRUN = 0;
    intmap_2.ACT = 0;
    intmap_2.INACT = 0;
    intmap_2.AWAKE = 1;
    intmap_2.INT_LOW = 1;
    adxl362_config_INTMAP(&intmap_2, false);

    adxl362_config_interrupt_mode(adxl362_INTERRUPT_LOOP, true , true);
    adxl362_activity_inactivity_interrupt_enable();

    adxl362_read_status_reg();

    led_on(LED0);

    gpio_init();

    // Enter main loop.
    while (1) {
        // sd_app_evt_wait();
    }
}
