/*
 * BLE setup functions
 */

// Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Nordic Libraries
#include "app_error.h"
#include "app_timer.h"
#include "app_util.h"
#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "ble_gatts.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "fds.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_ble_es.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "peer_manager.h"
#include "sdk_common.h"

// device firmware update code
#ifdef ENABLE_DFU
#include "ble_dfu.h"
#include "bootloader_types.h"
#include "bootloader_util.h"
#include "bootloader.h"

// Defines
#define IRQ_ENABLED               0x01
#define MAX_NUMBER_INTERRUPTS     32
#define BOOTLOADER_BLE_ADDR_START 0x20007F80
#define DFU_ADV_DATA_TYPE         0x16
#define DFU_ADV_DATA_VERS         0x01
#endif

// Simple BLE files
#include "simple_ble.h"

#if NRF_MODULE_ENABLED(SIMPLE_BLE)

/*******************************************************************************
 *   STATIC AND GLOBAL VARIABLES
 ******************************************************************************/
static const simple_ble_config_t* ble_config;

simple_ble_app_t app;

static ble_uuid_t PHYSWEB_SERVICE_UUID[] = {{PHYSWEB_SERVICE_ID, BLE_UUID_TYPE_BLE}};
static const ble_advdata_uuid_list_t PHYSWEB_SERVICE_LIST = {1, PHYSWEB_SERVICE_UUID};

// Buffer for scan data
static uint8_t scan_buffer[BLE_GAP_SCAN_BUFFER_MAX];
static ble_data_t m_scan_buffer = {
    .p_data = scan_buffer,
    .len = BLE_GAP_SCAN_BUFFER_MAX,
};

// Module instances
// ATTENTION: Those also instanciate event handlers for BLE and SYS events
NRF_BLE_GATT_DEF(m_gatt);                                                       /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                         /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);                                             /**< Advertising module instance. */

// Configuration settings that can be redefined by application
__attribute__((weak)) const int SLAVE_LATENCY = 0;
__attribute__((weak)) const int CONN_SUP_TIMEOUT = MSEC_TO_UNITS(4000, UNIT_10_MS);
__attribute__((weak)) const int FIRST_CONN_PARAMS_UPDATE_DELAY = APP_TIMER_TICKS(5000);

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
static void qwr_error_handler(uint32_t nrf_error);
static void conn_params_error_handler(uint32_t nrf_error);
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt);
static void on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);
static void on_adv_evt(ble_adv_evt_t ble_adv_evt);
#ifdef ENABLE_DFU
static void dfu_reset();
#endif

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);


/*******************************************************************************
 *   HANDLERS AND CALLBACKS
 ******************************************************************************/

// Default weak references for callbacks
//
// All of these callbacks are optional. By declaring prototypes with weak
// references here, these will turn into pointers with value 0 (definition of
// behavior for weak references with forward declaration but no implementation)
// rather than link time failures.
//
// Run-time code must check that these functions are valid before calling.
void __attribute__((weak)) ble_evt_connected(ble_evt_t const * p_ble_evt);
void __attribute__((weak)) ble_evt_disconnected(ble_evt_t const* p_ble_evt);
void __attribute__((weak)) ble_evt_write(ble_evt_t const* p_ble_evt);
void __attribute__((weak)) ble_evt_rw_auth(ble_evt_t const* p_ble_evt);
void __attribute__((weak)) ble_evt_user_handler(ble_evt_t const* p_ble_evt);
void __attribute__((weak)) ble_evt_adv_report(ble_evt_t const* p_ble_evt);
void __attribute__((weak)) ble_error(uint32_t error_code);


void __attribute__((weak)) app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
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

    // TODO: Include "better_error_handling.c" for all platforms
    printf("ERROR: APP ERROR on line %li, file %s!\n", line_num, p_file_name);

    // callback for user
    if (ble_error) {
        ble_error(error_code);
    }

    // Allow user to read current state with GDB
    while(1);
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name) {

    printf("ERROR: Assert in SoftDevice failed!\n");

    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}

static void qwr_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}

static void conn_params_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}

