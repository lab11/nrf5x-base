/*
 * Send advertising, periodically updating advertisement parameters
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "led.h"

#include "simple_ble.h"
#include "simple_adv.h"

// function prototypes
static void advertise_with_next_parameter_set(bool);

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x00,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = "ADV TEST",
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS), // this default doesn't matter, as we are changing it
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};

static uint16_t advertising_interval = 0;

// Select a payload size and advertising interval, and begin advertisement
static void advertise_with_next_parameter_set (bool init) {
    static ble_gap_adv_params_t adv_parameters = {0};
    uint32_t err_code;

    bool change_interval = true;
    bool change_payload_len = true;

    // modify the advertising interval
    if (change_interval) {
        // stop advertising and reset parameters
        sd_ble_gap_adv_stop();
        memset(&adv_parameters, 0, sizeof(adv_parameters));

        // non-connectable and non-scannable advertisements
        adv_parameters.type = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;

        // no filter policy
        adv_parameters.fp = BLE_GAP_ADV_FP_ANY;
        adv_parameters.p_whitelist = NULL;

        // advertising interval
        //advertising_interval = interval_list[interval_list_index];
        //advertising_interval = 533; // 333.125 ms
        advertising_interval = 180; // 112.5 ms
        adv_parameters.interval = advertising_interval;

        err_code = sd_ble_gap_adv_start(&adv_parameters);
        if (err_code != NRF_ERROR_INVALID_STATE) {
            // ignore Invalid State responses. Occurs when start is called twice
            APP_ERROR_CHECK(err_code);
        }
    }

    // modify the advertising payload
    if (change_payload_len) {
        // TODO: allow a variable payload length

        if (init) {
            ble_after_adv_event();
        }
    }
}

#define LED_PIN 13

// After a BLE advertisement event callback
void ble_after_adv_event (void) {
    static uint32_t counter = 0;

    static uint8_t advdata[31] = {
        0x1a,                         // 26 bytes after this one
        0xff,                         // manufacturer-specific data
        0x65, 0x78, 0x33, 0x2c,       // ex3,
        0x61, 0x69, 0x2c,             // ai, (advertisement interval)
        0x00, 0x00, 0x2c,             // <ADV_INTERVAL>,
        0x63, 0x74, 0x2c,             // ct, (counter value)
        0x00, 0x00, 0x00, 0x00, 0x2c, // <COUNTER>,
        0x70, 0x64, 0x2c,             // pd, (padding)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // <PADDING>
    };

    static uint8_t srdata[31] = {
        0x1e, 0x09,
        0x21, 0x21, 0x21, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a // !!!alphabet
    };

    // insert advertising interval
    advdata[10] = (advertising_interval >> 8) & 0xFF;
    advdata[9]  = (advertising_interval     ) & 0xFF;

    // insert counter
    advdata[18] = (counter >> 24) & 0xFF;
    advdata[17] = (counter >> 16) & 0xFF;
    advdata[16] = (counter >>  8) & 0xFF;
    advdata[15] = (counter      ) & 0xFF;
    counter++;

    // Pass encoded advertising data and/or scan response data to the stack.
    uint32_t err_code = sd_ble_gap_adv_data_set(advdata, 27, srdata, 0);
    APP_ERROR_CHECK(err_code);

    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
}

void ble_error (uint32_t err_code) {
    led_on(LED_PIN);
}

void advertising_start(void) {
    // do nothing
}

int main (void) {

    // Setup BLE
    simple_ble_init(&ble_config);

    led_init(LED_PIN);

    // Setup timer to change parameters
    //timer_begin();

    // Start advertising with initial parameters
    advertise_with_next_parameter_set(true);

    while (1) {
        power_manage();
    }
}
