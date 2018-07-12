/**
 * ZBOSS Zigbee 3.0
 * 
 * Copyright (c) 2012-2018 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * 
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
 * 
 * PURPOSE: Electrical Measurement cluster defintions
*/

#if ! defined ZB_ZCL_ELECTRICAL_MEASUREMENT_H
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_H

#include "zcl/zb_zcl_common.h"
#include "zcl/zb_zcl_commands.h"


/** @cond DOXYGEN_ZCL_SECTION */

/* Cluster ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT */

/*! @addtogroup ZB_ZCL_ELECTRICAL_MEASUREMENT
    @{
    @name Electrical Measurement cluster attributes
    @{
*/

/*! @brief Electrical Measurement cluster attribute identifiers
    @see HA spec, subclause 9.2.2.2
*/
enum zb_zcl_electrical_measurement_attr_e
{
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID                            = 0x0000,  /**< This attribute indicates a device’s measurement capabilities. */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_DCPOWER_ID                                     = 0x0106,  /**< The @e DCPower attribute represents the most recent DC power
                                                                                                * reading in @e Watts (W)
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_ACTIVE_POWER_ID                          = 0x0304,  /**< Active power represents the current demand of active power
                                                                                                * delivered or received at the premises, in @e kW
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_TOTAL_APPARENT_POWER_ID                        = 0x0306,  /**< Represents the current demand of apparent power, in @e kVA */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_MULTIPLIER_ID                            = 0x0402,  /**< Provides a value to be multiplied against a raw or uncompensated
                                                                                                * sensor count of power being measured by the metering device.
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_DEVISOR_ID                               = 0x0403,  /**< Provides a value to divide against the results of applying the
                                                                                                * @e Multiplier attribute against a raw or uncompensated sensor
                                                                                                * count of power being measured by the metering device.
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_LINE_CURRENT_ID                                = 0x0501,  /**< Represents the single phase or Phase A, AC line current
                                                                                                * (Square root of active and reactive current) value at
                                                                                                * the moment in time the attribute is read, in @e Amps (A).
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID                                  = 0x0505,  /**< Represents the most recent RMS voltage reading in @e Volts (V). */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID                                  = 0x0508,  /**< Represents the most recent RMS current reading in @e Amps (A). */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_ID                                = 0x050B,  /**< Represents the single phase or Phase A, current demand of active
                                                                                                * power delivered or received at the premises, in @e Watts (W).
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_ID                              = 0x050F,  /**< Represents the single phase or Phase A, current demand of
                                                                                                 * apparent (Square root of active and reactive power) power, in @e VA.
                                                                                                 */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_FACTOR_ID                                = 0x0510,  /**< Contains the single phase or PhaseA, Power Factor ratio in 1/100ths. */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_ID       = 0x0511,  /**< The Period in seconds that the RMS voltage is averaged over. */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_ID                        = 0x0600,  /**< Provides a value to be multiplied against the @e InstantaneousVoltage and
                                                                                                 * @e RMSVoltage attributes.
                                                                                                 */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_ID                           = 0x0601,  /**< Provides a value to be divided against the @e InstantaneousVoltage and
                                                                                                 * @e RMSVoltage attributes. This attribute must be used in conjunction
                                                                                                 * with the @e ACVoltageMultiplier attribute. 0x0000 is an invalid value
                                                                                                 * for this attribute.
                                                                                                 */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_ID                        = 0x0602,  /**< Provides a value to be multiplied against the @e InstantaneousCurrent
                                                                                                * and @e RMSCurrent attributes
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_ID                           = 0x0603,  /**< Provides a value to be divided against the @e ACCurrent,
                                                                                                * @e InstantaneousCurrent and @e RMSCurrent attributes.
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_ID                          = 0x0604,  /**< Provides a value to be multiplied against the @e InstantaneousPower
                                                                                                * and @e ActivePower attributes
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_ID                             = 0x0605,  /**< Provides a value to be divided against the @e InstantaneousPower
                                                                                                * and @e ActivePower attributes.
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID                              = 0x0905,  /**< Represents the most recent RMS voltage reading in @e Volts (V). */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID                              = 0x0908,  /**< Represents the most recent RMS current reading in @e Amps (A). */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHB_ID                            = 0x090B,  /**< Represents the Phase B, current demand of active power delivered or
                                                                                                * received at the premises, in @e Watts (W).
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHB_ID                          = 0x090F,  /**< Represents the Phase B, current demand of apparent (Square root
                                                                                                * of active and reactive power) power, in @e VA.
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHB_ID   = 0x0911,  /**< The number of times the average RMS voltage, has been above the
                                                                                                * @e AverageRMS @e OverVoltage threshold since last reset.
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID                              = 0x0A05,  /**< Represents the most recent RMS voltage reading in @e Volts (V).*/
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID                              = 0x0A08,  /**< Represents the most recent RMS current reading in @e Amps (A).*/
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHC_ID                            = 0x0A0B,  /**< Represents the Phase C, current demand of active power delivered
                                                                                                * or received at the premises, in @e Watts (W).
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_APPARENT_POWER_PHC_ID                          = 0x0A0F,  /**< Represents the Phase C, current demand of apparent (Square root of
                                                                                                * active and reactive power) power, in @e VA.
                                                                                                */
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AVERAGE_RMSVOLTAGE_MEASUREMENT_PERIOD_PHC_ID   = 0x0A11,  /**< The Period in seconds that the RMS voltage is averaged over*/
};