// Can also simply use "cp_init.disconnect_on_fail = true;
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt) {
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(app.conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
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

static void on_adv_evt(ble_adv_evt_t ble_adv_evt) {
    // do nothing
}

static void on_pm_evt(pm_evt_t const* p_evt) {
    ret_code_t err_code;

    switch (p_evt->evt_id)
    {
        case PM_EVT_BONDED_PEER_CONNECTED:
        {
            NRF_LOG_INFO("Connected to a previously bonded device.");
        } break;

        case PM_EVT_CONN_SEC_SUCCEEDED:
        {
            NRF_LOG_INFO("Connection secured: role: %d, conn_handle: 0x%x, procedure: %d.",
                         ble_conn_state_role(p_evt->conn_handle),
                         p_evt->conn_handle,
                         p_evt->params.conn_sec_succeeded.procedure);
        } break;

        case PM_EVT_CONN_SEC_FAILED:
        {
            /* Often, when securing fails, it shouldn't be restarted, for security reasons.
             * Other times, it can be restarted directly.
             * Sometimes it can be restarted, but only after changing some Security Parameters.
             * Sometimes, it cannot be restarted until the link is disconnected and reconnected.
             * Sometimes it is impossible, to secure the link, or the peer device does not support it.
             * How to handle this error is highly application dependent. */
        } break;

        case PM_EVT_CONN_SEC_CONFIG_REQ:
        {
            // Reject pairing request from an already bonded peer.
            pm_conn_sec_config_t conn_sec_config = {.allow_repairing = false};
            pm_conn_sec_config_reply(p_evt->conn_handle, &conn_sec_config);
        } break;

        case PM_EVT_STORAGE_FULL:
        {
            // Run garbage collection on the flash.
            err_code = fds_gc();
            if (err_code == FDS_ERR_NO_SPACE_IN_QUEUES)
            {
                // Retry.
            }
            else
            {
                APP_ERROR_CHECK(err_code);
            }
        } break;

        case PM_EVT_PEERS_DELETE_SUCCEEDED:
        {
            advertising_start();
        } break;

        case PM_EVT_PEER_DATA_UPDATE_FAILED:
        {
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peer_data_update_failed.error);
        } break;

        case PM_EVT_PEER_DELETE_FAILED:
        {
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peer_delete_failed.error);
        } break;

        case PM_EVT_PEERS_DELETE_FAILED:
        {
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peers_delete_failed_evt.error);
        } break;

        case PM_EVT_ERROR_UNEXPECTED:
        {
            // Assert.
            APP_ERROR_CHECK(p_evt->params.error_unexpected.error);
        } break;

        case PM_EVT_CONN_SEC_START:
        case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
        case PM_EVT_PEER_DELETE_SUCCEEDED:
        case PM_EVT_LOCAL_DB_CACHE_APPLIED:
        case PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED:
            // This can happen when the local DB has changed.
        case PM_EVT_SERVICE_CHANGED_IND_SENT:
        case PM_EVT_SERVICE_CHANGED_IND_CONFIRMED:
        default:
            break;
    }
}

