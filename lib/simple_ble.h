#ifndef __SIMPLE_BLE_H
#define __SIMPLE_BLE_H

/*******************************************************************************
 *   TYPE DEFINITIONS
 ******************************************************************************/
typedef struct ble_app_s {
    uint16_t    conn_handle;           // Handle of the current connection. This will be BLE_CONN_HANDLE_INVALID when not in a connection.
    uint16_t    service_handle;
    uint8_t     uuid_type;
} ble_app_t;

typedef struct simple_ble_config_s {
    uint8_t     platform_id;        // used as 4th octet in device BLE address
    char*       adv_name;           // used in advertisements if there is room
    uint16_t    adv_interval;
    uint16_t    min_conn_interval;
    uint16_t    max_conn_interval;
} simple_ble_config_t;


/*******************************************************************************
 *   FUNCTION PROTOTYPES
 ******************************************************************************/
// implement for callbacks
extern void __attribute__((weak)) ble_evt_connected(ble_evt_t* p_ble_evt);
extern void __attribute__((weak)) ble_evt_disconnected(ble_evt_t* p_ble_evt);
extern void __attribute__((weak)) ble_evt_write(ble_evt_t* p_ble_evt);
extern void __attribute__((weak)) ble_error(uint32_t error_code);

// overwrite to change functionality
void __attribute__((weak)) ble_stack_init (void);
void __attribute__((weak)) gap_params_init (void);
void __attribute__((weak)) advertising_init(void);
void __attribute__((weak)) conn_params_init(void);
void __attribute__((weak)) services_init (void);
void __attribute__((weak)) advertising_start(void);
void __attribute__((weak)) advertising_stop(void);
void __attribute__((weak)) power_manage(void);

// call to initialize
void simple_ble_init(const simple_ble_config_t* conf);

/*******************************************************************************
 *   DEFINES
 ******************************************************************************/
#include "app_timer.h"

//cannot change server database
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0

#define MANUFACTURER_NAME 				"Lab11UMich"
#define MODEL_NUMBER 					DEVICE_NAME
#define HARDWARE_REVISION 				"A"
#define FIRMWARE_REVISION 				"0.1"

//advertising timeout sec
#define APP_ADV_TIMEOUT_IN_SECONDS      0

//RTC1_Prescale
#define APP_TIMER_PRESCALER             0

#define APP_TIMER_MAX_TIMERS            6

//size of op queues
#define APP_TIMER_OP_QUEUE_SIZE         5

#define SLAVE_LATENCY                   0

//supervisory timeout 4s
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)

//time from initiating event(conn or notify start) to first time param_update c
//called
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)

#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)

//attempts before giving up parameter negotiation
#define MAX_CONN_PARAMS_UPDATE_COUNT    3

//timeout for pairing or sec requests secs
#define SEC_PARAM_TIMEOUT               30

//perform bonding
#define SEC_PARAM_BOND                  1

//no man in the middle
#define SEC_PARAM_MITM                  0

//no i/o capability
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE

//no out of bound data
#define SEC_PARAM_OOB                   0

#define SEC_PARAM_MIN_KEY_SIZE          7

#define SEC_PARAM_MAX_KEY_SIZE          16

//max scheduler event size
#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)

//max num in scheduler queue
#define SCHED_QUEUE_SIZE                10

#define MAX_PKT_LEN                     20


#endif

