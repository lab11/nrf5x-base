/* ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 PURPOSE: ZBOSS API header
*/
#ifndef ZBOSS_API_H
#define ZBOSS_API_H 1

#include "zb_version.h"
#include "zboss_api_core.h"
#include "zboss_api_internal.h"
#include "zboss_api_nwk.h"
#include "zboss_api_af.h"
#include "zboss_api_zdo.h"
#include "zboss_api_aps.h"
#include "zb_ha.h"
#include "zb_address.h"
#include "zboss_api_zcl.h"
#include "zboss_api_tl.h"
#include "zboss_api_zgp.h"


/** @cond (!DOXYGEN_ERL_SECTION) */
/*! \addtogroup secur_api */
/*! @{ */

/**
   Setup nwk key to be used by ZCP tests.

   Call this routine at TC to change random nwk key generated from
   zb_ib_set_defaults to well-known key. To be used in certification tests.

   @param key - key to be used
   @param i - key number (0-3)

   @snippet tp_r20_bv-01_zc.c zb_secur_setup_preconfigured_key_1
   @snippet tp_r20_bv-01_zc.c zb_secur_setup_preconfigured_key_2

   See TP_R20_BV-01 sample.
*/
void zb_secur_setup_nwk_key(zb_uint8_t *key, zb_uint8_t i);


/**
   Add the install code for the device with specified long
   address.

   This valid is valid only for the TC (ZC).

   @param address - long address of the device to add the install
   code.
   @param ic - pointer to the install code buffer.

   @return RET_OK on success or RET_CONVERSION_ERROR on error in install code
   CRC

   @snippet cs_ick_tc_02_dut_using_ic.c wrong_ic

*/
zb_ret_t zb_secur_ic_add(zb_ieee_addr_t address, zb_uint8_t *ic);

/**
   Add the client install code.

   This function is to be run at client side (not at TC).

   @param  ic - pointer to the install code buffer.
   @return RET_OK on success or RET_CONVERSION_ERROR on error in install code
   CRC
*/
zb_ret_t zb_secur_ic_set(zb_uint8_t *ic);


/*! @} */
/** @endcond */ /*(!DOXYGEN_ERL_SECTION) */
/*! @addtogroup zboss_general_api  */
/*! @{ */

/** @cond internals_doc */
/**
   Check whether the right library is selected at application compilation time

   To be called from ZB_INIT() at start.
 */
#ifdef ZB_ED_ROLE
#define ZB_CHECK_LIBRARY()  since_you_got_that_symbol_unresolved_you_probably_use_ZB_ED_ROLE_preprocessor_define_while_linking_with_zc_library();
void since_you_got_that_symbol_unresolved_you_probably_use_ZB_ED_ROLE_preprocessor_define_while_linking_with_zc_library(void);
#else
#define ZB_CHECK_LIBRARY()  since_you_got_that_symbol_unresolved_you_probably_forget_use_ZB_ED_ROLE_preprocessor_define_while_linking_with_zed_library();
void since_you_got_that_symbol_unresolved_you_probably_forget_use_ZB_ED_ROLE_preprocessor_define_while_linking_with_zed_library(void);
#endif /* ZB_ED_ROLE */
/** @endcond */

/** @} */

/** @addtogroup zb_general_start
@{
*/
/**
   Global stack initialization.

   To be called from MAIN() at start.

   Usual initialization sequence: ZB_INIT(), then assign some IB values, then zboss_start().

   @param trace_comment - trace file name component (valid for Unix, ignored at MCU)

   @b Example:
@code
  ZB_INIT("zdo_zc");
@endcode

  See any sample
 */
#if defined ZB_INIT_HAS_ARGS || defined DOXYGEN
#define ZB_INIT(trace_comment)               \
{                                            \
  ZB_CHECK_LIBRARY();                        \
  zb_init((zb_char_t *)trace_comment);       \
}
/** @cond internals_doc */
/** @brief Global stack initialization.

    Don't call directly, use @ref ZB_INIT() instead
*/
void zb_init(zb_char_t *trace_comment);
/** @endcond */
#else
#define ZB_INIT(trace_comment)               \
{                                            \
  ZB_CHECK_LIBRARY();                        \
  zb_init();                                 \
}
void zb_init(void);
#endif  /* ZB_INIT_HAS_ARGS || defined DOXYGEN */

/** @} */

