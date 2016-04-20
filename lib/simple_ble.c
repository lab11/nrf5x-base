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
#include "ble.h"
#include "ble_db_discovery.h"
#include "app_util.h"
#include "app_error.h"
#include "ble_conn_params.h"
#include "ble_hci.h"
#include "app_trace.h"
#include "ble_hrs_c.h"
#include "ble_bas_c.h"
#include "app_util.h"
#include "app_timer.h"
#include "softdevice_handler.h"
#include "nrf_sdm.h"
#ifdef ENABLE_DFU
#include "ble_dfu.h"
#include "bootloader_types.h"
#include "bootloader_util.h"
#include "bootloader.h"
#endif

// Configurations
#include "simple_ble.h"

#ifdef ENABLE_DFU
// Defines
#define IRQ_ENABLED               0x01
#define MAX_NUMBER_INTERRUPTS     32
#define BOOTLOADER_BLE_ADDR_START 0x20007F80
#endif

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

// configuration settings that can be redefined by application
__attribute__((weak)) const int SLAVE_LATENCY = 0;
__attribute__((weak)) const int CONN_SUP_TIMEOUT = MSEC_TO_UNITS(4000, UNIT_10_MS);
__attribute__((weak)) const int FIRST_CONN_PARAMS_UPDATE_DELAY = APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER);

#ifdef ENABLE_DFU
static simple_ble_service_t dfu_service = {
    .uuid128 =  {{0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
                  0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}},
    .uuid_handle.uuid = BLE_DFU_SERVICE_UUID};

static simple_ble_char_t    dfu_ctrlpt_char = {.uuid16 = BLE_DFU_CTRL_PT_UUID};

static bool pending_dfu = 0;
#endif

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
#ifndef SOFTDEVICE_s130 // This function is included in the SDK 11
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
#endif

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

#ifdef ENABLE_DFU
static void interrupts_disable(void) {
    uint32_t interrupt_setting_mask;
    uint32_t irq = 0; // We start from first interrupt, i.e. interrupt 0.

    // Fetch the current interrupt settings.
    interrupt_setting_mask = NVIC->ISER[0];

    for (; irq < MAX_NUMBER_INTERRUPTS; irq++)
    {
        if (interrupt_setting_mask & (IRQ_ENABLED << irq))
        {
            // The interrupt was enabled, and hence disable it.
            NVIC_DisableIRQ((IRQn_Type)irq);
        }
    }
}
#endif

