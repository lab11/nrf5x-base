#ifndef __SIMPLE_BLE_H
#define __SIMPLE_BLE_H

#include <stdint.h>
#include <stdbool.h>

#include "app_timer.h"
#include "ble_types.h"
#include "ble_advdata.h"

/*******************************************************************************
 *   TYPE DEFINITIONS
 ******************************************************************************/
typedef struct simple_ble_app_s {
    uint16_t    conn_handle; // Handle of the current connection. This will be BLE_CONN_HANDLE_INVALID when not in a connection.
} simple_ble_app_t;

typedef struct simple_ble_config_s {
    uint8_t     platform_id;        // used as 4th octet in device BLE address
    uint16_t    device_id;          // set the lower 16 bits of the device id. Set to DEVICE_ID_DEFAULT to use random.
    const char* adv_name;           // used in advertisements if there is room
    uint16_t    adv_interval;       // Between 20 ms and 10.24 s
    uint16_t    min_conn_interval;
    uint16_t    max_conn_interval;
} simple_ble_config_t;

typedef struct simple_ble_service_s {
    const ble_uuid128_t uuid128;
    ble_uuid_t uuid_handle;
    uint16_t service_handle;
    ble_uuid_t* adv_uuids;  // Array of UUIDs included in advertisements
    uint8_t     nr_adv_uuids;
    ble_uuid_t* sr_uuids;   // Array of UUIDs included in scan responses
    uint8_t     nr_sr_uuids;
} simple_ble_service_t;

typedef struct simple_ble_char_s {
    uint16_t uuid16;
    ble_gatts_char_handles_t char_handle;
} simple_ble_char_t;

/*******************************************************************************
 *   FUNCTION PROTOTYPES
 ******************************************************************************/
// Implement for callbacks
extern void ble_evt_connected(ble_evt_t const* p_ble_evt);
extern void ble_evt_disconnected(ble_evt_t const* p_ble_evt);
extern void ble_evt_write(ble_evt_t const* p_ble_evt);
extern void ble_evt_rw_auth(ble_evt_t const* p_ble_evt);
extern void ble_evt_user_handler(ble_evt_t const* p_ble_evt);
extern void ble_evt_adv_report(ble_evt_t const* p_ble_evt);
extern void ble_error(uint32_t error_code);

// Overwrite to change functionality
void ble_stack_init(void);
void ble_address_set (void);
void gap_params_init(void);
void advertising_init(void);
void conn_params_init(void);
void services_init(void);
#ifdef ENABLE_DFU
void dfu_init (void);
void dfu_reset_prepare (void);
#endif
void initialize_app_timer(void);
void advertising_start(void);
void advertising_stop(void);
void scanning_start(void);
void power_manage(void);

// Call to initialize
simple_ble_app_t* simple_ble_init(const simple_ble_config_t* conf);

// Standard service and characteristic creation
void simple_ble_add_service(simple_ble_service_t* service_char);

void simple_ble_add_characteristic(uint8_t read, uint8_t write, uint8_t notify, uint8_t vlen,
                                    uint16_t len, uint8_t* buf,
                                    simple_ble_service_t* service_handle,
                                    simple_ble_char_t* char_handle);

uint32_t simple_ble_update_char_len(simple_ble_char_t* char_handle, uint16_t len);
uint32_t simple_ble_notify_char(simple_ble_char_t* char_handle);
bool simple_ble_is_char_event(ble_evt_t const* p_ble_evt, simple_ble_char_t* char_handle);

// Enable read/write authorization on a characteristic
void simple_ble_add_auth_characteristic(uint8_t read, uint8_t write, uint8_t notify, uint8_t vlen,
                                         bool read_auth, bool write_auth,
                                         uint16_t len, uint8_t* buf,
                                         simple_ble_service_t* service_handle,
                                         simple_ble_char_t* char_handle);

bool simple_ble_is_read_auth_event(ble_evt_t* p_ble_evt, simple_ble_char_t* char_handle);
bool simple_ble_is_write_auth_event(ble_evt_t* p_ble_evt, simple_ble_char_t* char_handle);
uint32_t simple_ble_grant_auth(ble_evt_t* p_ble_evt);

// Characteristics with values on the SoftDevice stack
// These are harder to interact with, but necessary if you want to do BLE serialization
void simple_ble_add_stack_characteristic(uint8_t read, uint8_t write, uint8_t notify, uint8_t vlen,
                                          uint16_t len, uint8_t* buf,
                                          simple_ble_service_t* service_handle,
                                          simple_ble_char_t* char_handle);
uint32_t simple_ble_stack_char_get(simple_ble_char_t* char_handle, uint16_t* len, uint8_t* buf);
uint32_t simple_ble_stack_char_set(simple_ble_char_t* char_handle, uint16_t len, uint8_t* buf);