/** @addtogroup zb_general_set
    @{
*/
/**
   Set channel mask on device.
   @param channel_mask - ZigBee channel mask

   @b Example:
@code
  zb_set_channel((1l<<13) | (1l<<14));
@endcode
*/
zb_void_t zb_set_channel(zb_uint32_t channel_mask);

#ifdef ZB_OSIF_CONFIGURABLE_TX_POWER
/**
   Set transmit power.
   @param tx_power - Transmit power [dBm].

   @b Example:
@code
  zb_set_tx_power(0x32);
@endcode
*/

zb_ret_t zb_set_tx_power(zb_uint8_t tx_power);
#endif

#ifdef ZB_ED_ROLE
/**
   Set RxOnWhenIdle attribute
   @param rx_on - attribute value

   @b Example:
@code
  zb_set_rx_on_when_idle(ZB_FALSE); // switch device to rx off
@endcode
*/
zb_void_t zb_set_rx_on_when_idle(zb_bool_t rx_on);
#endif /* ZB_ED_ROLE */

/** @} */

/** @addtoproup zb_general_get
@{
*/

/**
   Returns current state of RX-ON-When-Idle of Zigbee ZED

   @return ZB_TRUE if ZED is not sleepy ZED, ZB_FALSE if ZED is a sleepy ZED
 */
zb_bool_t zb_get_rx_on_when_idle(void);

/** @} */

/** @addtogroup zb_general_start
@{
*/


/** @brief ZBOSS start function.
  *
  * Typical device start: init, load some parameters from nvram and proceed with startup.
  *
  * Startup means either Formation (for ZC), rejoin or discovery/association join.  After startup
  * complete @ref zboss_signal_handler callback is called, so application will know when to do
  * some useful things.
  *
  * Precondition: stack must be inited by @ref ZB_INIT() call. @ref ZB_INIT() loads IB from NVRAM
  * or set its defaults, so caller has a chance to change some parameters.
  * @note: ZB is not looped in this routine. Instead, it schedules callback and returns.  Caller
  * must run @ref zboss_main_loop() after this routine.
  *
  * @return RET_OK on success.
  */
zb_ret_t zboss_start(void);

/** @} */

/** @addtoproup zb_general_get
@{
*/
/**
 *  @brief Get ZBOSS version.
 *  @returns Pointer to zero-terminated version string.
 */
zb_char_t ZB_IAR_CODE *zb_get_version(void);

/** @} */

/** @addtogroup zb_general_start
@{
*/

/**
   Start ZBOSS with only minimal initialization, without starting commissioning.

   This function initializes scheduler and buffers pool, but not MAC and upper layers.
   Typically zboss_start_no_autostart() is used when application is doing some
   additional initialization (HW init etc) and requires ZBOSS scheduler to be
   working.
   Application should later call ZBOSS commissioning initiation - for
   instance, @ref bdb_start_top_level_commissioning().

   @return RET_OK on success.  
 */
zb_ret_t zboss_start_no_autostart(void);

#ifdef ZB_PROMISCUOUS_MODE

/**
   Start ZBOSS in the sniffer mode

   Initialize ZBOSS MAC layer to work as a sniffer.
   Once ZBOSS is intialized in the sniffer mode, it can't be commissined in the
   normal mode until reboot.
 */
zb_ret_t zboss_start_in_sniffer_mode(void);

/**
   Start sniffing 

   ZBOSS must be started in the sniffer mode.

   @param channel - channel to work on
   @param data_ind_cb - callback to be called to pass data to the sniffer application
 */
void zboss_sniffer_start(zb_uint8_t channel, zb_callback_t data_ind_cb);

/**
   Stop sniffing or do nothing if not sniffing now.

   ZBOSS must be started in the sniffer mode.
 */
void zboss_sniffer_stop(void);
#endif  /* ZB_PROMISCUOUS_MODE */

/** @} */

/** @addtogroup zb_general_get
@{
*/

/**
   @cond internals_doc
*/
zb_bool_t zb_zdo_joined(void);
/** @endcond */

/**
   Checks that device joined to network.
   @return true if device is joined; false - otherwise
*/
#define ZB_JOINED()  zb_zdo_joined()

/** @} */

/** @addtogroup zb_general_main_loop
@{
*/

/**
   ZBOSS main loop.
   Must be called after ZB_INIT() and zboss_start().
   Loops infinitely.

See any sample
*/
zb_void_t zboss_main_loop(void);

/**
   ZBOSS main loop iteration.

   This function is useful iw application wants to do something special in the
   main loop without involving ZBOSS scheduler.

   Must be called after ZB_INIT() and zboss_start() / zboss_start_no_autostart()
   inside the application's main cycle.

   @snippet on_off_switch_zed.c zboss_main_loop_iteration
*/

