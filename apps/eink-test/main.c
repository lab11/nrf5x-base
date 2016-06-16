/* Blink with simple timer
 */

#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "nrf_drv_spi.h"
#include "nrf_delay.h"
#include "app_gpiote.h"
#include "app_util_platform.h"

#include "board.h"
// #include "adxl362.h"

// Need pin number for LED
#define LED0 18
#define LED1 19
#define LED2 20

#define nTC_EN 24
#define nTC_BUSY 23
#define nTC_CS 22

// #define SPI_INSTANCE 0

// #define ACCELEROMETER_INTERRUPT_PIN 5

static nrf_drv_spi_t _spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);

app_gpiote_user_id_t gpiote_user_acc;


// static void acc_interrupt_handler (uint32_t pins_l2h, uint32_t pins_h2l) {
//     if (pins_h2l & (1 << ACCELEROMETER_INTERRUPT_PIN)) {
//         // High to low transition
//         led_toggle(LED0);
//     }
// }

// static void gpio_init (void) {
//     // Need one user: accelerometer
//     APP_GPIOTE_INIT(1);

//     // Register the accelerometer
//     app_gpiote_user_register(&gpiote_user_acc,
//                              1<<ACCELEROMETER_INTERRUPT_PIN,   // Which pins we want the interrupt for low to high
//                              1<<ACCELEROMETER_INTERRUPT_PIN,   // Which pins we want the interrupt for high to low
//                              acc_interrupt_handler);

//     // Enable the interrupt!
//     app_gpiote_user_enable(gpiote_user_acc);
// }

static void spi_init () {
    uint32_t err;

    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG(SPI_INSTANCE);
    // spi_config.frequency = NRF_DRV_SPI_FREQ_1M;
    spi_config.frequency = NRF_DRV_SPI_FREQ_2M;
    // We do need CS pin
    spi_config.ss_pin = nTC_CS;
    // Need CPOL=1 CPHA=1
    spi_config.mode = NRF_DRV_SPI_MODE_3;

    // We want blocking mode
    err = nrf_drv_spi_init(&_spi, &spi_config, NULL);
    APP_ERROR_CHECK(err);
}

static void wait_for_not_busy () {
    uint8_t found_busy_low = 0;
    while (1) {
        uint8_t pin = nrf_gpio_pin_read(nTC_BUSY);
        if (found_busy_low && pin) {
            break;
        }
        if (pin == 0) {
            found_busy_low = 1;
        }
    }
}

int main(void) {

    // Initialize.
    led_init(LED0);
    led_off(LED0);

    // led_init(13);
    // led_off(13);

    // Setup input for busy
    nrf_gpio_cfg_input(nTC_BUSY, NRF_GPIO_PIN_NOPULL);

    // Assert ENABLE
    nrf_gpio_cfg_output(nTC_EN);
    nrf_gpio_pin_clear(nTC_EN);

    // Need to wait 6.5 ms per datasheet (section 5.5)
    nrf_delay_ms(10);

    spi_init();

    // Get device id
    uint8_t tx[6] = {0x30, 0x01, 0x01, 0x00, 0x00, 0x00};
    uint8_t rx[256] = {0};

    // Send
    nrf_drv_spi_transfer(&_spi, tx, 4, NULL, 0);

    // Wait until no longer busy
    wait_for_not_busy();
    // while (nrf_gpio_pin_read(nTC_BUSY) == 0);

    // Receive response
    nrf_drv_spi_transfer(&_spi, NULL, 0, rx, 26);

    wait_for_not_busy();


    uint8_t pic[255];

    // Setup spi comm header
    pic[0] = 0x20;
    pic[1] = 0x01;
    pic[2] = 0x00;

    // How many bytes we want to send.
    pic[3] = 16;

    // Pic header
    pic[4] = 0x33; // 4.41"
    pic[5] = 0x01; // 400px
    pic[6] = 0x90;
    pic[7] = 0x01; // 300px
    pic[8] = 0x2c;
    pic[9] = 0x01; // 1 bit
    pic[10] = 0x02; // image pixel data format type 2
    pic[11] = 0; // reserved
    pic[12] = 0;
    pic[13] = 0;
    pic[14] = 0;
    pic[15] = 0;
    pic[16] = 0;
    pic[17] = 0;
    pic[18] = 0;
    pic[19] = 0;

    // Send header
    nrf_drv_spi_transfer(&_spi, pic, 20, NULL, 0);
    wait_for_not_busy();
    nrf_delay_ms(5);
    nrf_drv_spi_transfer(&_spi, NULL, 0, rx, 2);
    wait_for_not_busy();
    nrf_delay_ms(5);

    uint8_t i;

    pic[3] = 250;
    for (i=4; i<254; i++) {
        if (i % 2 == 0) {
            pic[i] = 0xFF;
        } else {
            pic[i] = 0x00;
        }
    }

    // Display a lot more
    for (i=0; i<30; i++) {
        nrf_drv_spi_transfer(&_spi, pic, 254, NULL, 0);
        wait_for_not_busy();
        nrf_drv_spi_transfer(&_spi, NULL, 0, rx, 2);
        wait_for_not_busy();
    }


    // Display an image
    tx[0] = 0x24;
    tx[1] = 0x01;
    tx[2] = 0x00;

    nrf_drv_spi_transfer(&_spi, tx, 3, NULL, 0);
    wait_for_not_busy();
    nrf_drv_spi_transfer(&_spi, NULL, 0, rx, 2);

    // adxl362_accelerometer_init(&_spi, adxl362_NOISE_NORMAL, true, false, false);

    // uint16_t act_thresh = 0x0222;
    // adxl362_set_activity_threshold(act_thresh);
    // uint16_t inact_thresh = 0x0096;
    // adxl362_set_inactivity_threshold(inact_thresh);

    // uint8_t a_time = 4;
    // adxl362_set_activity_time(a_time);
    // uint8_t ia_time = 30;
    // adxl362_set_inactivity_time(ia_time);

    // adxl362_interrupt_map_t intmap_2;

    // intmap_2.DATA_READY = 0;
    // intmap_2.FIFO_READY = 0;
    // intmap_2.FIFO_WATERMARK = 0;
    // intmap_2.FIFO_OVERRUN = 0;
    // intmap_2.ACT = 0;
    // intmap_2.INACT = 0;
    // intmap_2.AWAKE = 1;
    // intmap_2.INT_LOW = 1;
    // adxl362_config_INTMAP(&intmap_2, false);

    // adxl362_config_interrupt_mode(adxl362_INTERRUPT_LOOP, true , true);
    // adxl362_activity_inactivity_interrupt_enable();

    // adxl362_read_status_reg();

    led_on(LED0);

    // gpio_init();

    // Enter main loop.
    while (1) {
        // sd_app_evt_wait();
    }
}