static void on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context) {
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED: {

            app.conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, app.conn_handle);
            APP_ERROR_CHECK(err_code);

            // Continue advertising, but nonconnectably
            //m_advertising.adv_params.properties.type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED;
            //advertising_start();

            // Connected to device. Set initial CCCD attributes to NULL
            err_code = sd_ble_gatts_sys_attr_set(app.conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);

            printf("I'm connected!\n");

            // Callback for user. Weak reference, so check validity first
            if (ble_evt_connected) {
                ble_evt_connected(p_ble_evt);
            }
            break;
        }
        case BLE_GAP_EVT_DISCONNECTED: {

            // Invalidate handle
            app.conn_handle = BLE_CONN_HANDLE_INVALID;

            //advertising_stop();

#ifdef ENABLE_DFU
            // If pending dfu, clear and disable irq and then reset to bootloader
                if (pending_dfu) {
                    pending_dfu = 0;
                    dfu_reset();
                }
#endif
            // Go back to advertising connectably
            //m_advertising.adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
            //advertising_start();

            printf("I'm disconnected\n");

            // Callback for user. Weak reference, so check validity first
            if (ble_evt_disconnected) {
                ble_evt_disconnected(p_ble_evt);
            }
            break;
        }
        case BLE_GATTS_EVT_WRITE: {
#ifdef ENABLE_DFU
            // if written to dfu ctrl pt
            if (simple_ble_is_char_event(p_ble_evt, &dfu_ctrlpt_char)) {
                pending_dfu = 1;
                // disconnect, wait for event.
                err_code = sd_ble_gap_disconnect(app.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                APP_ERROR_CHECK(err_code);
                break;
            }
#endif
            // Callback for user. Weak reference, so check validity first
            if (ble_evt_write) {
                ble_evt_write(p_ble_evt);
            }
            break;
        }
        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST: {
            // Callback for user. Weak reference, so check validity first
            if (ble_evt_rw_auth) {
                ble_evt_rw_auth(p_ble_evt);
            }
            break;
        }
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST: {
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(app.conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GATTS_EVT_SYS_ATTR_MISSING: {
            // No system attributes have been stored
            err_code = sd_ble_gatts_sys_attr_set(app.conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GAP_EVT_AUTH_STATUS: {

            break;
        }
        case BLE_GAP_EVT_SEC_INFO_REQUEST: {
            // No keys found for this device.
            err_code = sd_ble_gap_sec_info_reply(app.conn_handle, NULL, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GAP_EVT_TIMEOUT: {

            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_SCAN) {
                err_code = sd_power_system_off();
                APP_ERROR_CHECK(err_code);
            }
            break;
        }
        case BLE_GATTC_EVT_TIMEOUT: {
            // Disconnect on GATT Client timeout event
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GATTS_EVT_TIMEOUT: {
            // Disconnect on GATT Server timeout event
            if (p_ble_evt->evt.gatts_evt.params.timeout.src == BLE_GATT_TIMEOUT_SRC_PROTOCOL) {
                err_code = sd_ble_gap_disconnect(app.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                APP_ERROR_CHECK(err_code);
            }
            break;
        }
        case BLE_GAP_EVT_ADV_REPORT: {
#ifdef ENABLE_DFU
            #ifdef SOFTDEVICE_s130
              // check if DFU advertisement
              uint8_t data[31];
              int len = parse_adata(p_ble_evt, 0xFF, data);

              if (len > 4 && len <= 31 &&
                  *((short *) data) == 0x02E0 &&
                  data[2] == DFU_ADV_DATA_TYPE &&
                  data[3] == DFU_ADV_DATA_VERS)
              {
                ble_gap_addr_t gap_addr;
                sd_ble_gap_addr_get(&gap_addr);

                if (memcmp(data+4, gap_addr.addr, 6) == 0) {
                  dfu_reset();
                }
              }
#endif
#endif

            if (ble_evt_adv_report) {
                ble_evt_adv_report(p_ble_evt);
            }

            // continue scanning
            err_code = sd_ble_gap_scan_start(NULL, &m_scan_buffer);
            APP_ERROR_CHECK(err_code);
            break;
        }
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST: {
            ble_gap_phys_t const phys =
                    {
                            .rx_phys = BLE_GAP_PHY_AUTO,
                            .tx_phys = BLE_GAP_PHY_AUTO,
                    };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
            break;
        }
        default:
            break;
    }

    // Allow users to handle events themselves if they want weak reference, check validity before calling
    if (ble_evt_user_handler) {
        ble_evt_user_handler(p_ble_evt);
    }
}



/*******************************************************************************
 *   INIT FUNCTIONS
 ******************************************************************************/

// Configure the MAC address of the device based on the config values and what is stored in the flash.
void __attribute__((weak)) ble_address_set(void) {
    uint32_t err_code;

    // Set the MAC address of the device
    // Highest priority is address from flash if available
    // Next is is the address supplied by user in ble_config
    // Finally, Nordic assigned random value is used as last choice
    ble_gap_addr_t gap_addr = {.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC};

    // Default is a combination of Michigan OUI and Platform ID
    uint8_t new_ble_addr[6] = {0x00, 0x00, ble_config->platform_id, 0xe5, 0x98, 0xc0};
    memcpy(gap_addr.addr, new_ble_addr, 6);

    if (ble_config->device_id != DEVICE_ID_DEFAULT) {
        new_ble_addr[0] = ble_config->device_id & 0xFF;
        new_ble_addr[1] = (ble_config->device_id >> 8) & 0xFF;
        memcpy(gap_addr.addr, new_ble_addr, sizeof(gap_addr.addr));
    } else {

    // get BLE address from Flash
#ifdef BLE_FLASH_ADDRESS
        uint8_t* _ble_address = (uint8_t*)BLE_FLASH_ADDRESS;
        if (_ble_address[1] == 0xFF && _ble_address[0] == 0xFF) {
            // No user-defined address stored in flash

            // Check if we should use the Nordic assigned random value for the
            // lower two bytes or use a user configured value.
            if (ble_config->device_id != DEVICE_ID_DEFAULT) {
                new_ble_addr[0] = ble_config->device_id & 0xFF;
                new_ble_addr[1] = (ble_config->device_id >> 8) & 0xFF;
                memcpy(gap_addr.addr, new_ble_addr, sizeof(gap_addr.addr));

            } else {
                // Set the new BLE address with the Michigan OUI, Platform ID, and
                //  bottom two octets from the original gap address
                // Get the current original address
                sd_ble_gap_addr_get(&gap_addr);
                memcpy(gap_addr.addr+2, new_ble_addr+2, sizeof(gap_addr.addr)-2);
            }
        } else {
            // User-defined address stored in flash

            // Set the new BLE address with the user-defined address
            memcpy(gap_addr.addr, _ble_address, 6);
        }
#endif

// If defined either over command line, in makefile or directly in ble_config.h
#ifdef BLE_ADDRESS
#define XID_STR(ID) ID_STR(ID)
#define ID_STR(ID) #ID
#define BLE_ADDRESS_LENGTH 6

        const char ble_address_string[] = XID_STR(BLE_ADDRESS);
        for (int i=0; i < BLE_ADDRESS_LENGTH; i++) {
            // For each 8bits, read the string and convert it from hex to a long; store it in LSB-first order
            ble_address[(BLE_ADDRESS_LENGTH-1) - i] = (uint8_t)strtoul(&ble_address_string[3*i], NULL, 16);
        }

        printf("Bluetooth address: %02x:%02x:%02x:%02x:%02x:%02x\n", ble_address[5], ble_address[4], ble_address[3], ble_address[2], ble_address[1], ble_address[0]);
        memcpy(gap_addr.addr, ble_address, BLE_ADDRESS_LENGTH);
#endif

#ifdef ENABLE_DFU
        // Write ble address to memory to share with bootloader
        memcpy((uint8_t*)BOOTLOADER_BLE_ADDR_START, gap_addr.addr, 6);
#endif
    }

    err_code = sd_ble_gap_addr_set(&gap_addr);
    APP_ERROR_CHECK(err_code);
}

// Init the crystal and SoftDevice, plus configure the device address
void __attribute__((weak)) ble_stack_init(void) {
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);
/*
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_app_ram_start_get(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Overwrite some of the default configurations for the BLE stack.
    ble_cfg_t ble_cfg;

    // Configure the maximum number of connections.
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.conn_cfg.conn_cfg_tag                     = APP_BLE_CONN_CFG_TAG;
    ble_cfg.conn_cfg.params.gap_conn_cfg.conn_count   = NRF_SDH_BLE_TOTAL_LINK_COUNT;
    ble_cfg.conn_cfg.params.gap_conn_cfg.event_length = NRF_SDH_BLE_GAP_EVENT_LENGTH;
    err_code = sd_ble_cfg_set(BLE_CONN_CFG_GAP, &ble_cfg, ram_start);
    APP_ERROR_CHECK(err_code);

    // Configure the connection roles
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.gap_cfg.role_count_cfg.periph_role_count  = NRF_SDH_BLE_PERIPHERAL_LINK_COUNT;
    ble_cfg.gap_cfg.role_count_cfg.central_role_count = NRF_SDH_BLE_CENTRAL_LINK_COUNT;
    ble_cfg.gap_cfg.role_count_cfg.central_sec_count  = BLE_GAP_ROLE_COUNT_CENTRAL_SEC_DEFAULT;
    err_code = sd_ble_cfg_set(BLE_GAP_CFG_ROLE_COUNT, &ble_cfg, ram_start);
    APP_ERROR_CHECK(err_code);

    // Configure max ATT MTU size
    memset(&ble_cfg, 0x00, sizeof(ble_cfg));
    ble_cfg.conn_cfg.conn_cfg_tag                 = APP_BLE_CONN_CFG_TAG;
    ble_cfg.conn_cfg.params.gatt_conn_cfg.att_mtu = NRF_SDH_BLE_GATT_MAX_MTU_SIZE;
    err_code = sd_ble_cfg_set(BLE_CONN_CFG_GATT, &ble_cfg, ram_start);
    APP_ERROR_CHECK(err_code);

    // Configure number of custom UUIDS.
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.common_cfg.vs_uuid_cfg.vs_uuid_count = NRF_SDH_BLE_VS_UUID_COUNT;
    err_code = sd_ble_cfg_set(BLE_COMMON_CFG_VS_UUID, &ble_cfg, ram_start);
    APP_ERROR_CHECK(err_code);

    // Configure the GATTS attribute table.
    memset(&ble_cfg, 0x00, sizeof(ble_cfg));
    ble_cfg.gatts_cfg.attr_tab_size.attr_tab_size = NRF_SDH_BLE_GATTS_ATTR_TAB_SIZE;
    err_code = sd_ble_cfg_set(BLE_GATTS_CFG_ATTR_TAB_SIZE, &ble_cfg, ram_start);
    APP_ERROR_CHECK(err_code);

    // Configure Service Changed characteristic.
    memset(&ble_cfg, 0x00, sizeof(ble_cfg));
    ble_cfg.gatts_cfg.service_changed.service_changed = NRF_SDH_BLE_SERVICE_CHANGED;
    err_code = sd_ble_cfg_set(BLE_GATTS_CFG_SERVICE_CHANGED, &ble_cfg, ram_start);
    APP_ERROR_CHECK(err_code);
*/

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, on_ble_evt, NULL);

    // And set the address in the init phase
    ble_address_set();
}

// GAP initialization: set up all the necessary GAP (Generic Access Profile) parameters of the device. It also sets the permissions and appearance
void __attribute__((weak)) gap_params_init(void) {
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    // Let anyone connect and set the name given the platform
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    err_code = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)ble_config->adv_name, strlen(ble_config->adv_name));
    APP_ERROR_CHECK(err_code);

    // Display default generic HID symbol when connected to other devices (e.g. Android, Windows)
    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_HID);
    APP_ERROR_CHECK(err_code);

    // Connection parameters
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = ble_config->min_conn_interval;
    gap_conn_params.max_conn_interval = ble_config->max_conn_interval;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

// Initializing the GATT module.
static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}

static ble_uuid_t m_adv_uuids[] =                                               /**< Universally unique service identifiers. */
{
    {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}
};
void __attribute__((weak)) advertising_init(void) {

    //XXX: Copied from template app
    ret_code_t             err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance      = true;
    init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    init.advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.advdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = 18000;

    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);

    /*
    ret_code_t             err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    // General Advertisement settings

    // Add device name in advertisements
    init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;

    // Set Advertisement setting
    init.advdata.include_appearance      = false;
    init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    //init.advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    //init.advdata.uuids_complete.p_uuids  = m_adv_uuids;

    // Configure advertisements
    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = ble_config->adv_interval;
    init.config.ble_adv_fast_timeout  = 0; // Never time out
    init.config.ble_adv_whitelist_enabled = false; // equivalent with BLE_GAP_ADV_FP_ANY

    // Define Event handler
    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);

    // Full strength signal, 0 dBm
    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, m_advertising.adv_handle, 0);
    APP_ERROR_CHECK(err_code);
    */
}

