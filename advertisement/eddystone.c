/*
 * Eddystone URL advertisement
 */

// Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Nordic Libraries
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "nrf.h"
#include "nrf_sdm.h"
#include "ble.h"
#include "ble_db_discovery.h"
#include "app_util.h"
#include "app_error.h"
#include "ble_advdata_parser.h"
#include "ble_conn_params.h"
#include "ble_hci.h"
#include "app_trace.h"
#include "ble_hrs_c.h"
#include "ble_bas_c.h"
#include "app_util.h"

// Platform, Peripherals, Devices, Services
#include "simple_ble.h"
#include "eddystone.h"

static ble_uuid_t PHYSWEB_SERVICE_UUID[] = {{PHYSWEB_SERVICE_ID, BLE_UUID_TYPE_BLE}};
static ble_advdata_uuid_list_t PHYSWEB_SERVICE_LIST = {1, PHYSWEB_SERVICE_UUID};

void eddystone_adv(char* url_str, const ble_advdata_t* scan_response_data) {
    uint32_t err_code;

    // Physical Web data
    uint8_t url_frame_length = 3 + strlen((char*)url_str); // Change to 4 if URLEND is applied
    uint8_t m_url_frame[url_frame_length];
    m_url_frame[0] = PHYSWEB_URL_TYPE;
    m_url_frame[1] = PHYSWEB_TX_POWER;
    m_url_frame[2] = PHYSWEB_URLSCHEME_HTTP;
    for (uint8_t i=0; i<strlen((char*)url_str); i++) {
        m_url_frame[i+3] = url_str[i];
    }
    //m_url_frame[url_frame_length-1] = PHYSWEB_URLEND_COMSLASH; // Remember to change url_frame_length
    
    // Physical web service
    ble_advdata_service_data_t service_data;
    service_data.service_uuid   = PHYSWEB_SERVICE_ID;
    service_data.data.p_data    = m_url_frame;
    service_data.data.size      = url_frame_length;

    // Build and set advertising data
    ble_advdata_t advdata;
    memset(&advdata, 0, sizeof(advdata));
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.p_service_data_array    = &service_data;
    advdata.service_data_count      = 1;
    advdata.uuids_complete          = PHYSWEB_SERVICE_LIST;

    // Actually set advertisement data
    err_code = ble_advdata_set(&advdata, scan_response_data);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();
}

