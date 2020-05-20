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
#include "nrf_log.h"
#include "led.h"

#include "simple_ble.h"
#include "simple_adv.h"

#define LED_PIN 13

// function prototypes
static void timer_handler(void*);
static void advertise_with_next_parameter_set(bool);
static void ble_adv_update(void);

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
static uint32_t adv_count = 0;

// Timer data structure
APP_TIMER_DEF(parameter_timer);

// Timer initialization
static void timer_begin (void) {
    uint32_t err_code;

    // Initialize timer module (settings copied from blink app)
    APP_TIMER_INIT(0, 4, false);

    // Create a timer
    err_code = app_timer_create(&parameter_timer, APP_TIMER_MODE_REPEATED, timer_handler);
    APP_ERROR_CHECK(err_code);

    // Start the timer
    const int timer_interval_ms = 1*1000; // Max is 512 seconds
    err_code = app_timer_start(parameter_timer, APP_TIMER_TICKS(timer_interval_ms, 0), NULL);
    APP_ERROR_CHECK(err_code);
}

static void timer_handler(void* _unused) {
    ble_adv_update();
}

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
        //advertising_interval = 180; // 112.5 ms
        advertising_interval = 1592; // 995 ms
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
            //ble_after_adv_event();
            ble_adv_update();
        }
    }
}

// After a BLE advertisement event callback
//void ble_after_adv_event (void) {

// Trigger update on timer instead of radio hardware.
// It seems to be impossible to tell if radio hardware had a state change due
// to scanning or advertising. Normal hardware would base advertisement
// updates on timers anyways.
// What we lose here is a true counter value that changes with each packet
// rather than with each data.
static void ble_adv_update (void) {
    static uint32_t counter = 0;

    static uint8_t advdata[31] = {
        0x1e,                         // 30 bytes after this one
        0xff,                         // manufacturer-specific data
        0x65, 0x78, 0x33, 0x2c,       // ex3,
        0x61, 0x69, 0x2c,             // ai, (advertisement interval)
        0x00, 0x00, 0x2c,             // <ADV_INTERVAL>,
      //0x63, 0x74, 0x2c,             // ct, (counter value)
      //0x00, 0x00, 0x00, 0x00, 0x2c, // <COUNTER>,
        0x72, 0x64, 0x2c,             // rd, (redundancy counter values)
        0x00, 0x00, 0x00, 0x00, 0x2c, // <unique data counter>
        0x00, 0x00, 0x2c,             // <redundant packet counter>
        0x73, 0x64, 0x2c,             // sd, (scan data)
        0x00, 0x00, 0x00, 0x00, 0x2c, // <count of seen advertisements>
      //0x70, 0x64, 0x2c,             // pd, (padding)
      //0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // <PADDING>
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

    advdata[21] = (0 >> 8) & 0xFF;
    advdata[20] = (0     ) & 0xFF;

    NRF_LOG_PRINTF("ADV COUNT: %d\n", adv_count);
    advdata[29] = (adv_count >> 24) & 0xFF;
    advdata[28] = (adv_count >> 16) & 0xFF;
    advdata[27] = (adv_count >>  8) & 0xFF;
    advdata[26] = (adv_count      ) & 0xFF;
    adv_count = 0;

    // Pass encoded advertising data and/or scan response data to the stack.
    uint32_t err_code = sd_ble_gap_adv_data_set(advdata, 31, srdata, 0);
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
    NRF_LOG_PRINTF("ERROR 0x%X\n", err_code);
}

void ble_evt_adv_report (ble_evt_t* p_ble_evt) {
    led_toggle(LED_PIN);
    adv_count++;

    ble_gap_evt_adv_report_t* report = &p_ble_evt->evt.gap_evt.params.adv_report;
    uint8_t* addr = report->peer_addr.addr;
    NRF_LOG_PRINTF("Addr: %0X%0X%0X%0X%0X%0X - %s - %d bytes\n", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0], (report->scan_rsp)?"RSP":"ADV", report->dlen);
}

void advertising_start(void) {
    // do nothing
}

int main (void) {

    // Setup LED
    led_init(LED_PIN);
    led_off(LED_PIN);

    // RTT code
    log_rtt_init();

    // Setup BLE
    simple_ble_init(&ble_config);

    // Setup timer to change parameters
    timer_begin();

    // Start advertising with initial parameters
    advertise_with_next_parameter_set(true);

    // Begin scanning
    simple_ble_scan_start();

    NRF_LOG_PRINTF("App Configured!\n");

    while (1) {
        power_manage();
    }
}