/*! @brief Bit mapping for Measurement Type attribute,
    @see HA spec, subclause 9.2.2.2.1.1 */
enum zb_zcl_electrical_measurement_measurement_type_e
{
  /*! Active Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_ACTIVE_MEASUREMENT          = 0x00000001,
  /*! Reactive Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_REACTIVE_MEASUREMENT        = 0x00000002,
  /*! Apparent Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_APPARENT_MEASUREMENT        = 0x00000004,
  /*! Phase A Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_PHASE_A_MEASUREMENT         = 0x00000008,
  /*! Phase B Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_PHASE_B_MEASUREMENT         = 0x00000010,
  /*! Phase C Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_PHASE_C_MEASUREMENT         = 0x00000020,
  /*! DC Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_DC_MEASUREMENT              = 0x00000040,
  /*! Harmonic Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_HARMONIC_MEASUREMENT        = 0x00000080,
  /*! Power Quality Measurement bit */
  ZB_ZCL_ELECTRICAL_MEASUREMENT_POWER_QUALITY_MEASUREMENT   = 0x00000100,

  ZB_ZCL_ELECTRICAL_MEASUREMENT_RESERVED                    = 0x00000200
};

/** @brief Default value for Measurement Type attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_DEFAULT_VALUE 0x00000000

/** @brief Default value for DCPower attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_DCPOWER_DEFAULT_VALUE 0x8000

/** @brief Default value for RMSVoltage attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_DEFAULT_VALUE 0xffff

/** @brief Default value for RMSCurrent attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSCURRENT_DEFAULT_VALUE 0xffff

/** @brief Default value for ActivePower attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_DEFAULT_VALUE 0xffff

/** @brief Default value for RMSCurrent attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_POWER_FACTOR_DEFAULT_VALUE 0x0

/** @brief Default value for PowerMultiplier attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_DEFAULT_VALUE 0x0001

/** @brief Default value for PowerDivisor attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_DEFAULT_VALUE 0x0001

/** @brief Default value for ACCurrentMultiplier attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_DEFAULT_VALUE 0x0001

/** @brief Default value for ACCurrentDivisor attribute */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_DEFAULT_VALUE 0x0001



/*! @} */ /* Electrical Measurement cluster attributes */

/*! @name Electrical Measurement cluster commands
    @{
*/

/*! @} */ /* Electrical Measurement cluster commands */

/*! @cond internals_doc
    @internal @name Electrical Measurement cluster internals
    Internal structures for attribute representation in cluster definitions.
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID(data_ptr) \
{                                                                                               \
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                                                    \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                 \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_DCPOWER_ID(data_ptr) \
{                                                                                      \
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_DCPOWER_ID,                                       \
  ZB_ZCL_ATTR_TYPE_S16,                                                                \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                         \
  (zb_voidp_t) data_ptr                                                                \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID(data_ptr) \
{                                                                                         \
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                   \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                            \
  (zb_voidp_t) data_ptr                                                                   \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID(data_ptr) \
{                                                                                         \
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                   \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                            \
  (zb_voidp_t) data_ptr                                                                   \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_ID(data_ptr)     \
{                                                                                               \
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_ID,                                           \
  ZB_ZCL_ATTR_TYPE_S16,                                                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                                  \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_FACTOR_ID(data_ptr)     \
{                                                                                               \
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_FACTOR_ID,                                           \
  ZB_ZCL_ATTR_TYPE_S8,                                                                          \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                                  \
  (zb_voidp_t) data_ptr                                                                         \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_ID(data_ptr) \
{                                                                                                 \
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                           \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                   \
  (zb_voidp_t) data_ptr                                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_ID(data_ptr) \
{                                                                                              \
  ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                                        \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                                \
  (zb_voidp_t) data_ptr                                                                        \
}


/** @internal @brief Declare attribute list for Electrical Measurement cluster
    @param attr_list - attribure list name
    @param measurement_type - pointer to variable to store Measurement Type attribute value
    @param dcpower - pointer to variable to store DCPower attribute value
*/
#define ZB_ZCL_DECLARE_ELECTRICAL_MEASUREMENT_ATTRIB_LIST(attr_list, measurement_type, dcpower)    \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                      \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID, (measurement_type)) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_DCPOWER_ID, (dcpower))                   \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @internal Number of attributes mandatory for reporting in Electrical Measurement cluster */
#define ZB_ZCL_ELECTRICAL_MEASUREMENT_REPORT_ATTR_COUNT 2

/*! @}
    @endcond */ /* Electrical Measurement cluster internals */

/*! @} */ /* ZCL HA Electrical Measurement cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_el_measurement_init_server();
zb_void_t zb_zcl_el_measurement_init_client();

#define ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_el_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_el_measurement_init_client

#endif /* ! defined ZB_ZCL_ELECTRICAL_MEASUREMENT_H */