void __attribute__((weak)) conn_params_init(void) {
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    // Setup connection parameter negotiation
    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false; // Uses on_conn_params_evt as an event handler
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

void __attribute__((weak)) services_init(void) {
    ret_code_t         err_code;
    nrf_ble_qwr_init_t qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);
}

#define SEC_PARAM_BOND                  1                                       /**< Perform bonding. */
#define SEC_PARAM_MITM                  0                                       /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                  0                                       /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS              0                                       /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE                    /**< No I/O capabilities. */
#define SEC_PARAM_OOB                   0                                       /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE          7                                       /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE          16                                      /**< Maximum encryption key size. */
static void peer_manager_init(void)
{
    ble_gap_sec_params_t sec_param;
    ret_code_t           err_code;

    err_code = pm_init();
    APP_ERROR_CHECK(err_code);

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    sec_param.bond           = SEC_PARAM_BOND;
    sec_param.mitm           = SEC_PARAM_MITM;
    sec_param.lesc           = SEC_PARAM_LESC;
    sec_param.keypress       = SEC_PARAM_KEYPRESS;
    sec_param.io_caps        = SEC_PARAM_IO_CAPABILITIES;
    sec_param.oob            = SEC_PARAM_OOB;
    sec_param.min_key_size   = SEC_PARAM_MIN_KEY_SIZE;
    sec_param.max_key_size   = SEC_PARAM_MAX_KEY_SIZE;
    sec_param.kdist_own.enc  = 1;
    sec_param.kdist_own.id   = 1;
    sec_param.kdist_peer.enc = 1;
    sec_param.kdist_peer.id  = 1;

    err_code = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err_code);

    err_code = pm_register(on_pm_evt);
    APP_ERROR_CHECK(err_code);
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

