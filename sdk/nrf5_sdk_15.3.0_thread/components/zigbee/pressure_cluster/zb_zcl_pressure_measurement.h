/**
 * Copyright (c) 2018 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef ZB_ZCL_PRESSURE_MEASUREMENT_H__
#define ZB_ZCL_PRESSURE_MEASUREMENT_H__

#include "zcl/zb_zcl_common.h"

/* Cluster ZB_ZCL_CLUSTER_ID_PRESSURE_MEASUREMENT */

/**@brief Pressure Measurement cluster attributes according to ZCL Specification 4.5.2.1.1. */
typedef struct
{
    zb_int16_t  measure_value;
    zb_int16_t  min_measure_value;
    zb_int16_t  max_measure_value;
    zb_uint16_t tolerance;
} zb_zcl_pressure_measurement_attrs_t;

/*@brief Pressure Measurement cluster attribute identifiers
  @see ZCL spec, Pressure Measurement Cluster 4.5.2.2.1
*/
enum zb_zcl_pressure_measurement_attr_e
{
    /*@brief MeasuredValue, ZCL spec 4.5.2.2.1.1 */
    ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID = 0x0000,
    /*@brief MinMeasuredValue, ZCL spec 4.5.2.2.1.2 */
    ZB_ZCL_ATTR_PRES_MEASUREMENT_MIN_VALUE_ID = 0x0001,
    /*@brief MaxMeasuredValue, ZCL spec 4.5.2.2.1.3 */
    ZB_ZCL_ATTR_PRES_MEASUREMENT_MAX_VALUE_ID = 0x0002,
    /*@brief Tolerance, ZCL spec 4.5.2.2.1.4 */
#ifndef ZB_DISABLE_PRESSURE_MEASUREMENT_TOLERANCE_ID
    ZB_ZCL_ATTR_PRES_MEASUREMENT_TOLERANCE_ID = 0x0003,
#else
    ZB_ZCL_ATTR_PRESSURE_MEASUREMENT_TOLERANCE_ID = 0xF003,
#endif
};

/**@brief MeasuredValue attribute unknown value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_UNKNOWN                  ((zb_int16_t)0x8000)

/**@brief MinMeasuredValue attribute minimum value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_MIN_VALUE_MIN_VALUE            ((zb_int16_t)0x8001)

/**@brief MinMeasuredValue attribute maximum value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_MIN_VALUE_MAX_VALUE            0x7FFE

/**@brief MinMeasuredValue attribute invalid value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_MIN_VALUE_INVALID              ((zb_int16_t)0x8000)

/**@brief MaxMeasuredValue attribute minimum value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_MAX_VALUE_MIN_VALUE            ((zb_int16_t)0x8002)

/**@brief MaxMeasuredValue attribute maximum value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_MAX_VALUE_MAX_VALUE            0x7FFF

/**@brief MaxMeasuredValue attribute invalid value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_MAX_VALUE_INVALID              ((zb_int16_t)0x8000)


 /* (See: Table 4.13 Temperature Measurement Information Attribute Set) */

/**@brief Tolerance attribute minimum value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_TOLERANCE_MIN_VALUE            0x0000

/**@brief Tolerance attribute maximum value. */
#define ZB_ZCL_ATTR_PRES_MEASUREMENT_TOLERANCE_MAX_VALUE            0x0800

/**@brief Default value for Value attribute. */
#define ZB_ZCL_PRES_MEASUREMENT_VALUE_DEFAULT_VALUE ((zb_int16_t)0x0)

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID(data_ptr) \
{                                                                              \
    ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_S16,                                                      \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,               \
    (zb_voidp_t) data_ptr                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_MIN_VALUE_ID(data_ptr) \
{                                                                                  \
    ZB_ZCL_ATTR_PRES_MEASUREMENT_MIN_VALUE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_S16,                                                          \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                  \
    (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_MAX_VALUE_ID(data_ptr) \
{                                                                                  \
    ZB_ZCL_ATTR_PRES_MEASUREMENT_MAX_VALUE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_S16,                                                          \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                  \
    (zb_voidp_t) data_ptr                                                          \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_TOLERANCE_ID(data_ptr) \
{                                                                                  \
    ZB_ZCL_ATTR_PRES_MEASUREMENT_TOLERANCE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_U16,                                                          \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                   \
    (zb_voidp_t) data_ptr                                                          \
}

/**@brief Declares attribute list for the Pressure Measurement cluster on the server side.
 *
 * @param attr_list    Attribute list name.
 * @param value        Pointer to the variable to store the MeasuredValue attribute.
 * @param min_value    Pointer to the variable to store the MinMeasuredValue attribute.
 * @param max_value    Pointer to the variable to store the MaxMeasuredValue attribute.
 * @param tolerance    Pointer to the variable to store the Tolerance attribute.
 */
#define ZB_ZCL_DECLARE_PRES_MEASUREMENT_ATTRIB_LIST(attr_list,                  \
    value, min_value, max_value, tolerance)                                     \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID, (value))          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_MIN_VALUE_ID, (min_value))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_MAX_VALUE_ID, (max_value))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_TOLERANCE_ID, (tolerance))  \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/**@brief Function initialising the server side of Pressure Measurement Cluster. */
zb_void_t zb_zcl_pres_measurement_init_server(void);
/**@brief Function initialising the client side of Pressure Measurement Cluster. */
zb_void_t zb_zcl_pres_measurement_init_client(void);

/**@brief Defines needed for the stack to initialise the cluster correctly. */
#define ZB_ZCL_CLUSTER_ID_PRESSURE_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_pres_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_PRESSURE_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_pres_measurement_init_client

#endif /* ZB_ZCL_PRESSURE_MEASUREMENT_H__ */