zb_void_t zboss_main_loop_iteration(void);

/** @} */

/** @addtogroup zb_general_set
@{
*/

/**
   Set 64-bit long address
   @param addr - long address structure
*/
zb_void_t zb_set_long_address(const zb_ieee_addr_t addr);

/** @} */
/** @addtogroup zb_general_get
@{
*/

/**
   Get 64-bit long address
   @param addr - pointer to memory where result will be stored

   @return 64-bit long address
*/
zb_void_t zb_get_long_address(zb_ieee_addr_t addr);

/** @} */
/** @addtogroup zb_general_set
@{
*/

/**
   Set Extended Pan ID (apsUseExtendedPANID attribute)
   @param ext_pan_id - Long (64-bit) Extented Pan ID
*/
zb_void_t zb_set_extended_pan_id(const zb_ext_pan_id_t ext_pan_id);

/** @} */
/** @addtogroup zb_general_get
@{
*/

/**
   Get Extended Pan ID (nwkExtendedPANId attribute)
   @param ext_pan_id - pointer to memory where result will be stored

   @return Long (64-bit) Extented Pan ID
*/
zb_void_t zb_get_extended_pan_id(zb_ext_pan_id_t ext_pan_id);

/** @} */
/** @addtogroup zb_general_set
@{
*/

/**
   Update ZDP Node Descriptor manufacturer code
   @param manuf_code - Manufacturer code
*/
zb_void_t zb_set_node_descriptor_manufacturer_code(zb_uint16_t manuf_code);

/**
   Initiate device as a ZigBee coordinator
   @param channel_mask - ZigBee channel mask
*/
void zb_set_network_coordinator_role(zb_uint32_t channel_mask);
/**
   Initiate device as a legacy (pre-r21) ZigBee coordinator
   @param channel_mask - ZigBee channel mask
*/
void zb_set_network_coordinator_role_legacy(zb_uint32_t channel_mask);

/**
   Initiate device as a ZigBee router
   @param channel_mask - ZigBee channel mask
*/
void zb_set_network_router_role(zb_uint32_t channel_mask);
/**
   Initiate device as a legacy (pre-r21) ZigBee router
   @param channel_mask - ZigBee channel mask
*/
void zb_set_network_router_role_legacy(zb_uint32_t channel_mask);

/**
   Initiate device as a ZigBee End Device
   @param channel_mask - ZigBee channel mask
*/
void zb_set_network_ed_role(zb_uint32_t channel_mask);

/** @} */
/** @addtogroup zb_general_get
@{
*/

/**
 * Returns the device role ID
 *
 * @return - zb_nwk_device_type_t device_role_id
 */
zb_nwk_device_type_t zb_get_network_role(void);

/** @} */
/** @addtogroup zb_general_set
@{
*/


/**
   Initiate device as a legacy (pre-r21) ZigBee End Device
   @param channel_mask - ZigBee channel mask
*/
void zb_set_network_ed_role_legacy(zb_uint32_t channel_mask);

/**
   Set the maximum number of connected devices. Used for coordinators
   and routers
   @param max_children - maximum number of connected devices.
*/
zb_void_t zb_set_max_children(zb_uint8_t max_children);


/**
 *  Enum all values used in End Device timeout request
 */
typedef enum nwk_requested_timeout_e
{
  ED_AGING_TIMEOUT_10SEC = 0, /*!< 10 second timeout */
  ED_AGING_TIMEOUT_2MIN,      /*!< 2 minutes */
  ED_AGING_TIMEOUT_4MIN,      /*!< 4 minutes */
  ED_AGING_TIMEOUT_8MIN,      /*!< 8 minutes */
  ED_AGING_TIMEOUT_16MIN,     /*!< 16 minutes */
  ED_AGING_TIMEOUT_32MIN,     /*!< 32 minutes */
  ED_AGING_TIMEOUT_64MIN,     /*!< 64 minutes */
  ED_AGING_TIMEOUT_128MIN,    /*!< 128 minutes */
  ED_AGING_TIMEOUT_256MIN,    /*!< 256 minutes */
  ED_AGING_TIMEOUT_512MIN,    /*!< 512 minutes */
  ED_AGING_TIMEOUT_1024MIN,   /*!< 1024 minutes */
  ED_AGING_TIMEOUT_2048MIN,   /*!< 2048 minutes */
  ED_AGING_TIMEOUT_4096MIN,   /*!< 4096 minutes */
  ED_AGING_TIMEOUT_8192MIN,   /*!< 8192 minutes */
  ED_AGING_TIMEOUT_16384MIN   /*!< 16384 minutes */
} nwk_requested_timeout_t;