void dfu_reset() {
    int err_code = 0;

    // set flag for bootloader to enter dfu
    err_code = sd_power_gpregret_clr(0xFF);
    APP_ERROR_CHECK(err_code);
    err_code = sd_power_gpregret_set(BOOTLOADER_DFU_START);
    APP_ERROR_CHECK(err_code);

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

void __attribute__((weak)) initialize_app_timer (void) {

    // Init application timer module
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


/*******************************************************************************
 *   HELPER FUNCTIONS
 ******************************************************************************/
void __attribute__((weak)) advertising_start(void) {

    uint32_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);

    if (err_code != NRF_ERROR_INVALID_STATE) {
        // Ignore Invalid State responses. Occurs when start is called twice
        APP_ERROR_CHECK(err_code);
    }
}

void __attribute__((weak)) advertising_stop(void) {

    uint32_t err_code = sd_ble_gap_adv_stop(m_advertising.adv_handle);

    if (err_code != NRF_ERROR_INVALID_STATE) {
        // Ignore Invalid State responses. Occurs when stop is called although advertising is not running
        APP_ERROR_CHECK(err_code);
    }
}

void __attribute__((weak)) scanning_start(void) {
    static ble_gap_scan_params_t m_scan_params = {
        .active            = false, // passive scanning (no scan response)
        .interval          = MSEC_TO_UNITS(100, UNIT_0_625_MS), // interval 100 ms
        .window            = MSEC_TO_UNITS(100, UNIT_0_625_MS), // window 100 ms
        .timeout           = BLE_GAP_SCAN_TIMEOUT_UNLIMITED,
        .scan_phys         = BLE_GAP_PHY_1MBPS,
        .filter_policy     = BLE_GAP_SCAN_FP_ACCEPT_ALL,
    };

    ret_code_t err_code = sd_ble_gap_scan_start(&m_scan_params, &m_scan_buffer);
    APP_ERROR_CHECK(err_code);
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

    // Initialize power management
    ret_code_t err_code = NRF_SUCCESS;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);

    // APP_TIMER_INIT must be called before BLE setup that relies on it
    initialize_app_timer();

    // Setup BLE and services
    ble_stack_init();
    gap_params_init();
    gatt_init();
    advertising_init();
    services_init();

    // Create device information service
#if defined(HW_REVISION) || defined(FW_REVISION)
    //simple_ble_device_info_service_automatic();
#endif

    // Enable device firmware updates
#ifdef ENABLE_DFU
    //dfu_init();
#endif

    conn_params_init();
    //peer_manager_init();

    // Initialize our connection state to "not in a connection"
    app.conn_handle = BLE_CONN_HANDLE_INVALID;

    // Return a reference to the application state so that the user of this
    // module has a pointer to the connection handle.
    return &app;
}

