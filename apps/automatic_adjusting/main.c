/*
 * Scan for portions of a second
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "nrf_log.h"
#include "led.h"

#include "simple_ble.h"
#include "simple_adv.h"

#define LED0 13

// function prototypes
static void timer_handler(void*);
static void advertise_with_next_parameter_set(bool);

// Timer data structure
APP_TIMER_DEF(parameter_timer);

// Count of number of advertisements seen
uint32_t adv_count = 0;
uint64_t prr_product = 1000000;

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x40,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = "RTTTEST",       // used in advertisements if there is room
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};


// Timer initialization
static void timer_begin (void) {
    uint32_t err_code;

    // Initialize timer module (settings copied from blink app)
    APP_TIMER_INIT(0, 4, false);

    // Create a timer
    err_code = app_timer_create(&parameter_timer, APP_TIMER_MODE_SINGLE_SHOT, timer_handler);
    APP_ERROR_CHECK(err_code);

    // Start the timer
    const int timer_interval_ms = 1*1000; // Max is 512 seconds
    err_code = app_timer_start(parameter_timer, APP_TIMER_TICKS(timer_interval_ms, 0), NULL);
    APP_ERROR_CHECK(err_code);
}

// Timer callback
static void timer_handler (void* _unused) {
    static bool scanning_enabled = false;
    static int scan_duration_ms = 1*1000;
    uint32_t err_code;

    if (scanning_enabled) {
        // stop scanning, print results, wait a bit
        sd_ble_gap_scan_stop();
        /*
        uint64_t prr = 1000000;
        for (int i=0; i<adv_count; i++) {
            prr = (prr*999248)/1000000;
        }
        uint64_t p_c = 1000000-prr;
        */

        // determine how many packets per second to send
        const uint32_t gateway_loss = 78800;
        uint64_t prr = ((prr_product)*(1000000-gateway_loss))/1000000; // account for expected loss at gateway
        uint8_t adv_reliable_count = 1;
        NRF_LOG_PRINTF("\tCount: %d Expected PRR: %u\n", adv_reliable_count, (uint32_t)prr);
        while (prr < 990000 && adv_reliable_count < 10) {
            adv_reliable_count++;
            prr = 1000000 - (((1000000-prr)*(1000000-prr))/1000000);
            NRF_LOG_PRINTF("\tCount: %d Expected PRR: %u\n", adv_reliable_count, (uint32_t)prr);
        }
        NRF_LOG_PRINTF("Adv count: %d. Prr: %u. I should send %d packets.\n", adv_count, (uint32_t)prr_product, adv_reliable_count);
        scanning_enabled = false;
        const int timer_interval_ms = 1*1000; // Max is 512 seconds
        err_code = app_timer_start(parameter_timer, APP_TIMER_TICKS(timer_interval_ms, 0), NULL);
        APP_ERROR_CHECK(err_code);
    } else {
        // begin scanning
        scanning_enabled = true;
        adv_count = 0;
        prr_product = 1000000;
        err_code = app_timer_start(parameter_timer, APP_TIMER_TICKS(scan_duration_ms, 0), NULL);
        APP_ERROR_CHECK(err_code);
        simple_ble_scan_start();
    }
}

void ble_error(uint32_t error_code) {
    led_on(LED0);
    log_rtt_printf(0, "ERROR 0x%X\n", error_code);
}

void ble_evt_adv_report (ble_evt_t* p_ble_evt) {
    led_toggle(LED0);

    ble_gap_evt_adv_report_t* report = &p_ble_evt->evt.gap_evt.params.adv_report;
    //uint8_t* addr = report->peer_addr.addr;
    uint32_t local_prr = 1000000 - ((31+16)*8 + (report->dlen + 16)*8);
    prr_product = (prr_product * (uint64_t)local_prr)/1000000;
    //NRF_LOG_PRINTF("Addr: %02X%02X%02X%02X%02X%02X - %s - %d bytes\n", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0], (report->scan_rsp)?"RSP":"ADV", report->dlen);

    adv_count++;
}

int main (void) {
    led_init(LED0);
    led_off(LED0);

    // Setup BLE
    simple_ble_init(&ble_config);

    // Advertise because why not
    //simple_adv_only_name();

    // And scan at the same time
    //simple_ble_scan_start();

    // RTT code
    log_rtt_init();
    NRF_LOG_PRINTF("Debug to RTT\n");

    // Start timers
    timer_begin();

    while (1) {
        power_manage();
    }
}