/**
   Set end device timeout.

   Use it on End Device to specify End Device timeout period used in ED Timeout request.
   @param to - index in @ref nwk_requested_timeout_e
*/
void zb_set_ed_timeout(zb_uint_t to);

/**
   Set keep alive timeout.

   Use it on End Device to specify how often device should poll it's parent.
   @param to - timeout, in beacon intervals
*/
void zb_set_keepalive_timeout(zb_uint_t to);

/** @} */
/** @addtogroup zb_comm_signals
@{
There are tools that allow an application to handle Zigbee events like network formation, association status, leave indication, and others. Due to its nature, different Zigbee events appear asynchronously and ZBOSS informs the application about it through application signals. The application may handle or ignore a received signal. All application signals are processed in the application with a predefined callback function zboss_signal_handler(). It is mandatory for each application to implement this function, even if there is no need to process a signal. 

There are policies that should be taken into account while implementing zboss_signal_handler()  function:
@li	Signal processing should not do long operations synchronously. As soon as cooperative multitasking is implemented by the ZBOSS scheduler, each blocking operation blocks the entire Zigbee stack.
@li	If a valid reference to a memory buffer is passed to an application callback function, the application is responsible for managing this memory buffer. This means that the memory buffer must be either reused or released by the application otherwise this buffer will be lost - memory leak appears.


*/

/**
   ZBOSS application signal handler.

   Mandatory for all applications implemented on the top of ZBOSS stack.
   See @ref zb_zdo_app_signal_type_e.

   @param param - reference to the buffer which contains signal. See @ref zb_get_app_signal.
*/
zb_void_t zboss_signal_handler(zb_uint8_t param);

/*! @} */

/*! \addtogroup zboss_nvram
  @details
There are two predefined dataset IDs for a user application: ZB_NVRAM_APP_DATA1 and ZB_NVRAM_APP_DATA2. It's up to the application to decide what data to store in NVRAM. However, the total data size (all datasets) stored in the NVRAM storage should be less than one virtual page size. To start using NVRAM in the application, the following steps should be done in the application:

@li	Define an application specific data structure to be stored in the NVRAM.
@li	Register callbacks to write, read data and get size of data to store.
@li	Once write application data callback is called, it gets a flash page number and a page offset value to store data. The callback must write data using an API zb_osif_nvram_write(). The data size must be the same as returned by a get data size callback.
@li	Once get data size callback is called, it must return the size of the data being stored.
@li	Once the read application data callback is called, it gets a flash page number, a page offset value, and a payload size of data to read. The callback reads the data using an API zb_osif_nvram_read() with the provided addressing information as input parameters.
@li	At runtime trigger storing data to NVRAM with a call to zb_nvram_write_dataset().

*/
/*! @{ */

/**
 * Enum All NVRAM dataset types
 *
 * ZB_NVRAM_DATASET_NUMBER - count of dataset types. Not a real dataset type!
 */