static void on_ble_evt(ble_evt_t * p_ble_evt) {
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            app.conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            // continue advertising, but nonconnectably
            m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_SCAN_IND;
            advertising_start();
            // connected to device. Set initial CCCD attributes to NULL
            err_code = sd_ble_gatts_sys_attr_set(app.conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);

            // callback for user. Weak reference, so check validity first
            if (ble_evt_connected) {
                ble_evt_connected(p_ble_evt);
            }
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            app.conn_handle = BLE_CONN_HANDLE_INVALID;
            advertising_stop();
#ifdef ENABLE_DFU
            // if pending dfu, clear and disable irq and then reset to bootloader
            if (pending_dfu) {
                pending_dfu = 0;
                dfu_reset_prepare();
                err_code = sd_softdevice_disable();
                APP_ERROR_CHECK(err_code);

                err_code = sd_softdevice_vector_table_base_set(NRF_UICR->BOOTLOADERADDR);
                APP_ERROR_CHECK(err_code);

                NVIC_ClearPendingIRQ(SWI2_IRQn);
                interrupts_disable();
                bootloader_util_app_start(NRF_UICR->BOOTLOADERADDR);
            }
#endif
            // go back to advertising connectably
            m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_IND;
            advertising_start();

            // callback for user. Weak reference, so check validity first
            if (ble_evt_disconnected) {
                ble_evt_disconnected(p_ble_evt);
            }
            break;

        case BLE_GATTS_EVT_WRITE:
#ifdef ENABLE_DFU
            // if written to dfu ctrl pt
            if (simple_ble_is_char_event(p_ble_evt, &dfu_ctrlpt_char)) {
                pending_dfu = 1;
                // set flag for bootloader to enter dfu  
                err_code = sd_power_gpregret_clr(0xFF);
                APP_ERROR_CHECK(err_code);
                err_code = sd_power_gpregret_set(BOOTLOADER_DFU_START);
                APP_ERROR_CHECK(err_code);
                // disconnect, wait for event. 
                err_code = sd_ble_gap_disconnect(app.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION); 
                APP_ERROR_CHECK(err_code);
                break;
            }
#endif            
            // callback for user. Weak reference, so check validity first
            if (ble_evt_write) {
                ble_evt_write(p_ble_evt);
            }
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            // callback for user. Weak reference, so check validity first
            if (ble_evt_rw_auth) {
                ble_evt_rw_auth(p_ble_evt);
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

        case BLE_GATTS_EVT_TIMEOUT:
            if (p_ble_evt->evt.gatts_evt.params.timeout.src == BLE_GATT_TIMEOUT_SRC_PROTOCOL) {
                err_code = sd_ble_gap_disconnect(app.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BLE_GAP_EVT_ADV_REPORT:
            if (ble_evt_adv_report) {
                ble_evt_adv_report(p_ble_evt);
            }
            break;

        default:
            break;
    }

    // allow users to handle events themselves if they want
    //  weak reference, check validity before calling
    if (ble_evt_user_handler) {
        ble_evt_user_handler(p_ble_evt);
    }
}


/*******************************************************************************
 *   INIT FUNCTIONS
 ******************************************************************************/

// Configure the MAC address of the device based on the config values and
// what is stored in the flash.
void __attribute__((weak)) ble_address_set () {
    uint32_t err_code;

    // Set the MAC address of the device
    // Highest priority is address from flash if available
    // Next is is the address supplied by user in ble_config
    // Finally, Nordic assigned random value is used as last choice
    ble_gap_addr_t gap_addr;

    // get BLE address from Flash
    uint8_t* _ble_address = (uint8_t*)BLEADDR_FLASH_LOCATION;
    if (_ble_address[1] == 0xFF && _ble_address[0] == 0xFF) {
        // No user-defined address stored in flash

        // New address is a combination of Michigan OUI and Platform ID
        uint8_t new_mac_addr[6] = {0x00, 0x00, ble_config->platform_id, 0xe5, 0x98, 0xc0};

        // Check if we should use the Nordic assigned random value for the
        // lower two bytes or use a user configured value.
        if (ble_config->device_id != DEVICE_ID_DEFAULT) {
            new_mac_addr[0] = ble_config->device_id & 0xFF;
            new_mac_addr[1] = (ble_config->device_id >> 8) & 0xFF;
            memcpy(gap_addr.addr, new_mac_addr, sizeof(gap_addr.addr));

        } else {
            // Set the new BLE address with the Michigan OUI, Platform ID, and
            //  bottom two octets from the original gap address
            // Get the current original address
            sd_ble_gap_address_get(&gap_addr);
            memcpy(gap_addr.addr+2, new_mac_addr+2, sizeof(gap_addr.addr)-2);
        }
    } else {
        // User-defined address stored in flash

        // Set the new BLE address with the user-defined address
        memcpy(gap_addr.addr, _ble_address, 6);
    }
 
#ifdef ENABLE_DFU
    // write ble address to memory to share with bootloader
    memcpy((uint8_t*)BOOTLOADER_BLE_ADDR_START, gap_addr.addr, 6);
#endif
    gap_addr.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;
    err_code = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &gap_addr);
    APP_ERROR_CHECK(err_code);
}

// Init the crystal and softdevice, plus configure the device address
void __attribute__((weak)) ble_stack_init (void) {
    uint32_t err_code;

#ifdef SOFTDEVICE_s130
    // Softdevice 130 2.0.0 changes how the softdevice init procedure works.
    nrf_clock_lf_cfg_t clock_lf_cfg = {
        .source        = NRF_CLOCK_LF_SRC_RC,
        .rc_ctiv       = 16, // bradjc: I mostly made these up based on docs. May be not great.
        .rc_temp_ctiv  = 2,
        .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM};

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    // Initialize the SoftDevice handler module.
    // SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_TEMP_4000MS_CALIBRATION, NULL);

    ble_enable_params_t ble_enable_params;
    // Need these #defines. C is the worst.
    #define CENTRAL_LINK_COUNT    1
    #define PERIPHERAL_LINK_COUNT 1
    err_code = softdevice_enable_get_default_config(1, // central link count
                                                    1, // peripheral link count
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

#else // softdevice s110 and possibly others

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION,
            false);

    // Enable BLE stack
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
#endif

    //Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // And set the MAC address in the init phase
    ble_address_set();
}

void __attribute__((weak)) gap_params_init (void) {
    uint32_t                err_code;
    ble_gap_conn_sec_mode_t sec_mode;

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
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;
    ble_gap_conn_params_t  conn_params;

    // Specify parameters for a connection
    memset(&conn_params, 0, sizeof(conn_params));
    conn_params.min_conn_interval = ble_config->min_conn_interval;
    conn_params.max_conn_interval = ble_config->max_conn_interval;
    conn_params.slave_latency     = SLAVE_LATENCY;
    conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&conn_params);
    APP_ERROR_CHECK(err_code);

    // setup connection parameter negotiation
    memset(&cp_init, 0, sizeof(cp_init));
    cp_init.p_conn_params = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail = false;
    cp_init.evt_handler = on_conn_params_evt;
    cp_init.error_handler = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

void __attribute__((weak)) services_init (void) {
}

#ifdef ENABLE_DFU
void __attribute__((weak)) dfu_init (void) {

    simple_ble_add_service(&dfu_service);
    // Add ctrl pt characteristic
    simple_ble_add_characteristic(0,1,1,1,
        BLE_L2CAP_MTU_DEF, NULL,
        &dfu_service,&dfu_ctrlpt_char); 
}

void __attribute__((weak)) dfu_reset_prepare (void) {
}
#endif

void __attribute__((weak)) initialize_app_timer (void) {
    // allow user to overwrite if they want to change timer parameters
#ifdef SDK_VERSION_9
    // old version of the API specifying maximum number of timers
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);
#else
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
#endif
}


