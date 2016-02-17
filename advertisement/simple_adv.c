/*
 * Advertise just a device name.
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
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "ble_hci.h"
#include "app_trace.h"
#include "ble_hrs_c.h"
#include "ble_bas_c.h"
#include "app_util.h"

// Platform, Peripherals, Devices, Services
#include "simple_ble.h"
#include "simple_adv.h"

static void full_adv (bool name, // if true, name goes in original packet
                      ble_uuid_t* service_uuid,
                      ble_advdata_manuf_data_t* manuf_specific_data) {
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t srdata;

    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));
    memset(&srdata, 0, sizeof(srdata));

    // Common
    advdata.include_appearance      = false;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    // Put name in main packet or in the scan response
    if (name) {
        advdata.name_type           = BLE_ADVDATA_FULL_NAME;
    } else {
        srdata.name_type            = BLE_ADVDATA_FULL_NAME;
    }

    // Handle service UUIDs
    if (service_uuid != NULL) {
        advdata.uuids_complete.uuid_cnt = 1;
        advdata.uuids_complete.p_uuids  = service_uuid;
    }

    // Handle manufacturer data
    if (manuf_specific_data != NULL) {
        advdata.p_manuf_specific_data   = manuf_specific_data;
    }

    err_code = ble_advdata_set(&advdata, &srdata);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();
}

void simple_adv_only_name () {
    full_adv(true, NULL, NULL);
}

void simple_adv_service (ble_uuid_t* service_uuid) {
    full_adv(false, service_uuid, NULL);
}

void simple_adv_manuf_data (ble_advdata_manuf_data_t* manuf_specific_data) {
    full_adv(false, NULL, manuf_specific_data);
}

void simple_adv_service_manuf_data (ble_uuid_t* service_uuid,
                                    ble_advdata_manuf_data_t* manuf_specific_data) {
    full_adv(false, service_uuid, manuf_specific_data);
}