typedef enum zb_nvram_dataset_types_e
{
  /* Do not use 0 as dataset type: it can be used internally */
  ZB_NVRAM_RESERVED              = 0,
  ZB_NVRAM_COMMON_DATA           = 1, /**< Dataset, contains common ZigBee data */
  ZB_NVRAM_HA_DATA               = 2, /**< Dataset, contains HA profile ZigBee data */
  ZB_NVRAM_ZCL_REPORTING_DATA    = 3, /**< Dataset, contains ZCL reporting data */
  /* FIXME: Remove gaps and modify the order of datasets: ADDR_MAP and NEIGHBOUR_TBL should be
   * restored right after the COMMON_DATA, other datasets - after these two.
   * Rationale: some datasets (such as APS_SECURE_DATA) use addr_map reference on restoring, which
   * should be filled before. We are restoring datasets in strong order defined by this enum!
   * WARNING: It will break the compability with old nvram at all. Implement special migration case
   * if needed (read datasets with old types enum, and then work with new). */
  ZB_NVRAM_APS_SECURE_DATA_GAP   = 4,
  ZB_NVRAM_APS_BINDING_DATA_GAP  = 5,
  ZB_NVRAM_HA_POLL_CONTROL_DATA  = 6, /**< Dataset, contains HA POLL CONTROL data */
  ZB_IB_COUNTERS                 = 7, /**< Dataset, contains NIB outgoing frame counter */
  ZB_NVRAM_DATASET_GRPW_DATA     = 8, /**< Green Power dataset */
  ZB_NVRAM_APP_DATA1             = 9, /**< Application-specific data #1 */
  ZB_NVRAM_APP_DATA2             = 10, /**< Application-specific data #2 */
  ZB_NVRAM_ADDR_MAP              = 11, /**< Dataset stores address map info */
  ZB_NVRAM_NEIGHBOUR_TBL         = 12, /**< Dataset stores Neighbour table info */
  ZB_NVRAM_INSTALLCODES          = 13, /**< Dataset contains APS installcodes data */
  ZB_NVRAM_APS_SECURE_DATA       = 14, /**< Dataset, contains APS secure keys data */
  ZB_NVRAM_APS_BINDING_DATA      = 15, /**< Dataset, contains APS binding data */
  ZB_NVRAM_DATASET_GP_PRPOXYT    = 16, /**< Green Power Proxy table */
  ZB_NVRAM_DATASET_GP_SINKT      = 17, /**< Green Power Sink table */
  ZB_NVRAM_DATASET_GP_CLUSTER    = 18, /**< Green Power Cluster data */
  ZB_NVRAM_APS_GROUPS_DATA       = 19, /**< Dataset, contains APS groups data */
  ZB_NVRAM_DATASET_NUMBER,             /**< Count of Dataset */
  ZB_NVRAM_DATA_SET_TYPE_PAGE_HDR = 0x1e /**< Special internal dataset type  */
} zb_nvram_dataset_types_t;


/**
 * Declares application callback used for reading application datasets from NVRAM.
 *
 * @param page - page in NVRAM from data will be read
 * @param pos - offset in page
 * @param payload_length - number of bytes to read
 */
typedef void (*zb_nvram_read_app_data_t)(zb_uint8_t page, zb_uint32_t pos, zb_uint16_t payload_length);
/**
 * Declares application callback used for writing application datasets into NVRAM.
 *
 * @param page - page in NVRAM where data will be stotred
 * @param pos - offset in page
 *
 * @return - RET_OK on successful write
 */
typedef zb_ret_t (*zb_nvram_write_app_data_t)(zb_uint8_t page, zb_uint32_t pos);
/**
 * Returns size of application datasets
 *
 * @return - size of application dataset in bytes
 */
typedef zb_uint16_t (*zb_nvram_get_app_data_size_t)(void);

/**
 * Register application callback for reading @ref ZB_NVRAM_APP_DATA1 from NVRAM
 *
 * @param cb - callback that will be called upon reading dataset
 */
void zb_nvram_register_app1_read_cb(zb_nvram_read_app_data_t cb);
/**
 * Register application callback for reading @ref ZB_NVRAM_APP_DATA2 from NVRAM
 *
 * @param cb - callback that will be called upon reading dataset
 */
void zb_nvram_register_app2_read_cb(zb_nvram_read_app_data_t cb);

/**
 * Register application callback for writing @ref ZB_NVRAM_APP_DATA1 to NVRAM
 *
 * @param wcb - callback that will be called upon writing
 * @param gcb - this callback will be called upon writing to determine dataset size
 */
void zb_nvram_register_app1_write_cb(
    zb_nvram_write_app_data_t wcb,
    zb_nvram_get_app_data_size_t gcb);

/**
 * Register application callback for writing @ref ZB_NVRAM_APP_DATA2 to NVRAM
 *
 * @param wcb - callback that will be called upon writing
 * @param gcb - this callback will be called upon writing to determine dataset size
 */
void zb_nvram_register_app2_write_cb(
    zb_nvram_write_app_data_t wcb,
    zb_nvram_get_app_data_size_t gcb);

/**
 * Write specified dataset into NVRAM
 *
 * @param t - dataset index, see @ref zb_nvram_dataset_types_e
 *
 * @b Example @b #1:
 * @code
 *   zb_nvram_write_dataset(ZB_NVRAM_APP_DATA1);
 *   zb_nvram_write_dataset(ZB_NVRAM_APP_DATA2);
 * @endcode
 *
 * @n
 * @b Example @b #2 (Using application callbacks):@n
 * @b Define @b user @b dataset @b type:
 * @snippet cn_nsa_tc_02_common.h nvram_app_dataset_usage1
 *
 * @b Callback @b prototypes:
 * @snippet cn_nsa_tc_02_thc1.c nvram_app_dataset_usage2
 *
 * @b Register @b application @b callbacks:
 * @snippet cn_nsa_tc_02_thc1.c nvram_app_dataset_usage3
 *
 * @b Callback @b implementation @b and @b usage:
 * @snippet cn_nsa_tc_02_thc1.c nvram_app_dataset_usage4
 *
 * See CN-NSA-TC-02
 */