/*******************************************************************************
 *   GLOBAL CONFIGURATIONS
 *******************************************************************************/
extern const int SLAVE_LATENCY;                       // 0                                           //!< Slave latency.
extern const int CONN_SUP_TIMEOUT;                    // MSEC_TO_UNITS(4000, UNIT_10_MS)             //!< Connection supervision time-out (4 seconds). The supervision time-out uses 10 ms units.
extern const int FIRST_CONN_PARAMS_UPDATE_DELAY;      // APP_TIMER_TICKS(5000)                       //!< Time from initiating an event (connection or start of notification) to the first time @ref sd_ble_gap_conn_param_update is called (5 seconds).


/*******************************************************************************
 *   DEFINES
 ******************************************************************************/

#define APP_COMPANY_IDENTIFIER          0x02E0
#define APP_SERVICE_ID                  0x0022
#define MANUFACTURER_NAME 				"Lab11"
#define HARDWARE_REVISION 				"A"
#define FIRMWARE_REVISION 				"0.1"

#define DEVICE_ID_DEFAULT               0xFFFF

// Cannot change server database
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0

// RTC1_Prescale
#define APP_TIMER_PRESCALER             0

#define APP_TIMER_MAX_TIMERS            6

// Size of op queues
#define APP_TIMER_OP_QUEUE_SIZE         8


// Max scheduler event size
#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)

// Max num in scheduler queue
#define SCHED_QUEUE_SIZE                10

#define MAX_PKT_LEN                     20


// BLE CONFIGS
#define CENTRAL_LINK_COUNT                  0                                           //!< Number of central links used by the application. When changing this number, remember to adjust the RAM settings.
#define PERIPHERAL_LINK_COUNT               1                                           //!< Number of peripheral links used by the application. When changing this number, remember to adjust the RAM settings.

#define APP_ADV_INTERVAL                    MSEC_TO_UNITS(300, UNIT_0_625_MS)

#define MIN_CONN_INTERVAL                   MSEC_TO_UNITS(50, UNIT_1_25_MS)             //!< Minimum acceptable connection interval (50 ms). The connection interval uses 1.25 ms units.
#define MAX_CONN_INTERVAL                   MSEC_TO_UNITS(100, UNIT_1_25_MS)            //!< Maximum acceptable connection interval (100 ms). The connection interval uses 1.25 ms units.
#define NEXT_CONN_PARAMS_UPDATE_DELAY       APP_TIMER_TICKS(30000)                      //!< Time between each call to @ref sd_ble_gap_conn_param_update after the first call (30 seconds).
#define MAX_CONN_PARAMS_UPDATE_COUNT        3                                           //!< Number of attempts before giving up the connection parameter negotiation.

// Priority of the application BLE event handler.
#define APP_BLE_OBSERVER_PRIO               3
#define APP_BLE_CONN_CFG_TAG                1                                           /**< A tag identifying the SoftDevice BLE configuration. */


/*******************************************************************************
 *   Advertising
 ******************************************************************************/

// Functions
void simple_ble_set_adv(ble_advdata_t* adv_data, ble_advdata_t* scan_rsp_data);
void simple_ble_adv_only_name(void);

/*******************************************************************************
 *   EDDYSTONE
 ******************************************************************************/

// Functions
void simple_ble_es_adv(const char*, const ble_advdata_t*);
void simple_ble_es_with_manuf_adv(const char* url_str, ble_advdata_manuf_data_t* manuf_specific_data);
void simple_ble_es_with_name(const char* url_str);

// Physical Web
#define PHYSWEB_SERVICE_ID  0xFEAA
#define PHYSWEB_URL_TYPE    0x10    // Denotes URLs (vs URIs or TLM data)
#define PHYSWEB_TX_POWER    0xBA    // Tx Power. Measured at 1 m plus 41 dBm. (who cares)

#define PHYSWEB_URLSCHEME_HTTPWWW   0x00    // http://www.
#define PHYSWEB_URLSCHEME_HTTPSWWW  0x01    // https://www.
#define PHYSWEB_URLSCHEME_HTTP      0x02    // http://
#define PHYSWEB_URLSCHEME_HTTPS     0x03    // https://

#define PHYSWEB_URLEND_COMSLASH 0x00    // .com/
#define PHYSWEB_URLEND_ORGSLASH 0x01    // .org/
#define PHYSWEB_URLEND_EDUSLASH 0x02    // .edu/
#define PHYSWEB_URLEND_COM      0x07    // .com
#define PHYSWEB_URLEND_ORG      0x08    // .org
#define PHYSWEB_URLEND_EDU      0x09    // .edu

#endif /*__SIMPLE_BLE_H */