/*******************************************************************************
 *   HELPER FUNCTIONS
 ******************************************************************************/
void __attribute__((weak)) advertising_start(void) {
    uint32_t err_code = sd_ble_gap_adv_start(&m_adv_params);
    if (err_code != NRF_ERROR_INVALID_STATE) {
        // ignore Invalid State responses. Occurs when start is called twice
        APP_ERROR_CHECK(err_code);
    }
}

void __attribute__((weak)) advertising_stop(void) {
    uint32_t err_code = sd_ble_gap_adv_stop();
    if (err_code != NRF_ERROR_INVALID_STATE) {
        // ignore Invalid State responses. Occurs when stop is called although
        //  advertising is not running
        APP_ERROR_CHECK(err_code);
    }
}

void __attribute__((weak)) power_manage(void) {
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

/*******************************************************************************
 *   SIMPLE BLE API
 ******************************************************************************/

simple_ble_app_t* simple_ble_init(const simple_ble_config_t* conf) {
    ble_config = conf;

    // Setup BLE and services
    ble_stack_init();
    gap_params_init();
    advertising_init();
    services_init();
#ifdef ENABLE_DFU
    dfu_init();
#endif
    // APP_TIMER_INIT must be called before conn_params_init since it uses timers
    initialize_app_timer();
    conn_params_init();

    // initialize our connection state to "not in a connection"
    app.conn_handle = BLE_CONN_HANDLE_INVALID;

    // Return a reference to the application state so that the user of this
    // module has a pointer to the connection handle.
    return &app;
}

void simple_ble_add_service (simple_ble_service_t* service_handle) {
    uint32_t err_code;

    // Setup our long UUID so that nRF recognizes it. This is done by storing
    //  the full 128-bit UUID and using 16 bits of it as a handle
    uint16_t uuid16 = (service_handle->uuid128.uuid128[12] << 8) |(service_handle->uuid128.uuid128[13]);
    if (!service_handle->uuid_handle.uuid) {
      service_handle->uuid_handle.uuid = uuid16;
    }
    err_code = sd_ble_uuid_vs_add(&(service_handle->uuid128), &(service_handle->uuid_handle.type));
    APP_ERROR_CHECK(err_code);

    // Add the custom service to the system
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
            &(service_handle->uuid_handle), &(service_handle->service_handle));
    APP_ERROR_CHECK(err_code);
}

