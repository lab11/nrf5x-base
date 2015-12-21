/*
 * IoT Gateway Advertisement Protocol from HotMobile'15 paper.
 *
 * https://github.com/lab11/iot-gateway
 */

// Standard Libraries
#include <stdint.h>
// #include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Nordic Libraries
#include "nordic_common.h"
#include "nrf_error.h"
#include "ble_advdata.h"

// Platform, Peripherals, Devices, Services
#include "simple_ble.h"
#include "iot_gateway.h"

uint32_t iot_gateway_adv (char* post_url_str,
                          uint8_t incentive_program_level,
                          uint8_t reliability_level,
                          uint8_t sensors,
                          uint8_t* data,
                          uint8_t data_len,
                          const ble_advdata_t* scan_response_data) {
    uint32_t err_code;

    // Check inputs
    if (incentive_program_level > 0xf) return NRF_ERROR_INVALID_PARAM;
    if (reliability_level > 0xf) return NRF_ERROR_INVALID_PARAM;
    if (data_len > 10) return NRF_ERROR_INVALID_PARAM;

    // Buffer to craft the adv packet format
    uint8_t gateway_api[IOT_GATEWAY_PKT_MAX_LEN] = {0};

    // First 14 bytes are destination URL
    strncpy((char*) gateway_api, post_url_str, IOT_GATEWAY_PKT_URL_MAX_LEN);

    // Next byte is the incentive program and reliability
    gateway_api[14] = (incentive_program_level << 4) | reliability_level;

    // The which sensors the app wants
    gateway_api[15] = sensors;

    // Finally app-specific arbitrary data
    memcpy(gateway_api+16, data, data_len);

    ble_advdata_service_data_t  service_data;
    service_data.service_uuid = (((uint16_t) gateway_api[0])<<8) + gateway_api[1];
    service_data.data.p_data  = gateway_api+2;
    service_data.data.size    = IOT_GATEWAY_PKT_HEADER_LEN-2 + data_len;

    // Build and set advertising data.
    ble_advdata_t advdata;
    memset(&advdata, 0, sizeof(advdata));
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.p_service_data_array    = &service_data;
    advdata.service_data_count      = 1;

    // Actually set advertisement data
    err_code = ble_advdata_set(&advdata, scan_response_data);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();

    return NRF_SUCCESS;
}
