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
 * PURPOSE: ZLL On/Off Plugin Unit device definition
*/

#if ! defined ZB_ZLL_ON_OFF_PLUGIN_UNIT_H
#define ZB_ZLL_ON_OFF_PLUGIN_UNIT_H

/**
 *  @defgroup ZB_ZLL_ON_OFF_PLUGIN_UNIT On/Off Plugin Unit device.
 *  @ingroup ZB_ZLL_DEVICES
 *  @addtogroup ZB_ZLL_ON_OFF_PLUGIN_UNIT
 *  ZLL On/Off Plugin Unit device.
 *  @{
    @details
    On/Off Plugin Unit device has 5 clusters (see spec 5.2.2): \n
        - @ref ZB_ZCL_BASIC \n
        - @ref ZB_ZCL_IDENTIFY \n
        - @ref ZB_ZCL_GROUPS \n
        - @ref ZB_ZCL_SCENES \n
        - @ref ZB_ZCL_ON_OFF

    Sample use On/Off Plugin Unit device

    @par Example
    Delcaration clusters
    @snippet on_off_plugin_unit.c COMMON_DECLARATION
    Register device list
    @snippet on_off_plugin_unit.c REGISTER
    @par

    See ll/all_devices test
*/

#if defined ZB_ZLL_DEFINE_DEVICE_ON_OFF_PLUGIN_UNIT || defined DOXYGEN

#define ZB_ZLL_DEVICE_VER_ON_OFF_PLUGIN_UNIT 0      /**< Device version */

#define ZB_ZLL_ON_OFF_PLUGIN_UNIT_IN_CLUSTER_NUM  6 /**< On/Off Plugin Unit input clusters number. */
#define ZB_ZLL_ON_OFF_PLUGIN_UNIT_OUT_CLUSTER_NUM 0 /**< On/Off Plugin Unit output clusters number. */

/*! @internal Number of attribute for reporting on On/Off Plugin Unit device */
#define ZB_ZLL_ON_OFF_PLUGIN_UNIT_REPORT_ATTR_COUNT \
  (ZB_ZCL_ON_OFF_REPORT_ATTR_COUNT)

/**
 *  @brief Declare cluster list for On/Off Plugin Unit device.
 *  @param cluster_list_name [IN] - cluster list variable name.
 *  @param basic_attr_list [IN] - attribute list for Basic cluster.
 *  @param identify_attr_list [IN] - attribute list for Identify cluster.
 *  @param groups_attr_list [IN] - attribute list for Groups cluster.
 *  @param scenes_attr_list [IN] - attribute list for Scenes cluster.
 *  @param on_off_attr_list [IN] - attribute list for On/Off cluster.
 *  @param commissioning_declare_both [IN] - determines Commissioning cluster role: ZB_TRUE implies
 *  both client and server, and ZB_FALSE implies server role only.
 */
#define ZB_ZLL_DECLARE_ON_OFF_PLUGIN_UNIT_CLUSTER_LIST(                                         \
  cluster_list_name,                                                                            \
  basic_attr_list,                                                                              \
  identify_attr_list,                                                                           \
  groups_attr_list,                                                                             \
  scenes_attr_list,                                                                             \
  on_off_attr_list,                                                                             \
  commissioning_declare_both)                                                                   \
      zb_zcl_cluster_desc_t cluster_list_name[] =                                               \
      {                                                                                         \
        ZB_ZCL_CLUSTER_DESC(                                                                    \
          ZB_ZCL_CLUSTER_ID_BASIC,                                                              \
          ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),                                    \
          (basic_attr_list),                                                                    \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
          ZB_ZCL_MANUF_CODE_INVALID                                                             \
        ),                                                                                      \
        ZB_ZCL_CLUSTER_DESC(                                                                    \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                           \
          ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),                                 \
          (identify_attr_list),                                                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
          ZB_ZCL_MANUF_CODE_INVALID                                                             \
        ),                                                                                      \
        ZB_ZCL_CLUSTER_DESC(                                                                    \
          ZB_ZCL_CLUSTER_ID_GROUPS,                                                             \
          ZB_ZCL_ARRAY_SIZE(groups_attr_list, zb_zcl_attr_t),                                   \
          (groups_attr_list),                                                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
          ZB_ZCL_MANUF_CODE_INVALID                                                             \
        ),                                                                                      \
        ZB_ZCL_CLUSTER_DESC(                                                                    \
          ZB_ZCL_CLUSTER_ID_SCENES,                                                             \
          ZB_ZCL_ARRAY_SIZE(scenes_attr_list, zb_zcl_attr_t),                                   \
          (scenes_attr_list),                                                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
          ZB_ZCL_MANUF_CODE_INVALID                                                             \
        ),                                                                                      \
        ZB_ZCL_CLUSTER_DESC(                                                                    \
          ZB_ZCL_CLUSTER_ID_ON_OFF,                                                             \
          ZB_ZCL_ARRAY_SIZE(on_off_attr_list, zb_zcl_attr_t),                                   \
          (on_off_attr_list),                                                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                                           \
          ZB_ZCL_MANUF_CODE_INVALID                                                             \
        ),                                                                                      \
        ZB_ZCL_CLUSTER_DESC(                                            \
          ZB_ZLL_CLUSTER_ID_COMMISSIONING,                              \
          0,                                                            \
          NULL,                                                         \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_MANUF_CODE_INVALID                                     \
          ),                                                            \
        ZB_ZCL_CLUSTER_DESC(                                            \
          ZB_ZLL_CLUSTER_ID_COMMISSIONING,                              \
          0,                                                            \
          NULL,                                                         \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                   \
          ZB_ZCL_MANUF_CODE_INVALID                                     \
          )                                                             \
      }