void simple_ble_add_characteristic (uint8_t read,
                                    uint8_t write,
                                    uint8_t notify,
                                    uint8_t vlen,
                                    uint16_t len,
                                    uint8_t* buf,
                                    simple_ble_service_t* service_handle,
                                    simple_ble_char_t* char_handle) {
    volatile uint32_t err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    // set characteristic metadata
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read   = read;
    char_md.char_props.write  = write;
    char_md.char_props.notify = notify;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    // set characteristic uuid
    char_uuid.type = service_handle->uuid_handle.type;
    char_uuid.uuid = char_handle->uuid16;

    // set attribute metadata
    memset(&attr_md, 0, sizeof(attr_md));
    if (read) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    if (write) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc    = BLE_GATTS_VLOC_USER;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = vlen;

    // set attribute data
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = len;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = len; // max len can be up to BLE_GATTS_FIX_ATTR_LEN_MAX (510)
    attr_char_value.p_value   = buf;

    err_code = sd_ble_gatts_characteristic_add((service_handle->service_handle),
            &char_md, &attr_char_value, &(char_handle->char_handle));
    APP_ERROR_CHECK(err_code);
}

uint32_t simple_ble_update_char_len (simple_ble_char_t* char_handle, uint16_t len) {
    volatile uint32_t err_code;

    ble_gatts_value_t value_config;
    value_config.len = len;
    value_config.offset = 0;
    value_config.p_value = NULL;

    // Update length for vlen variable stored in user-space (VLOC_USER)
    err_code = sd_ble_gatts_value_set(BLE_CONN_HANDLE_INVALID, char_handle->char_handle.value_handle, &value_config);
    // since this isn't a configuration-time call, actually return the error
    //  code to the user for handling rather than checking it ourselves and
    //  possibly crashing the app
    return err_code;
}

uint32_t simple_ble_notify_char (simple_ble_char_t* char_handle) {
    volatile uint32_t err_code;

    // can't notify if we aren't in a connection
    if (app.conn_handle == BLE_CONN_HANDLE_INVALID) {
        // not an error though
        return NRF_SUCCESS;
    }

    ble_gatts_hvx_params_t hvx_params;
    hvx_params.handle = char_handle->char_handle.value_handle;
    hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = 0;
    hvx_params.p_len = NULL; // notify full length. No response wanted
    hvx_params.p_data = NULL; // use existing value

    err_code = sd_ble_gatts_hvx(app.conn_handle, &hvx_params);
    if (err_code == NRF_ERROR_INVALID_STATE) {
        // error means notify is not enabled by the client. IGNORE
        return NRF_SUCCESS;
    }

    // since this isn't a configuration-time call, actually return the error
    //  code to the user for handling rather than checking it ourselves and
    //  possibly crashing the app
    return err_code;
}

bool simple_ble_is_char_event (ble_evt_t* p_ble_evt, simple_ble_char_t* char_handle) {
    ble_gatts_evt_write_t* p_evt_write = &(p_ble_evt->evt.gatts_evt.params.write);

    return (bool)(p_evt_write->handle == char_handle->char_handle.value_handle);
}

void simple_ble_add_auth_characteristic (uint8_t read,
                                    uint8_t write,
                                    uint8_t notify,
                                    uint8_t vlen,
                                    bool read_auth,
                                    bool write_auth,
                                    uint16_t len,
                                    uint8_t* buf,
                                    simple_ble_service_t* service_handle,
                                    simple_ble_char_t* char_handle) {
    volatile uint32_t err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    // set characteristic metadata
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read   = read;
    char_md.char_props.write  = write;
    char_md.char_props.notify = notify;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    // set characteristic uuid
    char_uuid.type = service_handle->uuid_handle.type;
    char_uuid.uuid = char_handle->uuid16;

    // set attribute metadata
    memset(&attr_md, 0, sizeof(attr_md));
    if (read) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    if (write) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc    = BLE_GATTS_VLOC_USER;
    attr_md.rd_auth = read_auth;
    attr_md.wr_auth = write_auth;
    attr_md.vlen    = vlen;

    // set attribute data
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = len;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = len; // max len can be up to BLE_GATTS_FIX_ATTR_LEN_MAX (510)
    attr_char_value.p_value   = buf;

    err_code = sd_ble_gatts_characteristic_add((service_handle->service_handle),
            &char_md, &attr_char_value, &(char_handle->char_handle));
    APP_ERROR_CHECK(err_code);
}

bool simple_ble_is_read_auth_event (ble_evt_t* p_ble_evt, simple_ble_char_t* char_handle) {
    ble_gatts_evt_rw_authorize_request_t* p_auth_req =
            &(p_ble_evt->evt.gatts_evt.params.authorize_request);

    if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_READ) {
        // read authorization
        uint16_t read_handle = p_auth_req->request.read.handle;
        if (read_handle == char_handle->char_handle.value_handle) {
            // read request for the matching characteristic
            return true;
        }
    }

    return false;
}

