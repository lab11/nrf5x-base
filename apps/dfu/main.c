/*
 * Send an advertisement periodically
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "led.h"
#include "bootloader.h"
#include "nrf_drv_config.h"
//#include "hci_transport_config.h"

#include "simple_ble.h"
#include "simple_adv.h"

// Custom UTF-8 name. Flash this app and scan to see it!
char name[20] = {0xE2, 0x9D, 0x84, 0xE2, 0x9D,
                 0x85, 0xE2, 0x9D, 0x86, 0xE2,
                 0x9C, 0x8B, 0xE2, 0x87, 0x92,
                 0xF0, 0x9F, 0x8D, 0xBB, 0x00};


// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x00,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = name,
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};


int main(void) {
    uint32_t err_code;

    // Setup BLE
    simple_ble_init(&ble_config);

    if (!bootloader_app_is_valid(DFU_BANK_0_REGION_START))
    {
        // Initiate an update of the firmware.
        err_code = bootloader_dfu_start();
        APP_ERROR_CHECK(err_code);
    }

    if (bootloader_app_is_valid(DFU_BANK_0_REGION_START))
    {
        // Select a bank region to use as application region.
        // @note: Only applications running from DFU_BANK_0_REGION_START is supported.
        bootloader_app_start(DFU_BANK_0_REGION_START);
    }

    NVIC_SystemReset();
}