void simple_ble_add_service(simple_ble_service_t* service_handle) {
    uint32_t err_code;

    // Setup our long UUID so that nRF recognizes it. This is done by storing
    // the full 128-bit UUID and using 16 bits of it as a handle
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

void simple_ble_add_characteristic(uint8_t read, uint8_t write, uint8_t notify, uint8_t vlen,
                                   uint16_t len, uint8_t* buf,
                                   simple_ble_service_t* service_handle,
                                   simple_ble_char_t* char_handle) {
    ret_code_t err_code;

    // Set characteristic UUID & add it
    ble_uuid_t char_uuid;
    char_uuid.type = service_handle->uuid_handle.type;
    char_uuid.uuid = char_handle->uuid16;

    err_code = sd_ble_uuid_vs_add(&service_handle->uuid128, &char_uuid.type);
    APP_ERROR_CHECK(err_code);

    // Set characteristic metadata
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read   = read;
    char_md.char_props.write  = write;
    char_md.char_props.notify = notify;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    // Configuring Client Characteristic Configuration Descriptor (CCCD) metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc      = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md = &cccd_md;

    // Set attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    if (read) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    if (write) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc    = BLE_GATTS_VLOC_USER;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = vlen;

    // Set attribute data
    ble_gatts_attr_t attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = len;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = len; // max len can be up to BLE_GATTS_FIX_ATTR_LEN_MAX (510)
    attr_char_value.p_value   = buf;

    err_code = sd_ble_gatts_characteristic_add((service_handle->service_handle), &char_md, &attr_char_value, &(char_handle->char_handle));
    APP_ERROR_CHECK(err_code);
}

uint32_t simple_ble_update_char_len(simple_ble_char_t* char_handle, uint16_t len) {
    volatile uint32_t err_code;

    ble_gatts_value_t value_config;
    value_config.len = len;
    value_config.offset = 0;
    value_config.p_value = NULL;

    // Update length for vlen variable stored in user-space (VLOC_USER)
    err_code = sd_ble_gatts_value_set(BLE_CONN_HANDLE_INVALID, char_handle->char_handle.value_handle, &value_config);
    // Since this isn't a configuration-time call, actually return the error
    //  code to the user for handling rather than checking it ourselves and
    //  possibly crashing the app
    return err_code;
}

uint32_t simple_ble_notify_char(simple_ble_char_t* char_handle) {
    volatile uint32_t err_code;

    // Can't notify if we aren't in a connection
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
        // Error means notify is not enabled by the client. IGNORE
        return NRF_SUCCESS;
    }

    // Since this isn't a configuration-time call, actually return the error
    //  code to the user for handling rather than checking it ourselves and
    //  possibly crashing the app
    return err_code;
}

bool simple_ble_is_char_event(ble_evt_t const* p_ble_evt, simple_ble_char_t* char_handle) {
    ble_gatts_evt_write_t const* p_evt_write = &(p_ble_evt->evt.gatts_evt.params.write);

    return (bool)(p_evt_write->handle == char_handle->char_handle.value_handle);
}

