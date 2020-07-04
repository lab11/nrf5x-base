/*
 * Adjust to a scan of the environment
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "led.h"
#include "nrf_log.h"

#include "simple_ble.h"
#include "simple_adv.h"

#define LED_PIN 13

// function prototypes
static void setup_advertising(uint16_t);
void ble_after_adv_event(void);
static void ble_new_epoch(void);
static void setup_timers(void);
static void epoch_timer_handler(void*);
static void scan_timer_handler(void*);

// Timer for epochs
APP_TIMER_DEF(epoch_timer);
APP_TIMER_DEF(scan_timer);

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

static uint8_t minutes_in_epoch = 10;
static uint16_t packets_per_data = 1;
static bool start_new_epoch = false;

static uint32_t adv_count = 0;
static uint64_t prr_product = 1000000;
static bool scanning_enabled = false;

static void setup_advertising (uint16_t packets_per_second) {

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
    // only update advertisements if we are in advertising mode
    if (!scanning_enabled) {

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
        uint32_t err_code = sd_ble_gap_adv_data_set(advdata, 31, srdata, 0);
        APP_ERROR_CHECK(err_code);

        // check if we should roll to next cycle
        NRF_LOG_PRINTF("Epoch %d Data %d Packet %d\n", epoch, data_in_epoch, packet_in_data);
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
}

static void ble_new_epoch (void) {
    // stop advertising
    sd_ble_gap_adv_stop();

    // reset variables
    adv_count = 0;
    prr_product = 1000000;
    scanning_enabled = true;
    NRF_LOG_PRINTF("Starting scan\n");

    // start a timer
    const int timer_interval_ms = 1000; // Max is 512 seconds
    uint32_t err_code = app_timer_start(scan_timer, APP_TIMER_TICKS(timer_interval_ms, 0), NULL);
    APP_ERROR_CHECK(err_code);

    // start scanning for transmissions
    simple_ble_scan_start();
}

void ble_error (uint32_t error_code) {
    led_on(LED_PIN);
    NRF_LOG_PRINTF("ERROR 0x%X\n", error_code);
}

void ble_evt_adv_report (ble_evt_t* p_ble_evt) {
    // get the report
    ble_gap_evt_adv_report_t* report = &p_ble_evt->evt.gap_evt.params.adv_report;

    // update PRR model
    uint32_t local_prr = 1000000 - ((31+16)*8 + (report->dlen + 16)*8);
    prr_product = (prr_product * (uint64_t)local_prr)/1000000;

    // track number of advertisements seen
    adv_count++;
}

static void setup_timers (void) {
    uint32_t err_code;

    // Initialize timer module (settings copied from blink app)
    APP_TIMER_INIT(0, 4, false);

    // Create epoch timer
    err_code = app_timer_create(&epoch_timer, APP_TIMER_MODE_REPEATED, epoch_timer_handler);
    APP_ERROR_CHECK(err_code);

    // Create scan timer
    err_code = app_timer_create(&scan_timer, APP_TIMER_MODE_SINGLE_SHOT, scan_timer_handler);
    APP_ERROR_CHECK(err_code);

    // Start the epoch timer
    const int timer_interval_ms = 1*60*1000; // Max is 512 seconds
    err_code = app_timer_start(epoch_timer, APP_TIMER_TICKS(timer_interval_ms, 0), NULL);
    APP_ERROR_CHECK(err_code);
}

static void epoch_timer_handler (void* _unused) {
    static uint8_t minutes = 0;

    // track time that has passed
    minutes++;
    NRF_LOG_PRINTF("Minute\n");
    if (minutes >= minutes_in_epoch) {
        minutes = 0;
        NRF_LOG_PRINTF("New epoch\n");
        start_new_epoch = true;
    }
}

static void scan_timer_handler (void* _unused) {
    // stop scanning
    sd_ble_gap_scan_stop();
    scanning_enabled = false;

    // update tx_estimate
    tx_estimate = adv_count;

    // update packets_per_data as necessary
    const uint32_t gateway_loss = 78800;
    uint64_t prr = ((prr_product)*(1000000-gateway_loss))/1000000; // account for expected loss at gateway
    uint8_t adv_reliable_count = 1;
    NRF_LOG_PRINTF("Adv count: %d. Prr: %u.\n", adv_count, (uint32_t)prr_product, adv_reliable_count);
    NRF_LOG_PRINTF("\tCount: %d Expected PRR: %u\n", adv_reliable_count, (uint32_t)prr);
    while (prr < 990000 && adv_reliable_count < 10) {
        adv_reliable_count++;
        prr = 1000000 - (((1000000-prr)*(1000000-prr))/1000000);
        NRF_LOG_PRINTF("\tCount: %d Expected PRR: %u\n", adv_reliable_count, (uint32_t)prr);
    }
    NRF_LOG_PRINTF("\tI should send %d packets.\n", adv_reliable_count);
    packets_per_data = adv_reliable_count;

    // restart advertising, call setup_advertising again
    setup_advertising(packets_per_data);
}

int main (void) {

    // Setup BLE
    simple_ble_init(&ble_config);

    led_init(LED_PIN);

    // RTT code
    log_rtt_init();
    NRF_LOG_PRINTF("Debug to RTT\n");

    // Initialize timers and start epoch timer
    setup_timers();

    // Configure advertising parameters and start advertising
    setup_advertising(packets_per_data);

    while (1) {
        power_manage();
    }
}
