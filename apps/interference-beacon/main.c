/*
 * Send advertisements as quickly as possible.
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

#define LED_PIN 13

// function prototypes
static void setup_advertising(uint16_t);
void ble_after_adv_event(void);
void ble_new_epoch(void);

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
static uint16_t epoch = 0;
static uint32_t data_in_epoch = 0;
static uint16_t packet_in_data = 0;
static uint32_t tx_estimate = 0;

static uint16_t packets_per_data = 10;
static bool start_new_epoch = false;

static void setup_advertising(uint16_t packets_per_second) {

    // stop advertising and reset parameters
    sd_ble_gap_adv_stop();
    ble_gap_adv_params_t adv_parameters;
    memset(&adv_parameters, 0, sizeof(adv_parameters));

    // non-connectable and non-scannable advertisements
    adv_parameters.type = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;

    // no filter policy
    adv_parameters.fp = BLE_GAP_ADV_FP_ANY;
    adv_parameters.p_whitelist = NULL;

    // advertising interval
    uint16_t rate = (1000 - packets_per_second*5)/packets_per_second;
    if (rate < 100) {
        rate = 100; // 100 ms; fastest NONCONN interval
    }
    advertising_interval = MSEC_TO_UNITS(rate, UNIT_0_625_MS);
    adv_parameters.interval = advertising_interval;

    uint32_t err_code = sd_ble_gap_adv_start(&adv_parameters);
    if (err_code != NRF_ERROR_INVALID_STATE) {
        // ignore Invalid State responses. Occurs when start is called twice
        APP_ERROR_CHECK(err_code);
    }

    // set first advertisement data
    ble_after_adv_event();
}

// After a BLE advertisement event callback
void ble_after_adv_event (void) {

    static uint8_t advdata[31] = {
        0x1e,                         // 30 bytes after this one
        0xff,                         // manufacturer-specific data
        0x65, 0x78, 0x34, 0x2c,       // ex4,
        0x61, 0x69, 0x2c,             // ai, (advertisement interval)
        0x00, 0x00, 0x2c,             // <ADV_INTERVAL>,
        0x73, 0x6e, 0x2c,             // sn, (seqeunce number values)
        0x00, 0x00, 0x2c,             // <epoch number>
        0x00, 0x00, 0x00, 0x00, 0x2c, // <data in epoch number>
        0x00, 0x00, 0x2c,             // <packet in data number>
        0x00, 0x00, 0x00, 0x00, 0x2c, // <number of transmissions estimated this epoch>
    };

    static uint8_t srdata[31] = {
        0x1e, 0x09,
        0x21, 0x21, 0x21, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a // !!!alphabet
    };

    // insert advertising interval
    advdata[10] = (advertising_interval >> 8) & 0xFF;
    advdata[9]  = (advertising_interval     ) & 0xFF;

    // insert epoch number
    advdata[16] = (epoch >> 8) & 0xFF;
    advdata[15] = (epoch     ) & 0xFF;

    // insert data in epoch number
    advdata[21] = (data_in_epoch >> 24) & 0xFF;
    advdata[20] = (data_in_epoch >> 16) & 0xFF;
    advdata[19] = (data_in_epoch >>  8) & 0xFF;
    advdata[18] = (data_in_epoch      ) & 0xFF;

    // insert packet in data number
    advdata[24] = (packet_in_data >> 8) & 0xFF;
    advdata[23] = (packet_in_data     ) & 0xFF;

    // insert transmission count estimate
    advdata[29] = (tx_estimate >> 24) & 0xFF;
    advdata[28] = (tx_estimate >> 16) & 0xFF;
    advdata[27] = (tx_estimate >>  8) & 0xFF;
    advdata[26] = (tx_estimate      ) & 0xFF;

    // Pass encoded advertising data and/or scan response data to the stack.
    uint32_t err_code = sd_ble_gap_adv_data_set(advdata, 27, srdata, 0);
    APP_ERROR_CHECK(err_code);

    // check if we should roll to next cycle
    packet_in_data++;
    if (packet_in_data >= packets_per_data) {
        packet_in_data = 0;
        data_in_epoch++;

        if (start_new_epoch) {
            start_new_epoch = false;
            data_in_epoch = 0;
            epoch++;

            ble_new_epoch();
        }
    }

    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
    led_toggle(LED_PIN);
}

void ble_new_epoch(void) {
    // stop advertising
    // start a timer
    // start scanning for transmissions
    // when timer fires
    // update tx_estimate
    // update packets_per_data as necessary
    // restart advertising, call setup_advertising again
    // start a timer for next epoch
}

void ble_error (uint32_t err_code) {
    led_on(LED_PIN);
}

int main (void) {

    // Setup BLE
    simple_ble_init(&ble_config);

    led_init(LED_PIN);

    // Start an epoch timer
    //TODO

    // Configure advertising parameters and start advertising
    setup_advertising(packets_per_data);

    while (1) {
        power_manage();
    }
}