void simple_ble_add_auth_characteristic(uint8_t read, uint8_t write, uint8_t notify, uint8_t vlen,
                                         bool read_auth, bool write_auth,
                                         uint16_t len, uint8_t* buf,
                                         simple_ble_service_t* service_handle,
                                         simple_ble_char_t* char_handle) {
    volatile uint32_t err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    // Set characteristic metadata
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read   = read;
    char_md.char_props.write  = write;
    char_md.char_props.notify = notify;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    // Set characteristic uuid
    char_uuid.type = service_handle->uuid_handle.type;
    char_uuid.uuid = char_handle->uuid16;

    // Set attribute metadata
    memset(&attr_md, 0, sizeof(attr_md));
    if (read) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    if (write) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc    = BLE_GATTS_VLOC_USER;
    attr_md.rd_auth = read_auth;
    attr_md.wr_auth = write_auth;
    attr_md.vlen    = vlen;

    // Set attribute data
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

bool simple_ble_is_read_auth_event(ble_evt_t* p_ble_evt, simple_ble_char_t* char_handle) {
    ble_gatts_evt_rw_authorize_request_t* p_auth_req =
            &(p_ble_evt->evt.gatts_evt.params.authorize_request);

    if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_READ) {
        // Read authorization
        uint16_t read_handle = p_auth_req->request.read.handle;
        if (read_handle == char_handle->char_handle.value_handle) {
            // Read request for the matching characteristic
            return true;
        }
    }

    return false;
}

bool simple_ble_is_write_auth_event(ble_evt_t* p_ble_evt, simple_ble_char_t* char_handle) {
    ble_gatts_evt_rw_authorize_request_t* p_auth_req =
            &(p_ble_evt->evt.gatts_evt.params.authorize_request);

    if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE) {
        // Write authorization
        uint16_t write_handle = p_auth_req->request.write.handle;
        if (write_handle == char_handle->char_handle.value_handle) {
            // Write request for the matching characteristic
            return true;
        }
    }

    return false;
}
uint32_t simple_ble_grant_auth(ble_evt_t* p_ble_evt) {
    ble_gatts_evt_rw_authorize_request_t* p_auth_req =
            &(p_ble_evt->evt.gatts_evt.params.authorize_request);

    // Initialize response
    ble_gatts_rw_authorize_reply_params_t auth_resp;
    memset(&auth_resp, 0, sizeof(auth_resp));
    auth_resp.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;

    // Add proper response type
    if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_READ) {
        // Read request
        auth_resp.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
    } else if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE) {
        // Write request
        auth_resp.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
    } else {
        // Type is invalid, why are we here?
        auth_resp.type = BLE_GATTS_AUTHORIZE_TYPE_INVALID;
    }

    // Since this isn't configuration, return any possible errors to the user
    //  rather than app error checking
    return sd_ble_gatts_rw_authorize_reply(app.conn_handle, &auth_resp);
}