void zb_nvram_write_dataset(zb_nvram_dataset_types_t t);
/** @cond internals_doc */
/**
 * Clears all datasets except @ref ZB_IB_COUNTERS and application datasets.
 */
void zb_nvram_clear(void);
/** @endcond */
/**
   Enable or disable NVRAM erasing on every application startup.
   @param erase - 1 to enable erasing; 0 - disable.
   Erasing is disabled by default.
*/
zb_void_t zb_set_nvram_erase_at_start(zb_bool_t erase);
zb_void_t zb_set_nvram_erase_at_reset(zb_bool_t erase);


/**
   Start NVRAM transaction.

   Start writing multiple datasets at once.
   A transaction must finished by calling zb_nvram_transaction_commit().
 */
void zb_nvram_transaction_start(void);

/**
   Commit NVRAM transaction.

   Finish writing multiple datasets at once.
   A transaction must started by calling zb_nvram_transaction_start().
 */
void zb_nvram_transaction_commit(void);

#define ZB_INVALID_TX_POWER_VALUE     0x7F

#ifdef ZB_PRODUCTION_CONFIG
#define ZB_PRODUCTION_CONFIG_MAX_SIZE 128

typedef ZB_PACKED_PRE struct zb_production_config_hdr_t
{
  zb_uint32_t crc; /*!< crc for the whole production configuration (including application specific part) */
  zb_uint16_t len; /*!< length of production configuration (including application specific part) */
  zb_uint16_t version; /*!< Version of production configuration (reserved for future changes) */
}
ZB_PACKED_STRUCT zb_production_config_hdr_t;

typedef ZB_PACKED_PRE struct zb_production_config_s
{
  zb_production_config_hdr_t hdr; /*!< Header */
  zb_uint32_t aps_channel_mask; /*!< Channel mask for device to operate on */
  zb_64bit_addr_t extended_address; /*!< IEEE address */
  zb_int8_t mac_tx_power[16]; /*!< Tx power specified for every possible channel */
  zb_uint8_t install_code[ZB_CCM_KEY_SIZE+2]; /*!< Installation code + its own crc */
}
ZB_PACKED_STRUCT zb_production_config_t;

#endif /* ZB_PRODUCTION_CONFIG */

/*! @} */

#ifdef ZB_USE_SLEEP
/*! @addtogroup sleep_api  */
/*! @{ */

/** @cond internals_doc */
/** Default sleep threshold. Do not sleep when it is less then 1 Beacon Interval to wake up. */
#define ZB_SCHED_SLEEP_THRESHOLD_MS 20

/** Maximum sleep threshold. */
#define ZB_MAXIMUM_SLEEP_THRESHOLD_MS 86400000

/**
  Set sleep threshold on device; when scheduler detects that device can be put in sleep mode
  it will notify application with signal @see ZB_COMMON_SIGNAL_CAN_SLEEP.
  Device can not be put into sleep mode when slFeep interval less than this threshold.

  @param threshold_ms - sleep threshold in milliseconds
             If threshold is 0, means zero threshold, application will be notified each time when stack is ready to sleep
             (no immediate callbacks in quee).
  @return RET_OK if new threshold is valid and applied, RET_ERROR if user wants to set threshold
          greater that @ref ZB_MAXIMUM_SLEEP_THRESHOLD_MS or lesser than ZB_SCHED_SLEEP_THRESHOLD_MS.
*/
zb_ret_t zb_sleep_set_threshold(zb_uint32_t threshold_ms);


/**
  Get sleep threshold value from stack.
  @return threshold value in milliseconds
*/
zb_uint32_t zb_get_sleep_threshold(void);
/** @endcond */

/**
  Blocking function responsible for putting device into sleep mode.

*/
zb_void_t zb_sleep_now(void);

/*! @} */
#endif /* ZB_USE_SLEEP */

#ifdef ZB_EXTERNAL_MAC
/* 12/08/2017 EE CR:MAJOR Remove MAC-specific stuff form the upper level API */
zb_void_t zb_set_mac_transport(zb_uint8_t* dev);
#endif

#endif /*ZBOSS_API_H*/