/**
 *  @brief Declare simple descriptor for On/Off Plugin Unit device.
 *  @param ep_name - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param in_clust_num [IN] - number of supported input clusters.
 *  @param out_clust_num [IN] - number of supported output clusters.
 *  @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
 *  definitions, because these values are used to form simple descriptor type name.
 */
#define ZB_ZCL_DECLARE_ON_OFF_PLUGIN_UNIT_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num)  \
      ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                                    \
      ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)  simple_desc_##ep_name =            \
      {                                                                                       \
        ep_id,                                                                                \
        ZB_AF_ZLL_PROFILE_ID,                                                                 \
        ZB_ZLL_ON_OFF_PLUGIN_UNIT_DEVICE_ID,                                                  \
        ZB_ZLL_DEVICE_VER_ON_OFF_PLUGIN_UNIT,                                                 \
        0,                                                                                    \
        in_clust_num,                                                                         \
        out_clust_num,                                                                        \
        {                                                                                     \
          ZB_ZCL_CLUSTER_ID_BASIC,                                                            \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                         \
          ZB_ZCL_CLUSTER_ID_ON_OFF,                                                           \
          ZB_ZCL_CLUSTER_ID_GROUPS,                                                           \
          ZB_ZCL_CLUSTER_ID_SCENES,                                                           \
          ZB_ZLL_CLUSTER_ID_COMMISSIONING                                                     \
        }                                                                                     \
      }

/**
 *  @brief Declare endpoint for dimmable light device.
 *  @param ep_name [IN] - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param cluster_list [IN] - endpoint cluster list.
 */
#define ZB_ZLL_DECLARE_ON_OFF_PLUGIN_UNIT_EP(ep_name, ep_id, cluster_list)    \
      ZB_ZCL_DECLARE_DEVICE_SCENE_TABLE_RECORD_TYPE_ZLL(                      \
          zb_zll_on_off_light_scene_table_ ## ep_name ## _t,                  \
          ZB_ZCL_SCENES_FIELDSETS_LENGTH(ZB_ZCL_CLUSTER_ID_ON_OFF));          \
      ZB_ZCL_DEFINE_DEVICE_SCENE_TABLE_ZLL(                                   \
          zb_zll_on_off_light_scene_table_ ## ep_name ## _t,                  \
          g_zb_zll_on_off_light_scene_table_ ## ep_name);                     \
      ZB_ZCL_DECLARE_ON_OFF_PLUGIN_UNIT_SIMPLE_DESC(                          \
          ep_name,                                                            \
          ep_id,                                                              \
          ZB_ZLL_ON_OFF_PLUGIN_UNIT_IN_CLUSTER_NUM,                           \
          ZB_ZLL_ON_OFF_PLUGIN_UNIT_OUT_CLUSTER_NUM);                         \
      ZB_AF_START_DECLARE_ENDPOINT_LIST(ep_name)                              \
        ZB_AF_SET_ENDPOINT_DESC(                                              \
            ep_id,                                                            \
            ZB_AF_ZLL_PROFILE_ID,                                             \
            sizeof(zb_zll_on_off_light_scene_table_ ## ep_name ## _t),        \
            &g_zb_zll_on_off_light_scene_table_ ## ep_name,                   \
            ZB_ZCL_ARRAY_SIZE(                                                \
                cluster_list,                                                 \
                zb_zcl_cluster_desc_t),                                       \
            cluster_list,                                                     \
            (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name)                 \
      ZB_AF_FINISH_DECLARE_ENDPOINT_LIST

#define ZB_ZLL_DECLARE_ON_OFF_PLUGIN_UNIT_CTX(device_ctx, ep_name)                                        \
  ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info## device_ctx_name,                                    \
                                     ZB_ZLL_ON_OFF_PLUGIN_UNIT_REPORT_ATTR_COUNT);                        \
  ZBOSS_DECLARE_DEVICE_CTX(device_ctx, ep_name,                                                           \
                           ZB_ZCL_ARRAY_SIZE(ep_name, zb_af_endpoint_desc_t),                             \
                           ZB_ZLL_ON_OFF_PLUGIN_UNIT_REPORT_ATTR_COUNT, reporting_info## device_ctx_name, \
                           0, NULL)

#endif /* defined ZB_ZLL_DEFINE_DEVICE_ON_OFF_PLUGIN_UNIT || defined DOXYGEN */

/**
 *  @}
 */

#endif /* ! defined ZB_ZLL_ON_OFF_PLUGIN_UNIT_H */
