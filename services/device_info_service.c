/*
 * Device Information Serivce
 * https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.device_information.xml
 */

// Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Nordic Libraries
#include "nordic_common.h"
#include "nrf.h"
#include "ble.h"
#include "app_error.h"
#include "ble_dis.h"

// Simple BLE Libraries
#include "simple_ble.h"
#include "device_info_service.h"

void simple_ble_device_info_service(char* hw_rev, char* fw_rev, char* sw_rev) {
    uint32_t error_code;
    ble_dis_init_t device_info = {0};

	// It's not clear that we need to support:
	//	* Manufacturer Name
	//	* Model Number
	//	* Serial Number
	//
    // char* manuf_name, char* model_num, char* serial_num,
    /*
    if (manuf_name != NULL) {
        device_info.manufact_name_str.length = strlen(manuf_name);
        device_info.manufact_name_str.p_str = (uint8_t*)manuf_name;
    }

    if (model_num != NULL) {
        device_info.model_num_str.length = strlen(model_num);
        device_info.model_num_str.p_str = (uint8_t*)model_num;
    }

    if (serial_num != NULL) {
        device_info.serial_num_str.length = strlen(serial_num);
        device_info.serial_num_str.p_str = (uint8_t*)serial_num;
    }
    */

    // code and hardware revisions
    if (hw_rev != NULL) {
        device_info.hw_rev_str.length = strlen(hw_rev);
        device_info.hw_rev_str.p_str = (uint8_t*)hw_rev;
    }
    if (fw_rev != NULL) {
        device_info.fw_rev_str.length = strlen(fw_rev);
        device_info.fw_rev_str.p_str = (uint8_t*)fw_rev;
    }
    if (sw_rev != NULL) {
        device_info.sw_rev_str.length = strlen(sw_rev);
        device_info.sw_rev_str.p_str = (uint8_t*)sw_rev;
    }

    // BLE address of the system
    ble_dis_sys_id_t sys_id = {0};
    sys_id.manufacturer_id = (0xFFFE << 24) | (0x30004f);
    sys_id.organizationally_unique_id = 0xc098e5;
    device_info.p_sys_id = &sys_id;

    // read permission
    device_info.dis_attr_md.read_perm.sm = 1;
    device_info.dis_attr_md.read_perm.lv = 1;

	// Create service
    error_code = ble_dis_init(&device_info);
    APP_ERROR_CHECK(error_code);
}

