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
 PURPOSE: Public Network layer API
*/
#ifndef ZB_ZBOSS_API_NWK_H
#define ZB_ZBOSS_API_NWK_H 1

/** \addtogroup nwk_api */
/** @{ */

/** @name NWK common constants and API.
 *  @{
 */

/** @brief Network broadcast addresses types. */
typedef enum zb_nwk_broadcast_address_e
{
  ZB_NWK_BROADCAST_ALL_DEVICES        = 0xFFFF, /**< All devices in PAN */
  ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE    = 0xFFFD, /**< macRxOnWhenIdle = TRUE */
  ZB_NWK_BROADCAST_ROUTER_COORDINATOR = 0xFFFC, /**< All routers and coordinator */
  ZB_NWK_BROADCAST_LOW_POWER_ROUTER   = 0xFFFB, /**< Low power routers only */

  ZB_NWK_BROADCAST_RESERVED           = 0xFFF8
}
zb_nwk_broadcast_address_t;

/** @brief Check that address is broadcast.
  * @param addr - 16-bit address
  * @return TRUE if address is broadcast, FALSE otherwhise
  */
/* #define ZB_NWK_IS_ADDRESS_BROADCAST(addr) ( ((addr) & 0xFFF0) == 0xFFF0 ) */
#define ZB_NWK_IS_ADDRESS_BROADCAST(addr) ( (addr) >= ZB_NWK_BROADCAST_RESERVED )

/**
   Network device type
*/
typedef enum zb_nwk_device_type_e
{
  ZB_NWK_DEVICE_TYPE_COORDINATOR,       /*!< Device - Coordinator */
  ZB_NWK_DEVICE_TYPE_ROUTER,            /*!< Device - Router */
  ZB_NWK_DEVICE_TYPE_ED,                /*!< Device - End device */
  ZB_NWK_DEVICE_TYPE_NONE               /*!< Unknown Device */
}
zb_nwk_device_type_t;


/** @brief Leave types */
typedef enum zb_nwk_leave_type_e
{
  ZB_NWK_LEAVE_TYPE_RESET                            = 0x00, /**< Leave without rejoin */
  ZB_NWK_LEAVE_TYPE_REJOIN                           = 0x01  /**< Leave with rejoin */
} zb_nwk_leave_type_t;


/**
   LQI undefined value
 */
#define ZB_MAC_LQI_UNDEFINED 0
/**
   RSSI undefined value
 */
#define ZB_MAC_RSSI_UNDEFINED 0x7f


/**
   Number of the first channel
*/
#define ZB_MAC_START_CHANNEL_NUMBER 11
/**
   Maximal number of the channels
 */
#define ZB_MAC_MAX_CHANNEL_NUMBER   26
/**
   Total number of supported channels
 */
#define ZB_MAC_SUPPORTED_CHANNELS   (ZB_MAC_MAX_CHANNEL_NUMBER - ZB_MAC_START_CHANNEL_NUMBER + 1)

/** @} */

/** @name PIB cache
 *  @{
 */

/**
  Data structure used to cache frequently used PIB attributes.

  Useful whe using alien MAC layer without direct access to PIB: blockable "get"
  interface to PIB is too complex and slow.
 */
typedef struct zb_nwk_pib_cache_s
{
  zb_uint16_t             mac_short_address;             /*!< The 16-bit address that the device uses
                                                           to communicate in the PAN. */
  zb_uint16_t             mac_pan_id;                    /*!< The 16-bit identifier of the PAN on which
                                                           the device is operating. If this value is 0xffff,
                                                           the device is not associated. */
  zb_ieee_addr_t          mac_extended_address;          /*!< The 64-bit (IEEE) address assigned to the device. */

  zb_uint8_t              mac_association_permit;        /*!< Indication of whether a coordinator is currently
                                                           allowing association. A value of TRUE indicates*/
  zb_uint8_t              mac_rx_on_when_idle;           /*!< Indication of whether the MAC sublayer is to enable
                                                           its receiver during idle periods. */
  zb_uint8_t              phy_current_channel;           /*!< Index of current physical channel */
} zb_nwk_pib_cache_t;

/** @} */


/**
  * @name PIB cache mirror
  * \par Here are cache of PIB / NIB mirror of PIB values.
  *
  * After that values modified in NWK or upper, must sync it with MAC using MLME-SET.
  *
  * @{
  */

/**
  Function used to access PIB cache.
  @return pointer to PIB cache.
  */
zb_nwk_pib_cache_t *zb_nwk_get_pib_cache(void);
/**
  Macro used to access PIB cache.
  @return pointer to PIB cache.
  */
#define ZB_PIB_CACHE() zb_nwk_get_pib_cache()

/** Cached value of device network address */
#define ZB_PIBCACHE_NETWORK_ADDRESS()  ZB_PIB_CACHE()->mac_short_address
/** Cached value of device PAN ID */
#define ZB_PIBCACHE_PAN_ID()           ZB_PIB_CACHE()->mac_pan_id
/** Cached value of device extended address */
#define ZB_PIBCACHE_EXTENDED_ADDRESS() ZB_PIB_CACHE()->mac_extended_address
/** Cached value of RxOnWhenIdle attribute */
#define ZB_PIBCACHE_RX_ON_WHEN_IDLE()  ZB_PIB_CACHE()->mac_rx_on_when_idle
/** Cached value of AssociationPermit attribute */
#define ZB_PIBCACHE_ASSOCIATION_PERMIT() ZB_PIB_CACHE()->mac_association_permit
/** Cached value of CurrentChannel attribute */
#define ZB_PIBCACHE_CURRENT_CHANNEL()  ZB_PIB_CACHE()->phy_current_channel

    
/**
   Arguments of the NLME-PERMIT_JOINING.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_permit_joining_request_s
{
  zb_uint8_t permit_duration; /**< Time in seconds during which the coordinator
                               * or router will allow associations */
} ZB_PACKED_STRUCT
zb_nlme_permit_joining_request_t;

/**
   NLME-PERMIT-JOINING.request primitive

   Allow/disallow network joining

   @param param - buffer containing parameters - @see
   zb_nlme_network_formation_request_t
   @return RET_OK on success, error code otherwise.

   @snippet tp_pro_bv_18_zr2.c zb_nlme_permit_joining_request
 */
void zb_nlme_permit_joining_request(zb_uint8_t param);
/** @} */

/** @} */

#endif /*#ifndef ZB_ZBOSS_API_NWK_H*/