void simple_ble_add_stack_characteristic(uint8_t read, uint8_t write, uint8_t notify, uint8_t vlen,
                                          uint16_t len, uint8_t* buf,
                                          simple_ble_service_t* service_handle,
                                          simple_ble_char_t* char_handle) {
    volatile uint32_t err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    // Set characteristic metadata
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read   = read;
    char_md.char_props.write  = write;
    char_md.char_props.notify = notify;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    // Set characteristic uuid
    char_uuid.type = service_handle->uuid_handle.type;
    char_uuid.uuid = char_handle->uuid16;

    // Set attribute metadata
    memset(&attr_md, 0, sizeof(attr_md));
    if (read) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    if (write) BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = vlen;

    // Set attribute data
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

// Assuming that the buffer sent in there will be long enough
uint32_t simple_ble_stack_char_get(simple_ble_char_t* char_handle, uint16_t* len, uint8_t* buf) {
    ble_gatts_value_t value = {
            .len = *len,
            .offset = 0,
            .p_value = buf,
    };

    return sd_ble_gatts_value_get(app.conn_handle, char_handle->char_handle.value_handle, &value);
}

uint32_t simple_ble_stack_char_set(simple_ble_char_t* char_handle, uint16_t len, uint8_t* buf) {
    ble_gatts_value_t value = {
            .len = len,
            .offset = 0,
            .p_value = buf,
    };
    return sd_ble_gatts_value_set(app.conn_handle, char_handle->char_handle.value_handle, &value);
}


/*******************************************************************************
 *   Advertising
 ******************************************************************************/

void simple_ble_adv_only_name(void) {
    ret_code_t err_code;

    // stop advertising. We must do this before changing the advertisement data
    advertising_stop();

    // create adv data
    ble_advdata_t adv_data = {0};
    adv_data.name_type = BLE_ADVDATA_FULL_NAME; // include full name
    adv_data.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE; // BLE Low energy advertisement

    // encode advertisement data
    m_advertising.adv_data.adv_data.p_data = m_advertising.enc_advdata;
    m_advertising.adv_data.adv_data.len = BLE_GAP_ADV_SET_DATA_SIZE_MAX;
    err_code = ble_advdata_encode(&adv_data, m_advertising.adv_data.adv_data.p_data, &m_advertising.adv_data.adv_data.len);
    APP_ERROR_CHECK(err_code);

    // actually set advertisement
    err_code = sd_ble_gap_adv_set_configure(&m_advertising.adv_handle, &m_advertising.adv_data, &m_advertising.adv_params);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();
}

void simple_ble_set_adv(ble_advdata_t* adv_data, ble_advdata_t* scan_rsp_data) {
    uint32_t err_code;

    // stop advertising. We must do this before changing the advertisement data
    advertising_stop();

    // encode advertisement data
    if (adv_data != NULL) {
        m_advertising.adv_data.adv_data.p_data = m_advertising.enc_advdata;
        m_advertising.adv_data.adv_data.len = BLE_GAP_ADV_SET_DATA_SIZE_MAX;
        err_code = ble_advdata_encode(adv_data, m_advertising.adv_data.adv_data.p_data, &m_advertising.adv_data.adv_data.len);
        APP_ERROR_CHECK(err_code);
    }

    // encode scan response data
    if (scan_rsp_data != NULL) {
        m_advertising.adv_data.scan_rsp_data.p_data = m_advertising.enc_scan_rsp_data;
        m_advertising.adv_data.scan_rsp_data.len = BLE_GAP_ADV_SET_DATA_SIZE_MAX;
        err_code = ble_advdata_encode(scan_rsp_data, m_advertising.adv_data.scan_rsp_data.p_data, &m_advertising.adv_data.scan_rsp_data.len);
        APP_ERROR_CHECK(err_code);
    } else {
        m_advertising.adv_data.scan_rsp_data.p_data = NULL;
        m_advertising.adv_data.scan_rsp_data.len = 0;
    }

    // actually set advertisement
    err_code = sd_ble_gap_adv_set_configure(&m_advertising.adv_handle, &m_advertising.adv_data, &m_advertising.adv_params);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();
}


/*******************************************************************************
 *   EDDYSTONE
 ******************************************************************************/

void simple_ble_es_adv(const char* url_str, const ble_advdata_t* scan_rsp_data) {
    uint32_t err_code;

    // Physical Web data
    const uint8_t header_len = 3;
    uint8_t url_frame_length = header_len + strlen((char*)url_str); // Change to 4 if URLEND is applied
    uint8_t m_url_frame[url_frame_length];
    m_url_frame[0] = PHYSWEB_URL_TYPE;
    m_url_frame[1] = PHYSWEB_TX_POWER;
    m_url_frame[2] = PHYSWEB_URLSCHEME_HTTPS;
    for (uint8_t i=0; i<strlen((char*)url_str); i++) {
        m_url_frame[i + header_len] = url_str[i];
    }
    //m_url_frame[url_frame_length-1] = PHYSWEB_URLEND_COMSLASH; // Remember to change url_frame_length

    //printf("Advertising Eddystone address %s with total length %i\n", url_str, url_frame_length);

    // Physical web service
    ble_advdata_service_data_t service_data;
    service_data.service_uuid = PHYSWEB_SERVICE_ID;
    service_data.data.p_data  = m_url_frame;
    service_data.data.size    = url_frame_length;

    // Build and set advertising data
    ble_advdata_t advdata;
    memset(&advdata, 0, sizeof(advdata));
    advdata.include_appearance   = false;
    advdata.flags                = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.p_service_data_array = &service_data;
    advdata.service_data_count   = 1;
    advdata.uuids_complete       = PHYSWEB_SERVICE_LIST;

    // Set advertisement data
    err_code = ble_advdata_encode(&advdata, m_advertising.adv_data.adv_data.p_data, &m_advertising.adv_data.adv_data.len);
    APP_ERROR_CHECK(err_code);

    // Set scan response data
    err_code = ble_advdata_encode(scan_rsp_data, m_advertising.adv_data.scan_rsp_data.p_data, &m_advertising.adv_data.scan_rsp_data.len);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_adv_set_configure(&m_advertising.adv_handle, &m_advertising.adv_data, &m_advertising.adv_params);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();
}

void simple_ble_es_with_manuf_adv(const char* url_str, ble_advdata_manuf_data_t* manuf_specific_data) {

    ble_advdata_t srdata;
    memset(&srdata, 0, sizeof(srdata));

    // Setup Scan response
    srdata.name_type = BLE_ADVDATA_FULL_NAME;
    srdata.p_manuf_specific_data = manuf_specific_data;

    simple_ble_es_adv(url_str, &srdata);
}

void simple_ble_es_with_name(const char* url_str) {

    ble_advdata_t srdata;
    memset(&srdata, 0, sizeof(srdata));

    // Setup Scan response
    srdata.name_type = BLE_ADVDATA_FULL_NAME;

    simple_ble_es_adv(url_str, &srdata);
}

#endif /*SIMPLE_BLE_ENABLED*/