bool simple_ble_is_write_auth_event (ble_evt_t* p_ble_evt, simple_ble_char_t* char_handle) {
    ble_gatts_evt_rw_authorize_request_t* p_auth_req =
            &(p_ble_evt->evt.gatts_evt.params.authorize_request);

    if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE) {
        // write authorization
        uint16_t write_handle = p_auth_req->request.write.handle;
        if (write_handle == char_handle->char_handle.value_handle) {
            // write request for the matching characteristic
            return true;
        }
    }

    return false;
}
uint32_t simple_ble_grant_auth (ble_evt_t* p_ble_evt) {
    ble_gatts_evt_rw_authorize_request_t* p_auth_req =
            &(p_ble_evt->evt.gatts_evt.params.authorize_request);

    // initialize response
    ble_gatts_rw_authorize_reply_params_t auth_resp;
    memset(&auth_resp, 0, sizeof(auth_resp));
    auth_resp.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;

    // add proper response type
    if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_READ) {
        // read request
        auth_resp.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
    } else if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE) {
        // write request
        auth_resp.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
    } else {
        // type is invalid, why are we here?
        auth_resp.type = BLE_GATTS_AUTHORIZE_TYPE_INVALID;
    }

    // since this isn't configuration, return any possible errors to the user
    //  rather than app error checking
    return sd_ble_gatts_rw_authorize_reply(app.conn_handle, &auth_resp);
}

void simple_ble_add_stack_characteristic (uint8_t read,
                                    uint8_t write,
                                    uint8_t notify,
                                    uint8_t vlen,
                                    uint16_t len,
                                    uint8_t* buf,
                                    simple_ble_service_t* service_handle,
                                    simple_ble_char_t* char_handle) {
    volatile uint32_t err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    // set characteristic metadata
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read   = read;
    char_md.char_props.write  = write;
    char_md.char_props.notify = notify;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    // set characteristic uuid
    char_uuid.type = service_handle->uuid_handle.type;
    char_uuid.uuid = char_handle->uuid16;

    // set attribute metadata
    memset(&attr_md, 0, sizeof(attr_md));
    if (read) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    if (write) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = vlen;

    // set attribute data
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = len;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = len; // max len can be up to BLE_GATTS_FIX_ATTR_LEN_MAX (510)
    attr_char_value.p_value   = buf;

    err_code = sd_ble_gatts_characteristic_add((service_handle->service_handle),
            &char_md, &attr_char_value, &(char_handle->char_handle));
    APP_ERROR_CHECK(err_code);
}

// assuming that the buffer sent in there will be long enough
uint32_t simple_ble_stack_char_get (simple_ble_char_t* char_handle, uint16_t* len, uint8_t* buf) {
    uint32_t err_code;
    ble_gatts_value_t value = {0};

    err_code = sd_ble_gatts_value_get(app.conn_handle, char_handle->char_handle.value_handle, &value);
    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    // no error
    *len = value.len;
    for (int i=0; i<value.len; i++) {
        buf[i] = value.p_value[i];
    }
    return NRF_SUCCESS;
}

uint32_t simple_ble_stack_char_set (simple_ble_char_t* char_handle, uint16_t len, uint8_t* buf) {
    ble_gatts_value_t value = {
        .len = len,
        .offset = 0,
        .p_value = buf,
    };
    return sd_ble_gatts_value_set(app.conn_handle, char_handle->char_handle.value_handle, &value);
}

#ifdef SOFTDEVICE_s130
static const ble_gap_scan_params_t m_scan_param = {
    .active = 0,                   // Active scanning not set.
    .selective = 0,                // Selective scanning not set.
    .p_whitelist = NULL,           // No whitelist provided.
    .interval = 0x00A0,
    .window = 0x0050,
    .timeout = 0x0000              // No timeout.
};

void simple_ble_scan_start () {
    ret_code_t err_code;

    err_code = sd_ble_gap_scan_stop();

    err_code = sd_ble_gap_scan_start(&m_scan_param);
    // It is okay to ignore this error since we are stopping the scan anyway.
    if (err_code != NRF_ERROR_INVALID_STATE) {
        APP_ERROR_CHECK(err_code);
    }
}
#endif
