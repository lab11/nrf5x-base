/*
 * BLE setup functions
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
#include "nrf_gpio.h"
#include "pstorage.h"
#include "app_trace.h"
#include "ble_hrs_c.h"
#include "ble_bas_c.h"
#include "app_util.h"
#include "app_timer.h"

// Configurations
#include "simple_ble.h"
#include "led.h"

/*******************************************************************************
 *   STATIC AND GLOBAL VARIABLES
 ******************************************************************************/
static const simple_ble_config_t* ble_config;

simple_ble_app_t app;
ble_gap_adv_params_t m_adv_params;
ble_gap_sec_params_t m_sec_params = {
    SEC_PARAM_BOND,
    SEC_PARAM_MITM,
    SEC_PARAM_IO_CAPABILITIES,
    SEC_PARAM_OOB,
    SEC_PARAM_MIN_KEY_SIZE,
    SEC_PARAM_MAX_KEY_SIZE,
};


/*******************************************************************************
 *   FUNCTION PROTOTYPES
 ******************************************************************************/
static void service_error_handler(uint32_t nrf_error);
static void conn_params_error_handler(uint32_t nrf_error);
static void ble_evt_dispatch(ble_evt_t * p_ble_evt);
static void sys_evt_dispatch(uint32_t sys_evt);
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt);
static void on_ble_evt(ble_evt_t * p_ble_evt);

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);


/*******************************************************************************
 *   HANDLERS AND CALLBACKS
 ******************************************************************************/
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    // APPL_LOG("[APPL]: ASSERT: %s, %d, error 0x%08x\r\n", p_file_name, line_num, error_code);

    // This call can be used for debug purposes during development of an application.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    // ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover with a reset.
    //NVIC_SystemReset();

    // callback for user
    if (ble_error) {
        ble_error(error_code);
    }
    while(1);
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name) {
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}

static void service_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}

static void conn_params_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}

static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    on_ble_evt(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);
}

static void sys_evt_dispatch(uint32_t sys_evt) {
}

static void on_conn_params_evt(ble_conn_params_evt_t * p_evt) {
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED) {
        err_code = sd_ble_gap_disconnect(app.conn_handle,
                BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

static void on_ble_evt(ble_evt_t * p_ble_evt) {
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            app.conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            // callback for user. Weak reference, so check validity first
            if (ble_evt_connected) {
                ble_evt_connected(p_ble_evt);
            }
            // continue advertising, but nonconnectably
            m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
            advertising_start();
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            app.conn_handle = BLE_CONN_HANDLE_INVALID;
            // callback for user. Weak reference, so check validity first
            if (ble_evt_disconnected) {
                ble_evt_disconnected(p_ble_evt);
            }
            // go back to advertising connectably
            advertising_stop();
            m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_IND;
            advertising_start();
            break;

        case BLE_GATTS_EVT_WRITE:
            // callback for user. Weak reference, so check validity first
            if (ble_evt_write) {
                ble_evt_write(p_ble_evt);
            }
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            err_code = sd_ble_gap_sec_params_reply(app.conn_handle,
                    BLE_GAP_SEC_STATUS_SUCCESS, &m_sec_params, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            err_code = sd_ble_gatts_sys_attr_set(app.conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_AUTH_STATUS:
            break;

        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            // No keys found for this device.
            err_code = sd_ble_gap_sec_info_reply(app.conn_handle, NULL, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_TIMEOUT:
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING) {
                err_code = sd_power_system_off();
                APP_ERROR_CHECK(err_code);
            }
            break;

        default:
            break;
    }
}


/*******************************************************************************
 *   INIT FUNCTIONS
 ******************************************************************************/
void __attribute__((weak)) ble_stack_init (void) {
    uint32_t err_code;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION,
            false);

    // Enable BLE stack
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    //Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Set the MAC address of the device
    {
        ble_gap_addr_t gap_addr;

        // Get the current original address
        sd_ble_gap_address_get(&gap_addr);

        // New address is a combination of Michigan OUI and Platform ID
        uint8_t new_mac_addr[6] = {0x00, 0x00, ble_config->platform_id, 0xe5, 0x98, 0xc0};

        // Set the new BLE address with the Michigan OUI, Platform ID, and
        //  bottom two octets from the original gap address
        gap_addr.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;
        memcpy(gap_addr.addr+2, new_mac_addr+2, sizeof(gap_addr.addr)-2);
        err_code = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE,
                &gap_addr);
        APP_ERROR_CHECK(err_code);
    }
}

void __attribute__((weak)) gap_params_init (void) {
    uint32_t                err_code;
    ble_gap_conn_sec_mode_t sec_mode;
    ble_gap_conn_params_t   gap_conn_params;

    // Full strength signal
    sd_ble_gap_tx_power_set(4);

    // Let anyone connect and set the name given the platform
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    err_code = sd_ble_gap_device_name_set(&sec_mode,
            (const uint8_t *)ble_config->adv_name, strlen(ble_config->adv_name));
    APP_ERROR_CHECK(err_code);

    // Not sure what this is useful for, but why not set it
    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_COMPUTER);
    APP_ERROR_CHECK(err_code);

    // Specify parameters for a connection
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));
    gap_conn_params.min_conn_interval = ble_config->min_conn_interval;
    gap_conn_params.max_conn_interval = ble_config->max_conn_interval;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

void __attribute__((weak)) advertising_init(void) {
    memset(&m_adv_params, 0, sizeof(m_adv_params));
    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = ble_config->adv_interval;
    m_adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;
}

void __attribute__((weak)) conn_params_init(void) {
}

void __attribute__((weak)) services_init (void) {
}


/*******************************************************************************
 *   HELPER FUNCTIONS
 ******************************************************************************/
void __attribute__((weak)) advertising_start(void) {
    uint32_t err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
}

void __attribute__((weak)) advertising_stop(void) {
    uint32_t err_code = sd_ble_gap_adv_stop();
    APP_ERROR_CHECK(err_code);
}

void __attribute__((weak)) power_manage(void) {
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

simple_ble_app_t* simple_ble_init(const simple_ble_config_t* conf) {
    ble_config = conf;

    // Setup BLE and services
    ble_stack_init();
    gap_params_init();
    advertising_init();
    services_init();
    conn_params_init();

    // Return a reference to the application state so that the user of this
    // module has a pointer to the connection handle.
    return &app;
}
