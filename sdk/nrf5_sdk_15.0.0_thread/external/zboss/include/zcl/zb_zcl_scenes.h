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
 * PURPOSE: Scenes cluster definitoins
*/

#ifndef ZB_ZCL_SCENES_H
#define ZB_ZCL_SCENES_H 1

/** @cond DOXYGEN_ZCL_SECTION */

/*! @defgroup ZB_ZCL_SCENES ZCL Scenes cluster
    @ingroup ZB_ZCL_CLUSTERS
    @addtogroup ZB_ZCL_SCENES
    ZCL Scenes cluster definitions
    @{
*/

/* ZB_ZCL_CLUSTER_ID_SCENES = 0x0005 */

/*! @name Scenes cluster attributes
    @{
*/

/*! @brief Scene cluster attribute identifiers
    @see ZCL specification, subclause  3.7.2.2
 */
enum zb_zcl_scene_attr_e
{
  /*! Number of scenes currently in the device's scene table */
  ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID   = 0x0000,
  /*! Scene ID of the scene last invoked */
  ZB_ZCL_ATTR_SCENES_CURRENT_SCENE_ID = 0x001,
  /*! @brief Group ID of the scene last invoked

      Can hold 0x0000 if the scene last invoked is not associated with a group
  */
  ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_ID = 0x002,
  /*! @brief Indicates whether the state of the device corresponds to
      that associated with the CurrentScene and CurrentGroup attributes
      @li TRUE indicates that these attributes are valid
      @li FALSE indicates that they are not valid
  */
  ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID   = 0x003,
  /* TODO Remove default value for Name support attribute when scene name
     support will be provided
  */
  /*! @brief Indicates support for scene names

      The most significant bit of the NameSupport attribute indicates whether or
      not scene names are supported
      @li 1 indicates that they are supported
      @li 0 indicates that they are not supported
      @note Currently scene names are not supported
  */
  ZB_ZCL_ATTR_SCENES_NAME_SUPPORT_ID  = 0x004,
  /*! @brief specifies the IEEE address of the device that last configured the
      scene table

      The value 0xffffffffffffffff indicates that the device has not been
      configured, or that the address of the device that last configured the
      scenes cluster is not known
      @attention The attribute is not supported!
  */
  ZB_ZCL_ATTR_SCENES_LAST_CONFIGURED_BY_ID = 0x005
};

/** @brief Mask to extract Name support bit */
#define ZB_ZCL_SCENES_NAME_SUPPORT_BIT_MASK 0x80

/*! Maximal permissible value for Current group attribute */
#define ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MAX_VALUE 0xfff7
/*! Maximal number of scene table records */
#define ZB_ZCL_MAX_SCENE_TABLE_RECORDS 16

/** @brief Default value for Scene count attribute */
#define ZB_ZCL_SCENES_SCENE_COUNT_DEFAULT_VALUE 0

/** @brief Default value for Current scene attribute */
#define ZB_ZCL_SCENES_CURRENT_SCENE_DEFAULT_VALUE 0

/** @brief Default value for Current group attribute */
#define ZB_ZCL_SCENES_CURRENT_GROUP_DEFAULT_VALUE 0

/** @brief Default value for Scene valid attribute */
#define ZB_ZCL_SCENES_SCENE_VALID_DEFAULT_VALUE 0

/*
 * TODO: Remove default value for Scenes.NameSupport attribute when scene names
 * will be supported.
 */
/** @brief Default value for Name support attribute

    Actually Name support attribute has no default value (see ZCL spec,
    subclause 3.7.2.2.1.5), but we don't support scene names, so we are able to
    declare default value for the attribute until name support will be added
*/
#define ZB_ZCL_SCENES_NAME_SUPPORT_DEFAULT_VALUE 0

#if defined ZB_ENABLE_ZLL || defined DOXYGEN

/** @brief Global Scene - group identifier
    see ZLL spec 6.5.1.3.1*/
#define ZB_ZCL_SCENE_GLOBAL_SCENE_GROUP_ID 0x00

/** @brief Global Scene - scene identifier
    see ZLL spec 6.5.1.3.1*/
#define ZB_ZCL_SCENE_GLOBAL_SCENE_ID 0x00

/** @brief Store scene
 * @param endpoint - endpoint
 * @param group_id - scene group ID
 * @param scene_id - scene ID
 * @return status store scene
 * */
zb_zcl_status_t zb_zcl_scenes_process_store_scene(zb_uint8_t endpoint, zb_uint8_t group_id, zb_uint8_t scene_id);

/** @brief Recall Global scene
 * @param param - buffer for recall scene
 * @param endpoint - enpoint scene for recall
 * @result operation status */
zb_ret_t zb_zcl_scenes_recall_global_scene(zb_uint8_t param, zb_uint8_t endpoint);

#endif /*defined ZB_ENABLE_ZLL || defined DOXYGEN*/


/*! @brief Fixed-size part of the Scene table */
typedef ZB_PACKED_PRE struct zb_zcl_scene_table_record_fixed_s
{
  /*! @brief Group identifier for the scene

      Can be zero if the scene is not associated with any group @attention Since
      value of 0xffff is not appropriate as a group identifier, it is being used
      as a free record marker in a form of @ref ZB_ZCL_SCENES_FREE_SCENE_TABLE_RECORD
      macro
  */
  zb_uint16_t group_id;
  /*! @brief Scene transition time */
  zb_uint16_t transition_time;
  /*! @brief Scene identifier */
  zb_uint8_t scene_id;
  /*! @brief Field set length

      Actual length of the field sets. While we know maximal length of the field
      sets, we also need to know its actual length in order to finish reading at
      a proper point. E. g.: as stated in ZCL spec, subclause 3.7.2.4.1.1, it is
      not mandatory for a field set to be included in the command for every
      cluster on that endpoint that has a defined field set. With this field
      filled we know actual length of the field sets thus being able to read
      only the info we need.
  */
  zb_uint8_t field_sets_length;
} ZB_PACKED_STRUCT zb_zcl_scene_table_record_fixed_t;

#if defined DOXYGEN
/* TODO fill in macro names for fieldsets length evaluation, filling and parsing
   filedsets. */
/*! @brief Typical Scene table record structure

    For each endpoint corresponding type must be declared with ... macro.
    @note Scene names are optional and so are not supported.
*/
typedef ZB_PACKED_PRE struct zb_zcl_scene_table_record_type_s
{
  /*! Fixed-size part of the scene table */
  zb_zcl_scene_table_record_fixed_t common;
  /*! @brief Scene field sets

      Field sets are individual for each device, their actual size can be
      evaluated with ... macro. Field sets must be filled and parsed with ...
      and ... functions.
  */
  zb_uint8_t field_sets[FIELD_SETS_LENGTH];
} ZB_PACKED_STRUCT zb_zcl_scene_table_record_type_t;
#endif /* defined DOXYGEN */

/* TODO Also adjust documentation entry surrounded with "defined DOXYGEN"
   conditional */
/*! @brief Declares Scene table record type
    @param type_name name of the type for scene table record
    @param FIELD_SETS_LENGTH length of the field sets for this particular
    device. Can be evaluated with ... macro.
    @note For detail information on fields see @ref
    zb_zcl_scene_table_record_type_s
    @attention Scene table record structure will have a tag of the form
    type_name with "_s" appended.
*/
#define ZB_ZCL_DECLARE_DEVICE_SCENE_TABLE_RECORD_TYPE(  \
    type_name,                                          \
    FIELD_SETS_LENGTH)                                  \
typedef ZB_PACKED_PRE struct type_name ## _s            \
{                                                       \
  zb_zcl_scene_table_record_fixed_t common;             \
  zb_uint8_t field_sets[FIELD_SETS_LENGTH];             \
} ZB_PACKED_STRUCT type_name

/*! @brief Evaluates Scene table record size
    @param FIELD_SETS_LENGTH length of the fieldsets (variable part of the
    record)
*/
#define ZB_ZCL_SCENES_TABLE_RECORD_SIZE(FIELD_SETS_LENGTH)        \
  (FIELD_SETS_LENGTH + sizeof(zb_zcl_scene_table_record_fixed_t))

/*! Defines Scene table
    @param type_name Scene table record type name
    @param table_name Scene table variable name
*/
#define ZB_ZCL_DEFINE_DEVICE_SCENE_TABLE(type_name, table_name) \
type_name table_name[ZB_ZCL_MAX_SCENE_TABLE_RECORDS]

/*! @brief Declares a "free record" marker for a scene table

   Used as a value of a group_id field (see @ref
   zb_zcl_scene_table_record_type_s) in a corresponding table record
*/
#define ZB_ZCL_SCENES_FREE_SCENE_TABLE_RECORD 0xffff

/** @brief Default value for scene transition time */
#define ZB_ZCL_SCENES_TRANSITION_TIME_DEFAULT_VALUE 0

/*! @brief Marks all records in a table as free

    @param table_name Name of the scene table variable
*/
#define ZB_ZCL_SCENES_CLEAR_SCENE_TABLE(table_name)       \
{                                                         \
  zb_uint8_t scene_table_idx;                             \
  for ( scene_table_idx = 0;                              \
        scene_table_idx < ZB_ZCL_MAX_SCENE_TABLE_RECORDS; \
        ++scene_table_idx)                                \
  {                                                       \
    table_name[scene_table_idx].common.group_id =         \
        ZB_ZCL_SCENES_FREE_SCENE_TABLE_RECORD;            \
  }                                                       \
}

/*! @brief Marks all records in a table as free
    @param endpoint_desc - pointer to the description of the endpoint (of type @ref
    zb_af_endpoint_desc_s) a scene table belongs to
    @param remove_zero_group - boolean flag indicating we need to clean scenes for groups with zero
    group identifier too.
    @param status of the operation
*/
#define ZB_ZCL_SCENES_CLEAR_ENDPOINT_SCENE_TABLE(endpoint_desc, remove_zero_group, status)       \
{                                                                                                \
  zb_uint8_t scene_table_idx;                                                                    \
  zb_zcl_scene_table_record_fixed_t * scene_table;                                               \
  if (! (   (endpoint_desc)                                                                      \
         && (endpoint_desc)->scene_table_record_size                                             \
         && (endpoint_desc)->scene_table))                                                       \
  {                                                                                              \
    status = ZB_ZCL_STATUS_INVALID_FIELD;                                                        \
  }                                                                                              \
  else                                                                                           \
  {                                                                                              \
    scene_table =                                                                                \
        (zb_zcl_scene_table_record_fixed_t*)((endpoint_desc)->scene_table);                      \
    for ( scene_table_idx = 0;                                                                   \
          scene_table_idx < ZB_ZCL_MAX_SCENE_TABLE_RECORDS;                                      \
          ++scene_table_idx)                                                                     \
    {                                                                                            \
      if ((remove_zero_group) || scene_table->group_id)                                          \
      {                                                                                          \
        scene_table->group_id = ZB_ZCL_SCENES_FREE_SCENE_TABLE_RECORD;                           \
      }                                                                                          \
      scene_table = (zb_zcl_scene_table_record_fixed_t*)                                         \
                      ((zb_uint8_t*)scene_table + (endpoint_desc)->scene_table_record_size);     \
    }                                                                                            \
  }                                                                                              \
}

/*! @brief Sets endpoint scene table information
    @param endpoint scene table belongs to
    @param record_size - scene table record size for the endpoint
    @param table - pointer to the scen table in the userspace
*/
#define ZB_AF_SET_ENDPOINT_SCENE_TABLE_INFO(endpoint, record_size, table) \
{                                                                         \
  zb_af_endpoint_desc_t* ep = zb_af_get_endpoint_desc(endpoint);          \
  ep->scene_table_record_size = record_size;                              \
  ep->scene_table = table;                                                \
}

/*! @brief Common part of the field set definition */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_fieldset_common_s
{
  zb_uint16_t cluster_id;     /*!< Cluster identifier */
  zb_uint8_t fieldset_length; /*!< Length of the field set's variable part */
} ZB_PACKED_STRUCT zb_zcl_scenes_fieldset_common_t;

/*! @brief Declares Scenes attribute list
    @param attr_list Attribute list name
    @param scene_count @ref ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID placeholder
    @param current_scene @ref ZB_ZCL_ATTR_SCENES_CURRENT_SCENE_ID placeholder
    @param current_group @ref ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_ID placeholder
    @param scene_valid @ref ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID placeholder
    @param name_support @ref ZB_ZCL_ATTR_SCENES_NAME_SUPPORT_ID placeholder
*/
#define ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST(                                    \
    attr_list,                                                                \
    scene_count,                                                              \
    current_scene,                                                            \
    current_group,                                                            \
    scene_valid,                                                              \
    name_support)                                                             \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID, (scene_count))      \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SCENES_CURRENT_SCENE_ID, (current_scene))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_ID, (current_group))  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID, (scene_valid))      \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SCENES_NAME_SUPPORT_ID, (name_support))    \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*! @brief Evaluates length if a fieldset for a particular cluster

    CLUSTER_SYMBOL is a symbolic name for a cluster identifier, see literals
    in @ref zb_zcl_cluster_id_e
    @note it is strongly recommended to use the macro only with clusters
    defining Scenes extensions
*/
#define ZB_ZCL_SCENES_FIELDSETS_LENGTH(CLUSTER_SYMBOL) \
  (CLUSTER_SYMBOL ## _SCENE_FIELD_SETS_LENGTH)

/*! @copydoc ZB_ZCL_SCENES_FIELDSETS_LENGTH */
#define ZB_ZCL_SCENES_FIELDSETS_LENGTH_2(CLUSTER_SYMBOL1, CLUSTER_SYMBOL2)  \
  (ZB_ZCL_SCENES_FIELDSETS_LENGTH(CLUSTER_SYMBOL1) +                        \
   ZB_ZCL_SCENES_FIELDSETS_LENGTH(CLUSTER_SYMBOL2))

/*! @copydoc ZB_ZCL_SCENES_FIELDSETS_LENGTH */
#define ZB_ZCL_SCENES_FIELDSETS_LENGTH_3(             \
    CLUSTER_SYMBOL1,                                  \
    CLUSTER_SYMBOL2,                                  \
    CLUSTER_SYMBOL3)                                  \
  (ZB_ZCL_SCENES_FIELDSETS_LENGTH(CLUSTER_SYMBOL1) +  \
   ZB_ZCL_SCENES_FIELDSETS_LENGTH(CLUSTER_SYMBOL2) +  \
   ZB_ZCL_SCENES_FIELDSETS_LENGTH(CLUSTER_SYMBOL3))

/*! @copydoc ZB_ZCL_SCENES_FIELDSETS_LENGTH */
#define ZB_ZCL_SCENES_FIELDSETS_LENGTH_4(                               \
    CLUSTER_SYMBOL1,                                                    \
    CLUSTER_SYMBOL2,                                                    \
    CLUSTER_SYMBOL3,                                                    \
    CLUSTER_SYMBOL4)                                                    \
  (ZB_ZCL_SCENES_FIELDSETS_LENGTH_2(CLUSTER_SYMBOL1, CLUSTER_SYMBOL2) + \
   ZB_ZCL_SCENES_FIELDSETS_LENGTH_2(CLUSTER_SYMBOL3, CLUSTER_SYMBOL4))

/*! @copydoc ZB_ZCL_SCENES_FIELDSETS_LENGTH */
#define ZB_ZCL_SCENES_FIELDSETS_LENGTH_5(                               \
    CLUSTER_SYMBOL1,                                                    \
    CLUSTER_SYMBOL2,                                                    \
    CLUSTER_SYMBOL3,                                                    \
    CLUSTER_SYMBOL4,                                                    \
    CLUSTER_SYMBOL5)                                                    \
  (ZB_ZCL_SCENES_FIELDSETS_LENGTH_2(CLUSTER_SYMBOL1, CLUSTER_SYMBOL2) + \
   ZB_ZCL_SCENES_FIELDSETS_LENGTH_2(CLUSTER_SYMBOL3, CLUSTER_SYMBOL4) + \
   ZB_ZCL_SCENES_FIELDSETS_LENGTH(CLUSTER_SYMBOL5))

/** @brief Removes All scenes corresponding to a group identifiers in a whole device.
    @param group_id - group identifier for which all scenes must be removed.
*/
zb_void_t zb_zcl_scenes_remove_scenes_in_all_endpoints(zb_uint16_t group_id);

/*! @} */ /* Scenes cluster attributes */

/*! @name Scenes cluster internals
    Internal structures for Scenes cluster
    @internal
    @{
*/

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID(data_ptr) \
{                                                                          \
  ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                     \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                            \
  (zb_voidp_t) data_ptr                                                    \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SCENES_CURRENT_SCENE_ID(data_ptr) \
{                                                                            \
  ZB_ZCL_ATTR_SCENES_CURRENT_SCENE_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U8,                                                       \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                              \
  (zb_voidp_t) data_ptr                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_ID(data_ptr) \
{                                                                            \
  ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_ID,                                       \
  ZB_ZCL_ATTR_TYPE_U16,                                                      \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                              \
  (zb_voidp_t) data_ptr                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID(data_ptr) \
{                                                                          \
  ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID,                                       \
  ZB_ZCL_ATTR_TYPE_BOOL,                                                   \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                            \
  (zb_voidp_t) data_ptr                                                    \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SCENES_NAME_SUPPORT_ID(data_ptr) \
{                                                                           \
  ZB_ZCL_ATTR_SCENES_NAME_SUPPORT_ID,                                       \
  ZB_ZCL_ATTR_TYPE_8BITMAP,                                                 \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                             \
  (zb_voidp_t) data_ptr                                                     \
}

#if 0 /* Currently not supported (optional) */
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SCENES_LAST_CONFIGURED_BY_ID(  \
    data_ptr)                                                             \
{                                                                         \
  ZB_ZCL_ATTR_SCENES_LAST_CONFIGURED_BY_ID,                               \
  ZB_ZCL_ATTR_TYPE_IEEE_ADDR,                                             \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                           \
  (zb_voidp_t) data_ptr                                                   \
}
#endif /* 0 */

/*! Scenes fieldset length for Scenes cluster */
#define ZB_ZCL_CLUSTER_ID_SCENES_SCENE_FIELD_SETS_LENGTH 0

/*! @brief Scenes fieldsets representation structure for On/Off cluster-specific
    content
*/
typedef ZB_PACKED_PRE struct zb_zcl_scenes_on_off_fieldset_content_s
{
  zb_uint8_t on_off;  /**< On/Off atrribute value */
} ZB_PACKED_STRUCT zb_zcl_scenes_on_off_fieldset_content_t;

#if defined ZB_ZCL_SUPPORT_ON_OFF_SCENE_EXTENSIONS || defined DOXYGEN

/** @internal @brief Scenes fieldsets length for On/Off cluster */
#define ZB_ZCL_CLUSTER_ID_ON_OFF_SCENE_FIELD_SETS_LENGTH  \
  (sizeof(zb_zcl_scenes_fieldset_on_off_t))

/** @brief Scenes fieldsets representation structure for On/Off cluster */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_fieldset_on_off_s
{
  /** @brief Common part of fieldset definition */
  zb_zcl_scenes_fieldset_common_t common;
  /** @brief Fields specific to On/Off cluster */
  zb_zcl_scenes_on_off_fieldset_content_t specific;
} ZB_PACKED_STRUCT zb_zcl_scenes_fieldset_on_off_t;

/** @internal @brief Stores fieldset for On/Off cluster into Scene table
    @param endpoint to read attribute values from
    @param data_ptr - pointer to the place to put fieldset to. Will be
    incremented by total size of the fieldset on success
    @param max_data_size - maximum amount of data to store
    @param field_sets_length - length of the fieldsets already stored. Will be
    incremented after successfully storing the fieldset.
    @param status of the operation
*/
#define ZB_ZCL_SCENES_STORE_ON_OFF_FIELDSET(                                   \
    endpoint,                                                                  \
    data_ptr,                                                                  \
    max_data_size,                                                             \
    field_sets_length,                                                         \
    status)                                                                    \
  {                                                                            \
    if (( field_sets_length +                                                  \
          ZB_ZCL_CLUSTER_ID_ON_OFF_SCENE_FIELD_SETS_LENGTH) > max_data_size)   \
    {                                                                          \
      status = ZB_ZCL_STATUS_INSUFF_SPACE;                                     \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      zb_zcl_attr_t* attr_desc = zb_zcl_get_attr_desc_a(                       \
                                  endpoint,                                    \
                                  ZB_ZCL_CLUSTER_ID_ON_OFF,                    \
                                  ZB_ZCL_CLUSTER_SERVER_ROLE,           \
                                  ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);               \
      if (!attr_desc)                                                          \
      {                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                  \
      }                                                                        \
      else                                                                     \
      {                                                                        \
        ((zb_zcl_scenes_fieldset_on_off_t*)data_ptr)->common.cluster_id =      \
            ZB_ZCL_CLUSTER_ID_ON_OFF;                                          \
        ((zb_zcl_scenes_fieldset_on_off_t*)data_ptr)->common.fieldset_length = \
            sizeof(zb_zcl_scenes_on_off_fieldset_content_t);                   \
        ((zb_zcl_scenes_fieldset_on_off_t*)data_ptr)->specific.on_off =        \
            *(zb_uint8_t*)(attr_desc->data_p);                                 \
        field_sets_length += ZB_ZCL_CLUSTER_ID_ON_OFF_SCENE_FIELD_SETS_LENGTH; \
        data_ptr += ZB_ZCL_CLUSTER_ID_ON_OFF_SCENE_FIELD_SETS_LENGTH;          \
        status = ZB_ZCL_STATUS_SUCCESS;                                        \
      }                                                                        \
    }                                                                          \
  }

#else /* defined ZB_ZCL_SUPPORT_ON_OFF_SCENE_EXTENSIONS */
#define ZB_ZCL_CLUSTER_ID_ON_OFF_SCENE_FIELD_SETS_LENGTH  0
#endif /* defined ZB_ZCL_SUPPORT_ON_OFF_SCENE_EXTENSIONS */

/*! @brief Scenes fieldsets representation structure for Level control
    cluster-specific content
*/
typedef ZB_PACKED_PRE struct zb_zcl_scenes_level_control_fieldset_content_s
{
  zb_uint8_t current_level;  /**< Current level atrribute value */
} ZB_PACKED_STRUCT zb_zcl_scenes_level_control_fieldset_content_t;

#if defined ZB_ZCL_SUPPORT_LEVEL_CONTROL_SCENE_EXTENSIONS || defined DOXYGEN
/** @internal @brief Scenes fieldsets length for Level control cluster */
#define ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL_SCENE_FIELD_SETS_LENGTH \
  (sizeof(zb_zcl_scenes_fieldset_level_control_t))

/** @brief Scenes fieldsets representation structure for LevelControl cluster */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_fieldset_level_control_s
{
  /** @brief Common part of fieldset definition */
  zb_zcl_scenes_fieldset_common_t common;
  /** @brief Fields specific to Level control cluster */
  zb_zcl_scenes_level_control_fieldset_content_t specific;
} ZB_PACKED_STRUCT zb_zcl_scenes_fieldset_level_control_t;

/** @internal @brief Stores fieldset for Level Control cluster into Scene table
    @param endpoint to read attribute values from
    @param data_ptr - pointer to the place to put fieldset to. Will be
    incremented by total size of the fieldset on success
    @param max_data_size - maximum amount of data to store
    @param field_sets_length - length of the fieldsets already stored. Will be
    incremented after successfully storing the fieldset.
    @param status of the operation
*/
#define ZB_ZCL_SCENES_STORE_LEVEL_CONTROL_FIELDSET(                                        \
    endpoint,                                                                              \
    data_ptr,                                                                              \
    max_data_size,                                                                         \
    field_sets_length,                                                                     \
    status)                                                                                \
  {                                                                                        \
    if (  (   field_sets_length                                                            \
           +  ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL_SCENE_FIELD_SETS_LENGTH)                     \
        > max_data_size)                                                                   \
    {                                                                                      \
      status = ZB_ZCL_STATUS_INSUFF_SPACE;                                                 \
    }                                                                                      \
    else                                                                                   \
    {                                                                                      \
      zb_zcl_attr_t* attr_desc = zb_zcl_get_attr_desc_a(                                   \
                                endpoint,                                                  \
                                ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,                           \
                                ZB_ZCL_CLUSTER_SERVER_ROLE,             \
                                ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID);               \
      if (!attr_desc)                                                                      \
      {                                                                                    \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                              \
      }                                                                                    \
      else                                                                                 \
      {                                                                                    \
        ((zb_zcl_scenes_fieldset_level_control_t*)data_ptr)->                              \
            common.cluster_id = ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL;                           \
        ((zb_zcl_scenes_fieldset_level_control_t*)data_ptr)->                              \
          common.fieldset_length = sizeof(zb_zcl_scenes_level_control_fieldset_content_t); \
        ((zb_zcl_scenes_fieldset_level_control_t*)data_ptr)->                              \
            specific.current_level = *(zb_uint8_t*)(attr_desc->data_p);                    \
        field_sets_length +=                                                               \
            ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL_SCENE_FIELD_SETS_LENGTH;                       \
        data_ptr += ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL_SCENE_FIELD_SETS_LENGTH;               \
        status = ZB_ZCL_STATUS_SUCCESS;                                                    \
      }                                                                                    \
    }                                                                                      \
  }
#else /*    defined ZB_ZCL_SUPPORT_LEVEL_CONTROL_SCENE_EXTENSIONS
         || defined DOXYGEN */
#define ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL_SCENE_FIELD_SETS_LENGTH 0
#endif /*   defined ZB_ZCL_SUPPORT_LEVEL_CONTROL_SCENE_EXTENSIONS
         || defined DOXYGEN  */

/*! @brief Scenes fieldsets representation structure for Color control
    cluster-specific content
*/
typedef ZB_PACKED_PRE struct zb_zcl_scenes_color_control_fieldset_content_s
{
  zb_uint16_t current_x;  /**< CurrentX attribute */
  zb_uint16_t current_y;  /**< CurrentY attribute */

#if defined ZB_ENABLE_ZLL
  zb_uint16_t enhanced_current_hue;    /**< Enhanced Current Hue attribute */
#endif /*defined ZB_ENABLE_ZLL */

  zb_uint8_t current_saturation;       /**<Current Saturation attribute */

#if defined ZB_ENABLE_ZLL
  zb_uint8_t color_loop_active;        /**< Color Loop Active attribute */
  zb_uint8_t color_loop_direction;     /**< Color Loop Direction attribute */
  zb_uint16_t color_loop_time;          /**< Color Loop Time attribute */
#endif /*defined ZB_ENABLE_ZLL */

} ZB_PACKED_STRUCT zb_zcl_scenes_color_control_fieldset_content_t;

/** @brief Scenes fieldsets representation structure for ColorControl cluster */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_fieldset_color_control_s
{
  /** @brief Common part of fieldset definition */
  zb_zcl_scenes_fieldset_common_t common;
  /** @brief Fields specific to Color control cluster */
  zb_zcl_scenes_color_control_fieldset_content_t specific;
} ZB_PACKED_STRUCT zb_zcl_scenes_fieldset_color_control_t;

#if defined ZB_ZCL_SUPPORT_COLOR_CONTROL_SCENE_EXTENSIONS || defined DOXYGEN
/** @internal @brief Scenes fieldsets length for Color control cluster */
#define ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SCENE_FIELD_SETS_LENGTH \
  (sizeof(zb_zcl_scenes_fieldset_color_control_t))

/** @internal @brief Stores fieldset for Color Control cluster into Scene table
    @param endpoint to read attribute values from
    @param data_ptr - pointer to the place to put fieldset to. Will be
    incremented by total size of the fieldset on success
    @param max_data_size - maximum amount of data to store
    @param field_sets_length - length of the fieldsets already stored. Will be
    incremented after successfully storing the fieldset.
    @param status of the operation
*/
#if defined ZB_ENABLE_ZLL

#define ZB_ZCL_SCENES_STORE_COLOR_CONTROL_FIELDSET(                                            \
    endpoint,                                                                                  \
    data_ptr,                                                                                  \
    max_data_size,                                                                             \
    field_sets_length,                                                                         \
    status)                                                                                    \
  {                                                                                            \
    zb_zcl_attr_t* attr_desc;                                                                  \
                                                                                               \
    (status) =  (  (   field_sets_length                                                       \
           +  ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SCENE_FIELD_SETS_LENGTH)                         \
        > max_data_size) ?                                                                     \
              ZB_ZCL_STATUS_INSUFF_SPACE : ZB_ZCL_STATUS_SUCCESS;                              \
                                                                                               \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                                  ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID);                     \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.current_x = *(zb_uint16_t*)(attr_desc->data_p);                           \
      }                                                                                        \
    }                                                                                          \
                                                                                               \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                                         ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID); \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.current_y = *(zb_uint16_t*)(attr_desc->data_p);                           \
      }                                                                                        \
    }                                                                                          \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                  ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ZLL_ID);                      \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.enhanced_current_hue = *(zb_uint16_t*)(attr_desc->data_p);                \
      }                                                                                        \
    }                                                                                          \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                                         ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID); \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.current_saturation = *(zb_uint8_t*)(attr_desc->data_p);                   \
      }                                                                                        \
    }                                                                                          \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                                         ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ZLL_ID); \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.color_loop_active = *(zb_uint8_t*)(attr_desc->data_p);                    \
      }                                                                                        \
    }                                                                                          \
                                                                                               \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                                         ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ZLL_ID); \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.color_loop_direction = *(zb_uint8_t*)(attr_desc->data_p);                 \
      }                                                                                        \
    }                                                                                          \
                                                                                               \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                     ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_TIME_ZLL_ID);                        \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.color_loop_time = *(zb_uint16_t*)(attr_desc->data_p);                     \
      }                                                                                        \
    }                                                                                          \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                    \
          common.cluster_id = ZB_ZCL_CLUSTER_ID_COLOR_CONTROL;                                 \
      ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                    \
          common.fieldset_length = sizeof(zb_zcl_scenes_color_control_fieldset_content_t);     \
      field_sets_length += ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SCENE_FIELD_SETS_LENGTH;            \
      data_ptr += ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SCENE_FIELD_SETS_LENGTH;                     \
    }                                                                                          \
  }
#else

#define ZB_ZCL_SCENES_STORE_COLOR_CONTROL_FIELDSET(                                            \
    endpoint,                                                                                  \
    data_ptr,                                                                                  \
    max_data_size,                                                                             \
    field_sets_length,                                                                         \
    status)                                                                                    \
  {                                                                                            \
    zb_zcl_attr_t* attr_desc;                                                                  \
                                                                                               \
    (status) =  (  (   field_sets_length                                                       \
           +  ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SCENE_FIELD_SETS_LENGTH)                         \
        > max_data_size) ?                                                                     \
              ZB_ZCL_STATUS_INSUFF_SPACE : ZB_ZCL_STATUS_SUCCESS;                              \
                                                                                               \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                                         ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID); \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.current_x = *(zb_uint16_t*)(attr_desc->data_p);                           \
      }                                                                                        \
    }                                                                                          \
                                                                                               \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                                         ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID); \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.current_y = *(zb_uint16_t*)(attr_desc->data_p);                           \
      }                                                                                        \
    }                                                                                          \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,             \
                                         ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID); \
      if (!attr_desc)                                                                          \
      {                                                                                        \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                  \
      }                                                                                        \
      else                                                                                     \
      {                                                                                        \
        ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                  \
            specific.current_saturation = *(attr_desc->data_p);                                \
      }                                                                                        \
    }                                                                                          \
    if( (status)== ZB_ZCL_STATUS_SUCCESS)                                                      \
    {                                                                                          \
      ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                    \
          common.cluster_id = ZB_ZCL_CLUSTER_ID_COLOR_CONTROL;                                 \
      ((zb_zcl_scenes_fieldset_color_control_t*)data_ptr)->                                    \
          common.fieldset_length = sizeof(zb_zcl_scenes_color_control_fieldset_content_t);     \
      field_sets_length += ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SCENE_FIELD_SETS_LENGTH;            \
      data_ptr += ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SCENE_FIELD_SETS_LENGTH;                     \
    }                                                                                          \
  }
#endif

#else /*      defined ZB_ZCL_SUPPORT_COLOR_CONTROL_SCENE_EXTENSIONS
          ||  defined DOXYGEN */
#define ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_SCENE_FIELD_SETS_LENGTH 0
#endif /*     defined ZB_ZCL_SUPPORT_COLOR_CONTROL_SCENE_EXTENSIONS
          ||  defined DOXYGEN */

/*! @brief Scenes fieldsets representation structure for Door lock
    cluster-specific content
*/
typedef ZB_PACKED_PRE struct zb_zcl_scenes_door_lock_fieldset_content_s
{
  zb_uint8_t lock_state;  /**< Lock state attribute */
} ZB_PACKED_STRUCT zb_zcl_scenes_door_lock_fieldset_content_t;

#if defined ZB_ZCL_SUPPORT_DOOR_LOCK_SCENE_EXTENSIONS || defined DOXYGEN
/** @internal @brief Scenes fieldsets length for Color control cluster */
#define ZB_ZCL_CLUSTER_ID_DOOR_LOCK_SCENE_FIELD_SETS_LENGTH \
  (sizeof(zb_zcl_scenes_fieldset_door_lock_t))

/** @brief Scenes fieldsets representation structure for Door lock cluster */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_fieldset_door_lock_s
{
  /** @brief Common part of fieldset definition */
  zb_zcl_scenes_fieldset_common_t common;
  /** @brief Fields specific to Door lock cluster */
  zb_zcl_scenes_door_lock_fieldset_content_t specific;
} ZB_PACKED_STRUCT zb_zcl_scenes_fieldset_door_lock_t;

/** @internal @brief Stores fieldset for Door lock cluster into Scene table
    @param endpoint to read attribute values from
    @param data_ptr - pointer to the place to put fieldset to. Will be
    incremented by total size of the fieldset on success
    @param max_data_size - maximum amount of data to store
    @param field_sets_length - length of the fieldsets already stored. Will be
    incremented after successfully storing the fieldset.
    @param status of the operation
*/
#define ZB_ZCL_SCENES_STORE_DOOR_LOCK_FIELDSET(                                      \
    endpoint,                                                                        \
    data_ptr,                                                                        \
    max_data_size,                                                                   \
    field_sets_length,                                                               \
    status)                                                                          \
  {                                                                                  \
    if (  (   field_sets_length                                                      \
           +  ZB_ZCL_CLUSTER_ID_DOOR_LOCK_SCENE_FIELD_SETS_LENGTH)                   \
        > max_data_size)                                                             \
    {                                                                                \
      status = ZB_ZCL_STATUS_INSUFF_SPACE;                                           \
    }                                                                                \
    else                                                                             \
    {                                                                                \
      zb_zcl_attr_t* attr_desc = zb_zcl_get_attr_desc_a(                             \
                                  endpoint,                                          \
                                  ZB_ZCL_CLUSTER_ID_DOOR_LOCK,                       \
                                  ZB_ZCL_CLUSTER_SERVER_ROLE,           \
                                  ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID); \
      if (!attr_desc)                                                                \
      {                                                                              \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                        \
      }                                                                              \
      else                                                                           \
      {                                                                              \
        ((zb_zcl_scenes_fieldset_door_lock_t*)data_ptr)->                            \
        common.cluster_id = ZB_ZCL_CLUSTER_ID_DOOR_LOCK;                             \
        ((zb_zcl_scenes_fieldset_door_lock_t*)data_ptr)->                            \
        common.fieldset_length = sizeof(zb_zcl_scenes_door_lock_fieldset_content_t); \
        ((zb_zcl_scenes_fieldset_door_lock_t*)data_ptr)->                            \
        specific.lock_state = *(zb_uint8_t*)(attr_desc->data_p);                     \
        field_sets_length +=                                                         \
            ZB_ZCL_CLUSTER_ID_DOOR_LOCK_SCENE_FIELD_SETS_LENGTH;                     \
        data_ptr += ZB_ZCL_CLUSTER_ID_DOOR_LOCK_SCENE_FIELD_SETS_LENGTH;             \
        status = ZB_ZCL_STATUS_SUCCESS;                                              \
      }                                                                              \
    }                                                                                \
  }
#else /* defined ZB_ZCL_SUPPORT_DOOR_LOCK_SCENE_EXTENSIONS || defined DOXYGEN */
#define ZB_ZCL_CLUSTER_ID_DOOR_LOCK_SCENE_FIELD_SETS_LENGTH 0
#endif /*     defined ZB_ZCL_SUPPORT_DOOR_LOCK_SCENE_EXTENSIONS
          ||  defined DOXYGEN */

/*! @brief Scenes fieldsets representation structure for Thermostat
    cluster-specific content
*/
typedef ZB_PACKED_PRE struct zb_zcl_scenes_thermostat_fieldset_content_s
{
  zb_int16_t occupied_cooling_setpoint; /**< Occupied cooling setpoint
                                             attribute */
  zb_int16_t occupied_heating_setpoint; /**< Occupied heating setpoint
                                             attribute */
  zb_uint8_t system_mode;               /**< System mode attribute */
} ZB_PACKED_STRUCT zb_zcl_scenes_thermostat_fieldset_content_t;

/** @brief Scenes fieldsets representation structure for Thermostat cluster */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_fieldset_thermostat_s
{
  /** @brief Common part of fieldset definition */
  zb_zcl_scenes_fieldset_common_t common;
  /** @brief Fields specific to Thermostat cluster */
  zb_zcl_scenes_thermostat_fieldset_content_t specific;
} ZB_PACKED_STRUCT zb_zcl_scenes_fieldset_thermostat_t;

#if defined ZB_ZCL_SUPPORT_THERMOSTAT_SCENE_EXTENSIONS || defined DOXYGEN
/** @internal @brief Scenes fieldsets length for Color control cluster */
#define ZB_ZCL_CLUSTER_ID_THERMOSTAT_SCENE_FIELD_SETS_LENGTH \
  (sizeof(zb_zcl_scenes_fieldset_thermostat_t))

/** @internal @brief Stores fieldset for Thermostat cluster into Scene table
    @param endpoint to read attribute values from
    @param data_ptr - pointer to the place to put fieldset to. Will be
    incremented by total size of the fieldset on success
    @param max_data_size - maximum amount of data to store
    @param field_sets_length - length of the fieldsets already stored. Will be
    incremented after successfully storing the fieldset.
    @param status of the operation
*/
#define ZB_ZCL_SCENES_STORE_THERMOSTAT_FIELDSET(                                              \
    endpoint,                                                                                 \
    data_ptr,                                                                                 \
    max_data_size,                                                                            \
    field_sets_length,                                                                        \
    status)                                                                                   \
  {                                                                                           \
    if (  (   field_sets_length                                                               \
           +  ZB_ZCL_CLUSTER_ID_THERMOSTAT_SCENE_FIELD_SETS_LENGTH)                           \
        > max_data_size)                                                                      \
    {                                                                                         \
      status = ZB_ZCL_STATUS_INSUFF_SPACE;                                                    \
    }                                                                                         \
    else                                                                                      \
    {                                                                                         \
      zb_zcl_attr_t* attr_desc = zb_zcl_get_attr_desc_a(                                      \
                        endpoint,                                                             \
                        ZB_ZCL_CLUSTER_ID_THERMOSTAT,                                         \
                        ZB_ZCL_CLUSTER_SERVER_ROLE,                     \
                        ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID);                 \
      if (!attr_desc)                                                                         \
      {                                                                                       \
        status = ZB_ZCL_STATUS_INVALID_FIELD;                                                 \
      }                                                                                       \
      else                                                                                    \
      {                                                                                       \
        ((zb_zcl_scenes_fieldset_thermostat_t*)data_ptr)->                                    \
          common.cluster_id = ZB_ZCL_CLUSTER_ID_THERMOSTAT;                                   \
        ((zb_zcl_scenes_fieldset_thermostat_t*)data_ptr)->                                    \
        /* Cannot use any more specific type here - partial fieldset filling. */              \
          common.fieldset_length = sizeof(zb_int16_t);                                        \
        /*        TRACE_MSG(TRACE_ZCL3, "Hello2, *(zb_int16_t*)(attr_desc->data_p) %d", */    \
        /*          (FMT__D, *(zb_int16_t*)(attr_desc->data_p)));       */                    \
        ((zb_zcl_scenes_fieldset_thermostat_t*)data_ptr)->                                    \
          specific.occupied_cooling_setpoint =                                                \
          *(zb_int16_t*)(attr_desc->data_p);                                                  \
        attr_desc = zb_zcl_get_attr_desc_a(                                                   \
          endpoint,                                                                           \
          ZB_ZCL_CLUSTER_ID_THERMOSTAT,                                                       \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID);                               \
        if (!attr_desc)                                                                       \
        {                                                                                     \
          status = ZB_ZCL_STATUS_INVALID_FIELD;                                               \
        }                                                                                     \
        else                                                                                  \
        {                                                                                     \
          ((zb_zcl_scenes_fieldset_thermostat_t*)data_ptr)->                                  \
              specific.occupied_heating_setpoint =                                            \
                  *(zb_int16_t*)(attr_desc->data_p);                                          \
          ((zb_zcl_scenes_fieldset_thermostat_t*)data_ptr)->                                  \
          /* Cannot use any more specific type here - partial fieldset filling. */            \
              common.fieldset_length += sizeof(zb_int16_t);                                   \
          attr_desc = zb_zcl_get_attr_desc_a(                                                 \
                                      endpoint,                                               \
                                      ZB_ZCL_CLUSTER_ID_THERMOSTAT,                           \
                                      ZB_ZCL_CLUSTER_SERVER_ROLE, \
                                      ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID);                 \
          if (!attr_desc)                                                                     \
          {                                                                                   \
            status = ZB_ZCL_STATUS_INVALID_FIELD;                                             \
          }                                                                                   \
          else                                                                                \
          {                                                                                   \
            ((zb_zcl_scenes_fieldset_thermostat_t*)data_ptr)->                                \
                specific.system_mode = *(zb_uint8_t*)(attr_desc->data_p);                     \
            ((zb_zcl_scenes_fieldset_thermostat_t*)data_ptr)->                                \
                common.fieldset_length = sizeof(zb_zcl_scenes_thermostat_fieldset_content_t); \
            field_sets_length +=                                                              \
              ZB_ZCL_CLUSTER_ID_THERMOSTAT_SCENE_FIELD_SETS_LENGTH;                           \
            data_ptr += ZB_ZCL_CLUSTER_ID_THERMOSTAT_SCENE_FIELD_SETS_LENGTH;                 \
            status = ZB_ZCL_STATUS_SUCCESS;                                                   \
          }                                                                                   \
        }                                                                                     \
      }                                                                                       \
    }                                                                                         \
  }
#else /*    defined ZB_ZCL_SUPPORT_THERMOSTAT_SCENE_EXTENSIONS
        ||  defined DOXYGEN */
#define ZB_ZCL_CLUSTER_ID_THERMOSTAT_SCENE_FIELD_SETS_LENGTH 0
#endif /*     defined ZB_ZCL_SUPPORT_THERMOSTAT_SCENE_EXTENSIONS
          ||  defined DOXYGEN */

/*! @brief Scenes fieldsets representation structure for Window covering
    cluster-specific content
*/
typedef ZB_PACKED_PRE struct zb_zcl_scenes_window_covering_fieldset_content_s
{
  /** @brief WindowCovering.CurrentPositionLiftPercentage attribute value */
  zb_uint8_t current_position_lift_percentage;
  /** @brief WindowCovering.CurrentPositionTiltPercentage attribute value */
  zb_uint8_t current_position_tilt_percentage;
} ZB_PACKED_STRUCT zb_zcl_scenes_window_covering_fieldset_content_t;

/** @brief Scenes fieldsets representation structure for WindowCovering cluster
*/
typedef ZB_PACKED_PRE struct zb_zcl_scenes_fieldset_window_covering_s
{
  /** @brief Common part of fieldset definition */
  zb_zcl_scenes_fieldset_common_t common;
  /** @brief Fields specific to Window covering cluster */
  zb_zcl_scenes_window_covering_fieldset_content_t specific;
} ZB_PACKED_STRUCT zb_zcl_scenes_fieldset_window_covering_t;

#if defined ZB_ZCL_SUPPORT_WINDOW_COVERING_SCENE_EXTENSIONS || defined DOXYGEN
/** @internal @brief Scenes fieldsets length for Window covering cluster */
#define ZB_ZCL_CLUSTER_ID_WINDOW_COVERING_SCENE_FIELD_SETS_LENGTH \
  (sizeof(zb_zcl_scenes_fieldset_window_covering_t))

/** @internal @brief Stores fieldset for Window covering cluster into Scene
    table
    @param endpoint to read attribute values from
    @param data_ptr - pointer to the place to put fieldset to. Will be
    incremented by total size of the fieldset on success
    @param max_data_size - maximum amount of data to store
    @param field_sets_length - length of the fieldsets already stored. Will be
    incremented after successfully storing the fieldset.
    @param status of the operation
*/
#define ZB_ZCL_SCENES_STORE_WINDOW_COVERING_FIELDSET(                         \
    endpoint,                                                                 \
    data_ptr,                                                                 \
    max_data_size,                                                            \
    field_sets_length,                                                        \
    status)                                                                   \
  {                                                                           \
    if (  (   (field_sets_length)                                             \
           +  ZB_ZCL_CLUSTER_ID_WINDOW_COVERING_SCENE_FIELD_SETS_LENGTH)      \
        > (max_data_size))                                                    \
    {                                                                         \
      (status) = ZB_ZCL_STATUS_INSUFF_SPACE;                                  \
    }                                                                         \
    else                                                                      \
    {                                                                         \
      zb_zcl_attr_t* attr_desc = zb_zcl_get_attr_desc_a(                      \
            (endpoint),                                                       \
            ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,                                \
            ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
            ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID); \
    if (!attr_desc)                                                           \
      {                                                                       \
        (status) = ZB_ZCL_STATUS_INVALID_FIELD;                               \
      }                                                                       \
      else                                                                    \
      {                                                                       \
        ((zb_zcl_scenes_fieldset_window_covering_t*)(data_ptr))->             \
            common.cluster_id =                                               \
            ZB_ZCL_CLUSTER_ID_WINDOW_COVERING;                                \
        ((zb_zcl_scenes_fieldset_window_covering_t*)(data_ptr))->             \
            common.fieldset_length =                                          \
            sizeof(zb_zcl_scenes_window_covering_fieldset_content_t);         \
        ((zb_zcl_scenes_fieldset_window_covering_t*)(data_ptr))->             \
            specific.current_position_lift_percentage =                       \
            *(zb_uint16_t*)(attr_desc->data_p);                               \
        attr_desc = zb_zcl_get_attr_desc_a(                                   \
            (endpoint),                                                       \
            ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,                                \
            ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
            ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE_ID); \
        if (!attr_desc)                                                       \
        {                                                                     \
          (status) = ZB_ZCL_STATUS_INVALID_FIELD;                             \
        }                                                                     \
        else                                                                  \
        {                                                                     \
          ((zb_zcl_scenes_fieldset_window_covering_t*)(data_ptr))->           \
              specific.current_position_tilt_percentage =                     \
              *(zb_uint16_t*)(attr_desc->data_p);                             \
          (field_sets_length) +=                                              \
            ZB_ZCL_CLUSTER_ID_THERMOSTAT_SCENE_FIELD_SETS_LENGTH;             \
          (data_ptr) += ZB_ZCL_CLUSTER_ID_THERMOSTAT_SCENE_FIELD_SETS_LENGTH; \
          (status) = ZB_ZCL_STATUS_SUCCESS;                                   \
        }                                                                     \
      }                                                                       \
    }                                                                         \
  }
#else /*    defined ZB_ZCL_SUPPORT_WINDOW_COVERING_SCENE_EXTENSIONS
        ||  defined DOXYGEN */
#define ZB_ZCL_CLUSTER_ID_WINDOW_COVERING_SCENE_FIELD_SETS_LENGTH 0
#endif /*     defined ZB_ZCL_SUPPORT_WINDOW_COVERING_SCENE_EXTENSIONS
          ||  defined DOXYGEN */

/*! @internal @brief Marks all records in a table as free for a particular group
    @param endpoint - endpoint a scene table belongs to
    @param group - group identifier
    @return status of the operation
*/
zb_uint8_t zb_zcl_scenes_remove_all_scenes_action(
    zb_uint8_t endpoint,
    zb_uint16_t group);

/** @internal @brief Copies 16-bit attribute value
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
    @param status of the operation
*/
#define ZB_ZCL_SCENES_GET_16BIT_VALUE(                 \
    dst_data_ptr,                                      \
    src_data_ptr,                                      \
    content_length,                                    \
    dst_fieldset_length,                               \
    status)                                            \
  {                                                    \
    if (    content_length < sizeof(zb_uint16_t)       \
        ||  status != ZB_ZCL_STATUS_SUCCESS)           \
    {                                                  \
      (status) = ZB_ZCL_STATUS_INVALID_FIELD;          \
    }                                                  \
    else                                               \
    {                                                  \
      ZB_LETOH16((dst_data_ptr), (src_data_ptr));      \
      (dst_data_ptr) += sizeof(zb_uint16_t);           \
      (src_data_ptr) += sizeof(zb_uint16_t);           \
      (content_length) -= sizeof(zb_uint16_t);         \
      status = ZB_ZCL_STATUS_SUCCESS;                  \
      (*(dst_fieldset_length)) += sizeof(zb_uint16_t); \
    }                                                  \
  }

/** @internal @brief Gets pointer to fieldset begin.
    @param scene_table_record - pointer to the scene table record information
*/
#define ZB_ZCL_SCENES_GET_TABLE_FIELDSETS_BEGIN(scene_table_record) \
  (   ((scene_table_record)->field_sets_length)                     \
   ?  ((zb_uint8_t*)(scene_table_record) +                          \
        sizeof(zb_zcl_scene_table_record_fixed_t))                  \
   : NULL)

#define ZB_ZCL_SCENES_ADJUST_FIELDSET_PTR_INCONDITIONAL(cmd_struct_ptr)        \
  {                                                                            \
    (cmd_struct_ptr) += sizeof(zb_zcl_scenes_fieldset_common_t) +              \
        ((zb_zcl_scenes_fieldset_common_t*)(cmd_struct_ptr))->fieldset_length; \
  }

#define ZB_ZCL_SCENES_ADJUST_FIELDSET_PTR(cmd_struct_ptr)                      \
  {                                                                            \
    if (((zb_zcl_scenes_fieldset_common_t*)(cmd_struct_ptr))->fieldset_length) \
    {                                                                          \
      ZB_ZCL_SCENES_ADJUST_FIELDSET_PTR_INCONDITIONAL(cmd_struct_ptr)          \
    }                                                                          \
  }

#define ZB_ZCL_SCENES_RECALL_ON_OFF_ON_OFF(endpoint, cmd_struct_ptr)  \
  {                                                                   \
    if (((zb_zcl_scenes_fieldset_on_off_t*)(cmd_struct_ptr))->        \
            common.fieldset_length)                                   \
    {                                                                 \
      ZB_ZCL_SET_ATTRIBUTE(                                           \
          (endpoint),                                                 \
          ZB_ZCL_CLUSTER_ID_ON_OFF,                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
          ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,                               \
          &((((zb_zcl_scenes_fieldset_on_off_t*)(cmd_struct_ptr))->   \
                specific.on_off)),                                    \
          ZB_FALSE);                                                  \
    }                                                                 \
  }

#define ZB_ZCL_SCENES_RECALL_ON_OFF_FIELDSET(                     \
    endpoint,                                                     \
    cmd_struct_ptr)                                               \
  {                                                               \
    ZB_ZCL_SCENES_RECALL_ON_OFF_ON_OFF(endpoint, cmd_struct_ptr); \
  }

#define ZB_ZCL_SCENES_RECALL_LEVEL_CONTROL_CURRENT_LEVEL(                  \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
    if (((zb_zcl_scenes_fieldset_level_control_t*)(cmd_struct_ptr))->      \
            common.fieldset_length)                                        \
    {                                                                      \
      ZB_ZCL_SET_ATTRIBUTE(                                                \
          (endpoint),                                                      \
          ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID,                      \
          &((((zb_zcl_scenes_fieldset_level_control_t*)(cmd_struct_ptr))-> \
                specific.current_level)),                                  \
          ZB_FALSE);                                                       \
    }                                                                      \
  }

#define ZB_ZCL_SCENES_RECALL_LEVEL_CONTROL_FIELDSET(  \
    endpoint,                                         \
    cmd_struct_ptr)                                   \
  {                                                   \
    ZB_ZCL_SCENES_RECALL_LEVEL_CONTROL_CURRENT_LEVEL( \
        endpoint,                                     \
        cmd_struct_ptr);                              \
  }

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_X(                      \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
      ZB_ZCL_SET_ATTRIBUTE(                                                \
          (endpoint),                                                      \
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID,                          \
          (zb_uint8_t*)&((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))-> \
                specific.current_x)),                                      \
          ZB_FALSE);                                                       \
  }

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_Y(                      \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
      ZB_ZCL_SET_ATTRIBUTE(                                                \
          (endpoint),                                                      \
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID,                          \
          (zb_uint8_t*)&((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))-> \
                specific.current_y)),                                      \
          ZB_FALSE);                                                       \
  }

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_SATURATION(             \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
      ZB_ZCL_SET_ATTRIBUTE(                                                \
          (endpoint),                                                      \
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID,                 \
          (zb_uint8_t*)&((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))-> \
                specific.current_saturation)),                             \
          ZB_FALSE);                                                       \
  }

#if defined ZB_ENABLE_ZLL

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_ENHANCED_CURRENT_HUE(           \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
      ZB_ZCL_SET_ATTRIBUTE(                                                \
          (endpoint),                                                      \
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ZLL_ID,           \
          (zb_uint8_t*)&((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))-> \
                specific.enhanced_current_hue)),                           \
          ZB_FALSE);                                                       \
  }

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_COLOR_LOOP_ACTIVE(              \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
      ZB_ZCL_SET_ATTRIBUTE(                                                \
          (endpoint),                                                      \
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ZLL_ID,              \
          (zb_uint8_t*)&((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))-> \
                specific.color_loop_active)),                              \
          ZB_FALSE);                                                       \
  }

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_COLOR_LOOP_DIRECTION(           \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
      ZB_ZCL_SET_ATTRIBUTE(                                                \
          (endpoint),                                                      \
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ZLL_ID,           \
          (zb_uint8_t*)&((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))-> \
                specific.color_loop_direction)),                           \
          ZB_FALSE);                                                       \
  }

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_COLOR_LOOP_TIME(                \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
      ZB_ZCL_SET_ATTRIBUTE(                                                \
          (endpoint),                                                      \
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_TIME_ZLL_ID,                \
          (zb_uint8_t*)&((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))-> \
                specific.color_loop_time)),                                \
          ZB_FALSE);                                                       \
  }

#endif /* defined ZB_ENABLE_ZLL */

#if defined ZB_ENABLE_ZLL

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_FIELDSET(                                        \
    endpoint,                                                                               \
    cmd_struct_ptr)                                                                         \
  {                                                                                         \
    if (((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->common.fieldset_length \
        >= sizeof(zb_zcl_scenes_color_control_fieldset_content_t))                          \
    {                                                                                       \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_X(endpoint, cmd_struct_ptr);               \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_Y(endpoint, cmd_struct_ptr);               \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_ENHANCED_CURRENT_HUE(endpoint, cmd_struct_ptr);    \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_SATURATION(endpoint, cmd_struct_ptr);     \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_COLOR_LOOP_ACTIVE(endpoint, cmd_struct_ptr);       \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_COLOR_LOOP_DIRECTION(endpoint, cmd_struct_ptr);    \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_COLOR_LOOP_TIME(endpoint, cmd_struct_ptr);         \
    }                                                                                       \
  }

#else

#define ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_FIELDSET(                                        \
    endpoint,                                                                               \
    cmd_struct_ptr)                                                                         \
  {                                                                                         \
    if (((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->common.fieldset_length \
        >= sizeof(zb_zcl_scenes_color_control_fieldset_content_t))                          \
    {                                                                                       \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_X(endpoint, cmd_struct_ptr);               \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_Y(endpoint, cmd_struct_ptr);               \
      ZB_ZCL_SCENES_RECALL_COLOR_CONTROL_CURRENT_SATURATION(endpoint, cmd_struct_ptr);     \
    }                                                                                       \
  }

#endif

#define ZB_ZCL_SCENES_RECALL_DOOR_LOCK_LOCK_STATE(endpoint, cmd_struct_ptr) \
  {                                                                         \
    if (((zb_zcl_scenes_fieldset_door_lock_t*)(cmd_struct_ptr))->           \
            common.fieldset_length)                                         \
    {                                                                       \
      ZB_ZCL_SET_ATTRIBUTE(                                                 \
          (endpoint),                                                       \
          ZB_ZCL_CLUSTER_ID_DOOR_LOCK,                                      \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID,                              \
          &((((zb_zcl_scenes_fieldset_door_lock_t*)(cmd_struct_ptr))->      \
                specific.lock_state)),                                      \
          ZB_FALSE);                                                        \
    }                                                                       \
  }

#define ZB_ZCL_SCENES_RECALL_DOOR_LOCK_FIELDSET(                         \
    endpoint,                                                            \
    cmd_struct_ptr)                                                      \
  {                                                                      \
    ZB_ZCL_SCENES_RECALL_DOOR_LOCK_LOCK_STATE(endpoint, cmd_struct_ptr); \
  }

#define ZB_ZCL_SCENES_RECALL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT(      \
    endpoint,                                                           \
    cmd_struct_ptr)                                                     \
  {                                                                     \
    if (((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))->      \
            common.fieldset_length >= sizeof(zb_uint16_t))              \
    {                                                                   \
      ZB_ZCL_SET_ATTRIBUTE(                                             \
          (endpoint),                                                   \
          ZB_ZCL_CLUSTER_ID_THERMOSTAT,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID,          \
          (zb_uint8_t *)&((((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))-> \
                specific.occupied_cooling_setpoint)),                   \
          ZB_FALSE);                                                    \
    }                                                                   \
  }

#define ZB_ZCL_SCENES_RECALL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT(      \
    endpoint,                                                           \
    cmd_struct_ptr)                                                     \
  {                                                                     \
    if (((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))->      \
            common.fieldset_length >= 2 * sizeof(zb_uint16_t))          \
    {                                                                   \
      ZB_ZCL_SET_ATTRIBUTE(                                             \
          (endpoint),                                                   \
          ZB_ZCL_CLUSTER_ID_THERMOSTAT,                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID,          \
          (zb_uint8_t *)&((((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))-> \
                specific.occupied_heating_setpoint)),                   \
          ZB_FALSE);                                                    \
    }                                                                   \
  }

#define ZB_ZCL_SCENES_RECALL_THERMOSTAT_SYSTEM_MODE(endpoint, cmd_struct_ptr) \
  {                                                                           \
    if (((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))->            \
            common.fieldset_length > 2 * sizeof(zb_uint16_t))                 \
    {                                                                         \
      ZB_ZCL_SET_ATTRIBUTE(                                                   \
          (endpoint),                                                         \
          ZB_ZCL_CLUSTER_ID_THERMOSTAT,                                       \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID,                              \
          &((((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))->       \
                specific.system_mode)),                                       \
          ZB_FALSE);                                                          \
    }                                                                         \
  }

#define ZB_ZCL_SCENES_RECALL_THERMOSTAT_FIELDSET(              \
    endpoint,                                                  \
    cmd_struct_ptr)                                            \
  {                                                            \
    ZB_ZCL_SCENES_RECALL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT( \
        endpoint, cmd_struct_ptr);                             \
    ZB_ZCL_SCENES_RECALL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT( \
        endpoint, cmd_struct_ptr);                             \
    ZB_ZCL_SCENES_RECALL_THERMOSTAT_SYSTEM_MODE(               \
        endpoint, cmd_struct_ptr);                             \
  }

#define ZB_ZCL_SCENES_RECALL_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE( \
    endpoint,                                                                  \
    cmd_struct_ptr)                                                            \
  {                                                                            \
    if (((zb_zcl_scenes_fieldset_window_covering_t*)(cmd_struct_ptr))->        \
            common.fieldset_length)                                            \
    {                                                                          \
      ZB_ZCL_SET_ATTRIBUTE(                                                    \
          (endpoint),                                                          \
          ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID,     \
          &((((zb_zcl_scenes_fieldset_window_covering_t*)(cmd_struct_ptr))->   \
                specific.current_position_lift_percentage)),                   \
          ZB_FALSE);                                                           \
    }                                                                          \
  }

#define ZB_ZCL_SCENES_RECALL_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE( \
    endpoint,                                                                  \
    cmd_struct_ptr)                                                            \
  {                                                                            \
    if (((zb_zcl_scenes_fieldset_window_covering_t*)(cmd_struct_ptr))->        \
            common.fieldset_length > sizeof(zb_uint8_t))                       \
    {                                                                          \
      ZB_ZCL_SET_ATTRIBUTE(                                                    \
          (endpoint),                                                          \
          ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,                                   \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
          ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE_ID,     \
          &((((zb_zcl_scenes_fieldset_window_covering_t*)(cmd_struct_ptr))->   \
                specific.current_position_tilt_percentage)),                   \
          ZB_FALSE);                                                           \
    }                                                                          \
  }

#define ZB_ZCL_SCENES_RECALL_WINDOW_COVERING_FIELDSET(                     \
    endpoint,                                                              \
    cmd_struct_ptr)                                                        \
  {                                                                        \
    ZB_ZCL_SCENES_RECALL_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE( \
        endpoint,                                                          \
        cmd_struct_ptr);                                                   \
    ZB_ZCL_SCENES_RECALL_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE( \
        endpoint,                                                          \
        cmd_struct_ptr);                                                   \
  }

#define ZB_ZCL_SCENES_ADD_COMMON_FIELDSET_PART(cmd_struct_ptr, dst_data_ptr) \
  {                                                                          \
    ZB_HTOLE16(dst_data_ptr, cmd_struct_ptr);                                \
    ((zb_zcl_scenes_fieldset_common_t*)dst_data_ptr)->fieldset_length =      \
        ((zb_zcl_scenes_fieldset_common_t*)cmd_struct_ptr)->fieldset_length; \
    dst_data_ptr += sizeof(zb_zcl_scenes_fieldset_common_t);                 \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_ON_OFF_ON_OFF(cmd_struct_ptr, dst_data_ptr) \
  {                                                                          \
    if (((zb_zcl_scenes_fieldset_on_off_t*)(cmd_struct_ptr))->               \
            common.fieldset_length)                                          \
    {                                                                        \
      ZB_ZCL_SCENES_ADD_FIELDSET_ON_OFF_ATTR_ON_OFF(                         \
          (dst_data_ptr),                                                    \
          ((((zb_zcl_scenes_fieldset_on_off_t*)(cmd_struct_ptr))->           \
                specific.on_off)));                                          \
    }                                                                        \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_ON_OFF_FIELDSET(                         \
    cmd_struct_ptr,                                                       \
    dst_data_ptr)                                                         \
  {                                                                       \
    ZB_ZCL_SCENES_VIEW_SCENE_ON_OFF_ON_OFF(cmd_struct_ptr, dst_data_ptr); \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_LEVEL_CONTROL_CURRENT_LEVEL(             \
    cmd_struct_ptr,                                                       \
    dst_data_ptr)                                                         \
  {                                                                       \
    if (((zb_zcl_scenes_fieldset_level_control_t*)(cmd_struct_ptr))->     \
            common.fieldset_length)                                       \
    {                                                                     \
      ZB_ZCL_SCENES_ADD_FIELDSET_LEVEL_CONTROL_ATTR_CURRENT_LEVEL(        \
          (dst_data_ptr),                                                 \
          ((((zb_zcl_scenes_fieldset_level_control_t*)(cmd_struct_ptr))-> \
                specific.current_level)));                                \
    }                                                                     \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_LEVEL_CONTROL_FIELDSET(  \
    cmd_struct_ptr,                                       \
    dst_data_ptr)                                         \
  {                                                       \
    ZB_ZCL_SCENES_VIEW_SCENE_LEVEL_CONTROL_CURRENT_LEVEL( \
        cmd_struct_ptr,                                   \
        dst_data_ptr);                                    \
  }

#if defined ZB_ENABLE_ZLL
#define ZB_ZCL_SCENES_VIEW_SCENE_COLOR_CONTROL_FIELDSET(                    \
    cmd_struct_ptr,                                                         \
    dst_data_ptr)                                                           \
  {                                                                         \
    if (((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->       \
            common.fieldset_length >=                                       \
         sizeof(zb_zcl_scenes_color_control_fieldset_content_t))            \
    {                                                                       \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_X(              \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.current_x)));                                      \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_Y(              \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.current_y)));                                      \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_ENHANCED_CURRENT_HUE(   \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.enhanced_current_hue)));                           \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_SATURATION(     \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.current_saturation)));                             \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_COLOR_LOOP_ACTIVE(      \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.color_loop_active)));                              \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_COLOR_LOOP_DIRECTION(   \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.color_loop_direction)));                           \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_COLOR_LOOP_TIME(        \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.color_loop_time)));                                \
    }                                                                       \
  }
#else
#define ZB_ZCL_SCENES_VIEW_SCENE_COLOR_CONTROL_FIELDSET(                    \
    cmd_struct_ptr,                                                         \
    dst_data_ptr)                                                           \
  {                                                                         \
    if (((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->       \
            common.fieldset_length >=                                       \
         sizeof(zb_zcl_scenes_color_control_fieldset_content_t))            \
    {                                                                       \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_X(              \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.current_x)));                                      \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_Y(              \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.current_y)));                                      \
      ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_SATURATION(     \
          (dst_data_ptr),                                                   \
          ((((zb_zcl_scenes_fieldset_color_control_t*)(cmd_struct_ptr))->   \
                specific.current_saturation)));                             \
    }                                                                       \
  }
#endif

#define ZB_ZCL_SCENES_VIEW_SCENE_DOOR_LOCK_LOCK_STATE(                \
    cmd_struct_ptr,                                                   \
    dst_data_ptr)                                                     \
  {                                                                   \
    if (((zb_zcl_scenes_fieldset_door_lock_t*)(cmd_struct_ptr))->     \
            common.fieldset_length)                                   \
    {                                                                 \
      ZB_ZCL_SCENES_ADD_FIELDSET_DOOR_LOCK_ATTR_LOCK_STATE(           \
          (dst_data_ptr),                                             \
          ((((zb_zcl_scenes_fieldset_door_lock_t*)(cmd_struct_ptr))-> \
                specific.lock_state)));                               \
    }                                                                 \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_DOOR_LOCK_FIELDSET( \
    cmd_struct_ptr,                                  \
    dst_data_ptr)                                    \
  {                                                  \
    ZB_ZCL_SCENES_VIEW_SCENE_DOOR_LOCK_LOCK_STATE(   \
        cmd_struct_ptr,                              \
        dst_data_ptr);                               \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_THERMOSTAT_OCCUPIED_COOLING_SETPOINT(      \
    cmd_struct_ptr,                                                         \
    dst_data_ptr)                                                           \
  {                                                                         \
    if (((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))->          \
            common.fieldset_length >= sizeof(zb_int16_t))                   \
    {                                                                       \
      ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_OCCUPIED_COOLING_SETPOINT( \
          (dst_data_ptr),                                                   \
          ((zb_uint8_t*)&(((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))-> \
                specific.occupied_cooling_setpoint)));                      \
    }                                                                       \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_THERMOSTAT_OCCUPIED_HEATING_SETPOINT(      \
    cmd_struct_ptr,                                                         \
    dst_data_ptr)                                                           \
  {                                                                         \
    if (((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))->          \
            common.fieldset_length >= 2 * sizeof(zb_int16_t))               \
    {                                                                       \
      ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_OCCUPIED_HEATING_SETPOINT( \
          (dst_data_ptr),                                                   \
          ((zb_uint8_t*)&(((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))->      \
                specific.occupied_heating_setpoint)));                      \
    }                                                                       \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_THERMOSTAT_SYSTEM_MODE(              \
    cmd_struct_ptr,                                                   \
    dst_data_ptr)                                                     \
  {                                                                   \
    /* Checking size against 4 bytes - 2nd 2-byte field */            \
    if (((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))->    \
            common.fieldset_length > 2 * sizeof(zb_int16_t))          \
    {                                                                 \
      ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_SYSTEM_MODE(         \
          (dst_data_ptr),                                             \
          ((((zb_zcl_scenes_fieldset_thermostat_t*)(cmd_struct_ptr))-> \
                specific.system_mode)));                              \
    }                                                                 \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_THERMOSTAT_FIELDSET(              \
    cmd_struct_ptr,                                                \
    dst_data_ptr)                                                  \
  {                                                                \
    ZB_ZCL_SCENES_VIEW_SCENE_THERMOSTAT_OCCUPIED_COOLING_SETPOINT( \
        cmd_struct_ptr,                                            \
        dst_data_ptr);                                             \
    ZB_ZCL_SCENES_VIEW_SCENE_THERMOSTAT_OCCUPIED_HEATING_SETPOINT( \
        cmd_struct_ptr,                                            \
        dst_data_ptr);                                             \
    ZB_ZCL_SCENES_VIEW_SCENE_THERMOSTAT_SYSTEM_MODE(               \
        cmd_struct_ptr,                                            \
        dst_data_ptr);                                             \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE(      \
    cmd_struct_ptr,                                                                     \
    dst_data_ptr)                                                                       \
  {                                                                                     \
    if (((zb_zcl_scenes_fieldset_window_covering_t*)(cmd_struct_ptr))->                 \
            common.fieldset_length)                                                     \
    {                                                                                   \
      ZB_ZCL_SCENES_ADD_FIELDSET_WINDOW_COVERING_ATTR_CURRENT_POSITION_LIFT_PERCENTAGE( \
          (dst_data_ptr),                                                               \
          ((((zb_zcl_scenes_fieldset_window_covering_t*)(cmd_struct_ptr))->             \
                specific.current_position_lift_percentage)));                           \
    }                                                                                   \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE(      \
    cmd_struct_ptr,                                                                     \
    dst_data_ptr)                                                                       \
  {                                                                                     \
    if (((zb_zcl_scenes_fieldset_window_covering_t*)(cmd_struct_ptr))->                 \
            common.fieldset_length > sizeof(zb_uint8_t))                                \
    {                                                                                   \
      ZB_ZCL_SCENES_ADD_FIELDSET_WINDOW_COVERING_ATTR_CURRENT_POSITION_LIFT_PERCENTAGE( \
          (dst_data_ptr),                                                               \
          ((((zb_zcl_scenes_fieldset_window_covering_t*)(cmd_struct_ptr))->             \
                specific.current_position_tilt_percentage)));                           \
    }                                                                                   \
  }

#define ZB_ZCL_SCENES_VIEW_SCENE_WINDOW_COVERING_FIELDSET(                     \
    cmd_struct_ptr,                                                            \
    dst_data_ptr)                                                              \
  {                                                                            \
    ZB_ZCL_SCENES_VIEW_SCENE_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE( \
        cmd_struct_ptr,                                                        \
        dst_data_ptr);                                                         \
    ZB_ZCL_SCENES_VIEW_SCENE_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE( \
        cmd_struct_ptr,                                                        \
        dst_data_ptr);                                                         \
  }

/*! @} */ /* Scenes cluster internals */

/*! @name Scenes cluster commands
    @{
*/

/*! @brief Command identifiers for Scenes Cluster
    @see ZCL spec, subclause 3.7.2.4
*/
enum zb_zcl_scenes_cmd_e
{
  /*! Add scene command */
  ZB_ZCL_CMD_SCENES_ADD_SCENE             = 0x00,
  /*! View scene command */
  ZB_ZCL_CMD_SCENES_VIEW_SCENE            = 0x01,
  /*! Remove scene command */
  ZB_ZCL_CMD_SCENES_REMOVE_SCENE          = 0x02,
  /*! Remove all scenes command */
  ZB_ZCL_CMD_SCENES_REMOVE_ALL_SCENES     = 0x03,
  /*! Store scene command */
  ZB_ZCL_CMD_SCENES_STORE_SCENE           = 0x04,
  /*! Recall scene command */
  ZB_ZCL_CMD_SCENES_RECALL_SCENE          = 0x05,
  /*! Get scene membership command */
  ZB_ZCL_CMD_SCENES_GET_SCENE_MEMBERSHIP  = 0x06
};

/*! @brief Command response identifiers for Scenes Cluster
    @see ZCL spec, subclause 3.7.2.5
*/
enum zb_zcl_scenes_cmd_resp_e
{
  /*! Add scene response */
  ZB_ZCL_CMD_SCENES_ADD_SCENE_RESPONSE            = 0x00,
  /*! View scene response */
  ZB_ZCL_CMD_SCENES_VIEW_SCENE_RESPONSE           = 0x01,
  /*! Remove scene response */
  ZB_ZCL_CMD_SCENES_REMOVE_SCENE_RESPONSE         = 0x02,
  /*! Remove all scenes response */
  ZB_ZCL_CMD_SCENES_REMOVE_ALL_SCENES_REPONSE     = 0x03,
  /*! Store scene response */
  ZB_ZCL_CMD_SCENES_STORE_SCENE_RESPONSE          = 0x04,
  /*! Get scene membership response */
  ZB_ZCL_CMD_SCENES_GET_SCENE_MEMBERSHIP_RESPONSE = 0x06
};

/*! @brief Add scene command payload constant-structure part */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_add_scene_req_s
{
  zb_uint16_t group_id;         /**< Scene group identifier */
  zb_uint8_t scene_id;          /**< Scene identifier */
  zb_uint16_t transition_time;  /**< Scene transition time */
  /*! @brief Scene name

      Has zero length (for compatibility) as unsupported attribute.
  */
  zb_uint8_t scene_name[1];
} ZB_PACKED_STRUCT zb_zcl_scenes_add_scene_req_t;

/*! @brief Start filling Add scene command frame

    Fills the constant-structure part of the command payload only.

    Command can contain no fieldsets defining empty scene. The new scene can be
    used, for example, for storing fieldsets with Store scene command.
    @note According to a 3.7.2.4.2. subclause of ZCL spec, the command shall be
    addressed to a single device (not a group).
    @param buffer to put packet to
    @param cmd_struct_ptr - pointer to the place variable-structure part of the
    command payload (fieldsets) should be placed.
    @param dis_default_resp - enable/disable default response
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
    @param transition_time - scene transition time
*/
#define ZB_ZCL_SCENES_INIT_ADD_SCENE_REQ(                            \
    buffer,                                                          \
    cmd_struct_ptr,                                                  \
    dis_default_resp,                                                \
    group_id,                                                        \
    scene_id,                                                        \
    transition_time)                                                 \
  {                                                                  \
    cmd_struct_ptr = ZB_ZCL_START_PACKET_REQ(buffer)                    \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(             \
        cmd_struct_ptr,                                              \
        dis_default_resp)                                           \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                                 \
        cmd_struct_ptr,                                              \
        ZB_ZCL_GET_SEQ_NUM(),                                        \
        ZB_ZCL_CMD_SCENES_ADD_SCENE)                                \
    ZB_ZCL_PACKET_REQ_PUT_DATA16_VAL(cmd_struct_ptr, (group_id));        \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, (scene_id));             \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_struct_ptr, (transition_time)); \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, 0);                      \
  }

/*! @brief Sends Add scene command

    @note According to a 3.7.2.4.2. subclause of ZCL spec, the command shall be
    addressed to a single device (not a group).
    @param buffer - with prepared command payload
    @param cmd_struct_ptr - pointer to the place variable-structure part of the
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param callback for getting command send status
*/
#define ZB_ZCL_SCENES_SEND_ADD_SCENE_REQ(         \
    buffer,                                       \
    cmd_struct_ptr,                               \
    dst_addr,                                     \
    dst_ep,                                       \
    ep,                                           \
    prof_id,                                      \
    callback)                                     \
  {                                               \
    ZB_ZCL_FINISH_PACKET(buffer, cmd_struct_ptr) \
    ZB_ZCL_SEND_COMMAND_SHORT(                    \
        buffer,                                   \
        dst_addr,                                 \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,         \
        dst_ep,                                   \
        ep,                                       \
        prof_id,                                  \
        ZB_ZCL_CLUSTER_ID_SCENES,                 \
        callback);                                \
  }

/** @brief Adds fieldset into the buffer for sending command
    @param cmd_struct_ptr - pointer to the place in the buffer to put data to
    @param cluster identifier the fieldset under consideration belongs to
    @param fs_length - summary length of the attributes in the fieldset
    @attention The order of the attribute values in the fieldset is significant
*/
#define ZB_ZCL_SCENES_INIT_FIELDSET(cmd_struct_ptr, cluster, fs_length) \
  {                                                                     \
    ZB_ZCL_PACKET_PUT_DATA16_VAL((cmd_struct_ptr), (cluster));          \
    ZB_ZCL_PACKET_PUT_DATA8((cmd_struct_ptr), (fs_length));             \
  }

/** @brief Adds OnOff.OnOff attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param on_off_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_ON_OFF_ATTR_ON_OFF(    \
    cmd_struct_ptr,                                       \
    on_off_value)                                         \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, on_off_value)

/** @brief Adds LevelControl.CurrentLevel attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param current_level_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_LEVEL_CONTROL_ATTR_CURRENT_LEVEL( \
    cmd_struct_ptr,                                                  \
    current_level_value)                                             \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, current_level_value)

/** @brief Adds ColorControl.CurrentX attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param current_x_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_X(  \
    cmd_struct_ptr,                                               \
    current_x_value)                                              \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_struct_ptr, current_x_value)

/** @brief Adds ColorControl.CurrentY attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param current_y_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_Y(            \
    cmd_struct_ptr,                                                         \
    current_y_value)                                                        \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_struct_ptr, current_y_value)

/** @brief Adds ColorControl.CurrentSaturation attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param current_saturation_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_SATURATION(   \
    cmd_struct_ptr,                                                         \
    current_saturation_value)                                               \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, current_saturation_value)

#if defined ZB_ENABLE_ZLL

/** @brief Adds ColorControl.EnhancedCurrentHue attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param enhanced_hue_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_ENHANCED_CURRENT_HUE( \
    cmd_struct_ptr,                                                         \
    enhanced_hue_value)                                                     \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_struct_ptr, enhanced_hue_value)

/** @brief Adds ColorControl.ColorLoopActive attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param color_loop_active_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_COLOR_LOOP_ACTIVE(    \
    cmd_struct_ptr,                                                         \
    color_loop_active_value)                                                \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, color_loop_active_value)

/** @brief Adds ColorControl.ColorLoopDirection attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param color_loop_direction_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_COLOR_LOOP_DIRECTION( \
    cmd_struct_ptr,                                                         \
    color_loop_direction_value)                                             \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, color_loop_direction_value)

/** @brief Adds ColorControl.ColorLoopTime attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param color_loop_time_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_COLOR_LOOP_TIME(      \
    cmd_struct_ptr,                                                         \
    color_loop_time_value)                                                  \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_struct_ptr, color_loop_time_value)

#endif /* defined ZB_ENABLE_ZLL */

/** @brief Adds DoorLock.LockState attribute value to the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param lock_state_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_DOOR_LOCK_ATTR_LOCK_STATE( \
    cmd_struct_ptr,                                           \
    lock_state_value)                                         \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, lock_state_value)

/** @brief Adds Thermostat.OccupiedCoolingSetpoint attribute value to the
    fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param occupied_cooling_setpoint_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_OCCUPIED_COOLING_SETPOINT( \
    cmd_struct_ptr,                                                           \
    occupied_cooling_setpoint_value)                                          \
    ZB_ZCL_PACKET_PUT_DATA16(cmd_struct_ptr, occupied_cooling_setpoint_value)

/** @brief Adds Thermostat.OccupiedHeatingSetpoint attribute value to the
    fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param occupied_heating_setpoint_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_OCCUPIED_HEATING_SETPOINT( \
    cmd_struct_ptr,                                                           \
    occupied_heating_setpoint_value)                                          \
    ZB_ZCL_PACKET_PUT_DATA16(cmd_struct_ptr, occupied_heating_setpoint_value)

/** @brief Adds Thermostat.SystemMode attribute value to the
    fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param system_mode_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_SYSTEM_MODE( \
    cmd_struct_ptr,                                             \
    system_mode_value)                                          \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, system_mode_value)

/** @brief Adds WindowCovering.CurrentPositionLiftPercentage attribute value to
    the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param current_position_lift_percentage_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_WINDOW_COVERING_ATTR_CURRENT_POSITION_LIFT_PERCENTAGE( \
    cmd_struct_ptr,                                                                       \
    current_position_lift_percentage_value)                                               \
    ZB_ZCL_PACKET_PUT_DATA8(                                                              \
        cmd_struct_ptr,                                                                   \
        current_position_lift_percentage_value)

/** @brief Adds WindowCovering.CurrentPositionTiltPercentage attribute value to
    the fieldset
    @param cmd_struct_ptr - pointer to the place to put attribute value to
    @param current_position_tilt_percentage_value - value to put into the buffer
*/
#define ZB_ZCL_SCENES_ADD_FIELDSET_WINDOW_COVERING_ATTR_CURRENT_POSITION_TILT_PERCENTAGE( \
    cmd_struct_ptr,                                                                       \
    current_position_tilt_percentage_value)                                               \
    ZB_ZCL_PACKET_PUT_DATA8(                                                              \
        cmd_struct_ptr,                                                                   \
        current_position_tilt_percentage_value)

/** @brief Adds fieldset for On/Off cluster
    @param cmd_struct_ptr - pionter to the place to put fieldset to
    @param on_off_value - value of the OnOff.OnOff attribute to put into the
    fieldset
*/
#define ZB_ZCL_SCENES_ADD_ON_OFF_FIELDSET(cmd_struct_ptr, on_off_value) \
  {                                                                     \
    ZB_ZCL_SCENES_INIT_FIELDSET(                                        \
        cmd_struct_ptr,                                                 \
        ZB_ZCL_CLUSTER_ID_ON_OFF,                                       \
        sizeof(zb_zcl_scenes_on_off_fieldset_content_t));               \
    ZB_ZCL_SCENES_ADD_FIELDSET_ON_OFF_ATTR_ON_OFF(                      \
        cmd_struct_ptr,                                                 \
        on_off_value);                                                  \
  }

/** @brief Adds fieldset for Level control cluster
    @param cmd_struct_ptr - pionter to the place to put fieldset to
    @param current_level_value - value of the LevelControl.CurrentLevel
    attribute to put into the fieldset
*/
#define ZB_ZCL_SCENES_ADD_LEVEL_CONTROL_FIELDSET(                \
    cmd_struct_ptr,                                              \
    current_level_value)                                         \
  {                                                              \
    ZB_ZCL_SCENES_INIT_FIELDSET(                                 \
        cmd_struct_ptr,                                          \
        ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,                         \
        sizeof(zb_zcl_scenes_level_control_fieldset_content_t)); \
    ZB_ZCL_SCENES_ADD_FIELDSET_LEVEL_CONTROL_ATTR_CURRENT_LEVEL( \
        cmd_struct_ptr,                                          \
        current_level_value);                                    \
  }

/** @brief Adds fieldset for Color control cluster
    @param cmd_struct_ptr - pionter to the place to put fieldset to
    @param current_x_value - value of the ColorControl.CurrentX attribute to put
    into the fieldset
    @param current_y_value - value of the ColorControl.CurrentY attribute to put
    into the fieldset
*/
#define ZB_ZCL_SCENES_ADD_COLOR_CONTROL_FIELDSET(                \
    cmd_struct_ptr,                                              \
    current_x_value,                                             \
    current_y_value)                                             \
  {                                                              \
    ZB_ZCL_SCENES_INIT_FIELDSET(                                 \
        cmd_struct_ptr,                                          \
        ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,                         \
        sizeof(zb_zcl_scenes_color_control_fieldset_content_t)); \
    ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_X(     \
        cmd_struct_ptr,                                          \
        current_x_value);                                        \
    ZB_ZCL_SCENES_ADD_FIELDSET_COLOR_CONTROL_ATTR_CURRENT_Y(     \
        cmd_struct_ptr,                                          \
        current_y_value);                                        \
  }

/** @brief Adds fieldset for Door lock cluster
    @param cmd_struct_ptr - pionter to the place to put fieldset to
    @param lock_state_value - value of the DoorLock.LockState attribute to put
    into the fieldset
*/
#define ZB_ZCL_SCENES_ADD_DOOR_LOCK_FIELDSET(cmd_struct_ptr, lock_state_value) \
  {                                                                            \
    ZB_ZCL_SCENES_INIT_FIELDSET(                                               \
        cmd_struct_ptr,                                                        \
        ZB_ZCL_CLUSTER_ID_DOOR_LOCK,                                           \
        sizeof(zb_zcl_scenes_door_lock_fieldset_content_t));                   \
    ZB_ZCL_SCENES_ADD_FIELDSET_DOOR_LOCK_ATTR_LOCK_STATE(                      \
        cmd_struct_ptr,                                                        \
        lock_state_value);                                                     \
  }

/** @brief Adds fieldset for Thermostat cluster
    @param cmd_struct_ptr - pionter to the place to put fieldset to
    @param occupied_cooling_setpoint_value - value of the
    Thermostat.OccupiedCoolingSetpoint attribute to put into the fieldset
    @param occupied_heating_setpoint_value - value of the
    Thermostat.OccupiedHeatingSetpoint attribute to put into the fieldset
    @param system_mode_value - value of the Thermostat.SystemMode attribute to
    put into the fieldset
*/
#define ZB_ZCL_SCENES_ADD_THERMOSTAT_FIELDSET(                            \
    cmd_struct_ptr,                                                       \
    occupied_cooling_setpoint_value,                                      \
    occupied_heating_setpoint_value,                                      \
    system_mode_value)                                                    \
  {                                                                       \
    ZB_ZCL_SCENES_INIT_FIELDSET(                                          \
        cmd_struct_ptr,                                                   \
        ZB_ZCL_CLUSTER_ID_THERMOSTAT,  ,                                  \
        sizeof(zb_zcl_scenes_thermostat_fieldset_content_t));             \
    ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_OCCUPIED_COOLING_SETPOINT( \
        cmd_struct_ptr,                                                   \
        occupied_cooling_setpoint_value);                                 \
    ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_OCCUPIED_HEATING_SETPOINT( \
        cmd_struct_ptr,                                                   \
        occupied_heating_setpoint_value);                                 \
    ZB_ZCL_SCENES_ADD_FIELDSET_THERMOSTAT_ATTR_SYSTEM_MODE(               \
        cmd_struct_ptr,                                                   \
        system_mode_value);                                               \
  }

/** @brief Adds fieldset for Window covering cluster
    @param cmd_struct_ptr - pionter to the place to put fieldset to
    @param current_position_lift_percentage_value - value of the
    WindowCovering.CurrentPositionLiftPercentage attribute to put into the
    fieldset
    @param current_position_tilt_percentage_value - value of the
    WindowCovering.CurrentPositionTiltPercentage attribute to put into the
    fieldset
*/
#define ZB_ZCL_SCENES_ADD_WINDOW_COVERING_FIELDSET(                                   \
    cmd_struct_ptr,                                                                   \
    current_position_lift_percentage_value,                                           \
    current_position_tilt_percentage_value)                                           \
  {                                                                                   \
    ZB_ZCL_SCENES_INIT_FIELDSET(                                                      \
        cmd_struct_ptr,                                                               \
        ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,                                            \
        sizeof(zb_zcl_scenes_window_covering_fieldset_content_t));                    \
    ZB_ZCL_SCENES_ADD_FIELDSET_WINDOW_COVERING_ATTR_CURRENT_POSITION_LIFT_PERCENTAGE( \
        cmd_struct_ptr,                                                               \
        current_position_lift_percentage_value);                                      \
    ZB_ZCL_SCENES_ADD_FIELDSET_WINDOW_COVERING_ATTR_CURRENT_POSITION_TILT_PERCENTAGE( \
        cmd_struct_ptr,                                                               \
        current_position_tilt_percentage_value);                                      \
  }

/** @brief Parse common part of Add Scene command
    @param buffer containing Store scene command payload
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_add_scene_req_s) or NULL pointer if payload size is
    too small
    @attention The macro changes content of the buffer
    @note The macro ignores scene name in the buffer, and, if it has non-zero
    size, skips it and directly sets length of the scene name to zero in the
    command representation structure
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COMMON(buffer, cmd_struct_ptr) \
  {                                                                    \
    if (sizeof(zb_zcl_scenes_add_scene_req_t) > ZB_BUF_LEN((buffer)))  \
    {                                                                  \
      (cmd_struct_ptr) = NULL;                                         \
    }                                                                  \
    else                                                               \
    {                                                                  \
      (cmd_struct_ptr) =                                               \
        (zb_zcl_scenes_add_scene_req_t*)ZB_BUF_BEGIN(buffer);          \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);             \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->transition_time);      \
      ZB_BUF_CUT_LEFT2(                                                \
          (buffer),                                                    \
          sizeof(zb_zcl_scenes_add_scene_req_t) +                      \
            (cmd_struct_ptr)->scene_name[0]);                          \
      (cmd_struct_ptr)->scene_name[0] = 0;                             \
    }                                                                  \
  }

/** @brief Parses fieldset description
    @param buffer containing fieldset
    @param cmd_struct_ptr - pointer to the fieldset description representation
    structure (of type @ref zb_zcl_scenes_fieldset_common_s) or NULL pointer if
    payload size is too small
    @param content_length - variable to contain size of the fieldset content
    left unprocessed
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_NEXT_FIELDSET_DESC(             \
    buffer,                                                             \
    cmd_struct_ptr,                                                     \
    content_length)                                                     \
  {                                                                     \
    if (sizeof(zb_zcl_scenes_fieldset_common_t) > ZB_BUF_LEN((buffer))) \
    {                                                                   \
      (cmd_struct_ptr) = NULL;                                          \
    }                                                                   \
    else                                                                \
    {                                                                   \
      (cmd_struct_ptr) = (zb_uint8_t*)ZB_BUF_BEGIN(buffer);             \
      (content_length) = sizeof(zb_zcl_scenes_fieldset_common_t) +      \
          ( (zb_zcl_scenes_fieldset_common_t*)                          \
            (cmd_struct_ptr))->fieldset_length;                         \
      if ((content_length) > ZB_BUF_LEN((buffer)))                      \
      {                                                                 \
        (cmd_struct_ptr) = NULL;                                        \
        (content_length) = 0;                                           \
      }                                                                 \
      else                                                              \
      {                                                                 \
        /* Cludgy: decoding cluster_id in the fieldset directly */      \
        ZB_ZCL_HTOLE16_INPLACE((cmd_struct_ptr));                       \
        ZB_BUF_CUT_LEFT2((buffer), (content_length));                   \
      }                                                                 \
    }                                                                   \
  }

/** @brief Copies parsed fieldset description into designated memory area
    @param src_data_ptr - pointer to the memory area containing parsed fieldset
    description
    @param dst_data_ptr - pointer to the memory area to put fieldset description
    to
    @param content_length - remaining length of the unprocessed fieldset content
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_STORE_FIELDSET_COMMON(               \
    src_data_ptr,                                                            \
    dst_data_ptr,                                                            \
    content_length)                                                          \
  {                                                                          \
    ZB_MEMCPY(                                                               \
        (dst_data_ptr),                                                      \
        (src_data_ptr),                                                      \
        sizeof(zb_zcl_scenes_fieldset_common_t));                            \
    ((zb_zcl_scenes_fieldset_common_t*)(dst_data_ptr))->fieldset_length = 0; \
    content_length -= sizeof(zb_zcl_scenes_fieldset_common_t);               \
  }

/** @brief Copies OnOff.OnOff attribute value from Scenes.AddScene command
    payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
    @attention src_data_ptr and dst_data_ptr will be incremented by the
    OnOff.OnOff attribute size, content_length will be decremented by the same
    value
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_ON_OFF_ON_OFF(                         \
    dst_data_ptr,                                                              \
    src_data_ptr,                                                              \
    content_length,                                                            \
    dst_fieldset_length)                                                       \
  {                                                                            \
    if (content_length)                                                        \
    {                                                                          \
      ((zb_zcl_scenes_fieldset_on_off_t*)(dst_data_ptr))->specific.on_off =    \
          ((zb_zcl_scenes_fieldset_on_off_t*)(src_data_ptr))->specific.on_off; \
      ++(*dst_fieldset_length);                                                \
      --(content_length);                                                      \
    }                                                                          \
  }

/** @brief Extracts OnOff fieldset value from Scenes.AddScene command
    payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_ON_OFF_FIELDSET( \
    dst_data_ptr,                                        \
    src_data_ptr,                                        \
    content_length,                                      \
    dst_fieldset_length)                                 \
  ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_ON_OFF_ON_OFF(         \
      dst_data_ptr,                                      \
      src_data_ptr,                                      \
      content_length,                                    \
      dst_fieldset_length)

/** @brief Copies LevelControl.CurrentLevel attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_LEVEL_CONTROL_CURRENT_LEVEL(                         \
    dst_data_ptr,                                                                            \
    src_data_ptr,                                                                            \
    content_length,                                                                          \
    dst_fieldset_length)                                                                     \
  {                                                                                          \
    if (content_length)                                                                      \
    {                                                                                        \
      ((zb_zcl_scenes_fieldset_level_control_t*)(dst_data_ptr))->specific.current_level =    \
          ((zb_zcl_scenes_fieldset_level_control_t*)(src_data_ptr))->specific.current_level; \
      ++(*dst_fieldset_length);                                                              \
      --(content_length);                                                                    \
    }                                                                                        \
  }

/** @brief Extracts LevelControl fieldset value from Scenes.AddScene command
    payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_LEVEL_CONTROL_FIELDSET( \
    dst_data_ptr,                                               \
    src_data_ptr,                                               \
    content_length,                                             \
    dst_fieldset_length)                                                     \
  ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_LEVEL_CONTROL_CURRENT_LEVEL(  \
      dst_data_ptr,                                             \
      src_data_ptr,                                             \
      content_length,                                           \
      dst_fieldset_length)

/** @brief Copies ColorControl.CurrentX attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_X(                             \
    dst_data_ptr,                                                                            \
    src_data_ptr,                                                                            \
    content_length,                                                                          \
    dst_fieldset_length)                                                                     \
  {                                                                                          \
    if ((content_length) >= sizeof(zb_uint16_t))                                             \
    {                                                                                        \
      ZB_HTOLE16(                                                                            \
          &(((zb_zcl_scenes_fieldset_color_control_t*)(dst_data_ptr))->specific.current_x),  \
          &(((zb_zcl_scenes_fieldset_color_control_t*)(src_data_ptr))->specific.current_x)); \
      (content_length) -= sizeof(zb_uint16_t);                                               \
      *(dst_fieldset_length) += sizeof(zb_uint16_t);                                         \
    }                                                                                        \
  }

/** @brief Copies ColorControl.CurrentY attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_Y(                            \
    dst_data_ptr,                                                                           \
    src_data_ptr,                                                                           \
    content_length,                                                                         \
    dst_fieldset_length)                                                                    \
  {                                                                                         \
    if ((content_length) >= sizeof(zb_uint16_t))                                            \
    {                                                                                       \
      ZB_HTOLE16(                                                                           \
          &(((zb_zcl_scenes_fieldset_color_control_t*)(dst_data_ptr))->specific.current_y), \
          &(((zb_zcl_scenes_fieldset_color_control_t*)(src_data_ptr))->specific.current_y));\
      (content_length) -= sizeof(zb_uint16_t);                                              \
      *(dst_fieldset_length) += sizeof(zb_uint16_t);                                        \
    }                                                                                       \
  }

/** @brief Copies ColorControl.CurrentSaturation attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_SATURATION(                   \
    dst_data_ptr,                                                                           \
    src_data_ptr,                                                                           \
    content_length,                                                                         \
    dst_fieldset_length)                                                                    \
  {                                                                                         \
    if ((content_length) >= sizeof(zb_uint16_t))                                            \
    {                                                                                       \
      ((zb_zcl_scenes_fieldset_color_control_t*)(dst_data_ptr))->specific.current_saturation =  \
      ((zb_zcl_scenes_fieldset_color_control_t*)(src_data_ptr))->specific.current_saturation;   \
      (content_length) -= sizeof(zb_uint8_t);                                               \
      *(dst_fieldset_length) += sizeof(zb_uint8_t);                                         \
    }                                                                                       \
  }

#if defined ZB_ENABLE_ZLL

/** @brief Copies extension ColorControl.EnhancedCurrentHue attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_ENHANCED_CURRENT_HUE(                 \
    dst_data_ptr,                                                                           \
    src_data_ptr,                                                                           \
    content_length,                                                                         \
    dst_fieldset_length)                                                                    \
  {                                                                                         \
    if ((content_length) >= sizeof(zb_uint16_t))                                            \
    {                                                                                       \
      ZB_HTOLE16(                                                                           \
          &(((zb_zcl_scenes_fieldset_color_control_t*)(dst_data_ptr))->specific.enhanced_current_hue), \
          &(((zb_zcl_scenes_fieldset_color_control_t*)(src_data_ptr))->specific.enhanced_current_hue));\
      (content_length) -= sizeof(zb_uint16_t);                                              \
      *(dst_fieldset_length) += sizeof(zb_uint16_t);                                        \
    }                                                                                       \
  }

/** @brief Copies extension ColorControl.ColorLoopActive attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_COLOR_LOOP_ACTIVE(                    \
    dst_data_ptr,                                                                           \
    src_data_ptr,                                                                           \
    content_length,                                                                         \
    dst_fieldset_length)                                                                    \
  {                                                                                         \
    if ((content_length) >= sizeof(zb_uint16_t))                                            \
    {                                                                                       \
      ((zb_zcl_scenes_fieldset_color_control_t*)(dst_data_ptr))->specific.color_loop_active =  \
      ((zb_zcl_scenes_fieldset_color_control_t*)(src_data_ptr))->specific.color_loop_active;   \
      (content_length) -= sizeof(zb_uint8_t);                                               \
      *(dst_fieldset_length) += sizeof(zb_uint8_t);                                         \
    }                                                                                       \
  }

/** @brief Copies extension ColorControl.ColorLoopDirection attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_COLOR_LOOP_DIRECTION(                 \
    dst_data_ptr,                                                                           \
    src_data_ptr,                                                                           \
    content_length,                                                                         \
    dst_fieldset_length)                                                                    \
  {                                                                                         \
    if ((content_length) >= sizeof(zb_uint16_t))                                            \
    {                                                                                       \
      ((zb_zcl_scenes_fieldset_color_control_t*)(dst_data_ptr))->specific.color_loop_direction =  \
      ((zb_zcl_scenes_fieldset_color_control_t*)(src_data_ptr))->specific.color_loop_direction;   \
      (content_length) -= sizeof(zb_uint8_t);                                               \
      *(dst_fieldset_length) += sizeof(zb_uint8_t);                                         \
    }                                                                                       \
  }

/** @brief Copies extension ColorControl.ColorLoopTime attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_COLOR_LOOP_TIME(                      \
    dst_data_ptr,                                                                           \
    src_data_ptr,                                                                           \
    content_length,                                                                         \
    dst_fieldset_length)                                                                    \
  {                                                                                         \
    if ((content_length) >= sizeof(zb_uint16_t))                                            \
    {                                                                                       \
      ZB_HTOLE16(                                                                           \
          &(((zb_zcl_scenes_fieldset_color_control_t*)(dst_data_ptr))->specific.color_loop_time), \
          &(((zb_zcl_scenes_fieldset_color_control_t*)(src_data_ptr))->specific.color_loop_time));\
      (content_length) -= sizeof(zb_uint16_t);                                              \
      *(dst_fieldset_length) += sizeof(zb_uint16_t);                                        \
    }                                                                                       \
  }

#endif /* defined ZB_ENABLE_ZLL*/

/** @brief Extracts ColorControl fieldset value from Scenes.AddScene command
    payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#if defined ZB_ENABLE_ZLL
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_FIELDSET( \
    dst_data_ptr,                                               \
    src_data_ptr,                                               \
    content_length,                                             \
    dst_fieldset_length)                                        \
  {                                                             \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_X(    \
      dst_data_ptr,                                             \
      src_data_ptr,                                             \
      content_length,                                           \
      dst_fieldset_length);                                     \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_Y(    \
      dst_data_ptr,                                             \
      src_data_ptr,                                             \
      content_length,                                           \
      dst_fieldset_length);                                     \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_ENHANCED_CURRENT_HUE( \
      dst_data_ptr,                                                     \
      src_data_ptr,                                                     \
      content_length,                                                   \
      dst_fieldset_length);                                             \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_SATURATION(   \
      dst_data_ptr,                                                     \
      src_data_ptr,                                                     \
      content_length,                                                   \
      dst_fieldset_length);                                             \
      ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_COLOR_LOOP_ACTIVE(  \
      dst_data_ptr,                                                     \
      src_data_ptr,                                                     \
      content_length,                                                   \
      dst_fieldset_length);                                             \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_COLOR_LOOP_DIRECTION( \
      dst_data_ptr,                                                     \
      src_data_ptr,                                                     \
      content_length,                                                   \
      dst_fieldset_length);                                             \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_COLOR_LOOP_TIME(      \
      dst_data_ptr,                                                     \
      src_data_ptr,                                                     \
      content_length,                                                   \
      dst_fieldset_length);                                             \
  }
#else
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_FIELDSET( \
    dst_data_ptr,                                               \
    src_data_ptr,                                               \
    content_length,                                             \
    dst_fieldset_length)                                        \
  {                                                             \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_X(    \
      dst_data_ptr,                                             \
      src_data_ptr,                                             \
      content_length,                                           \
      dst_fieldset_length);                                     \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_Y(    \
      dst_data_ptr,                                             \
      src_data_ptr,                                             \
      content_length,                                           \
      dst_fieldset_length);                                     \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COLOR_CONTROL_CURRENT_SATURATION(   \
      dst_data_ptr,                                                     \
      src_data_ptr,                                                     \
      content_length,                                                   \
      dst_fieldset_length);                                             \
  }
#endif

/** @brief Copies DoorLock.LockState attribute value from Scenes.AddScene
    command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_DOOR_LOCK_LOCK_STATE(                         \
    dst_data_ptr,                                                                     \
    src_data_ptr,                                                                     \
    content_length,                                                                   \
    dst_fieldset_length)                                                              \
  {                                                                                   \
    if (content_length)                                                               \
    {                                                                                 \
      ((zb_zcl_scenes_fieldset_door_lock_t*)(dst_data_ptr))->specific.lock_state =    \
          ((zb_zcl_scenes_fieldset_door_lock_t*)(src_data_ptr))->specific.lock_state; \
      --(content_length);                                                             \
      ++(*(dst_fieldset_length));                                                     \
    }                                                                                 \
  }

/** @brief Extracts DoorLock fieldset value from Scenes.AddScene command
    payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_DOOR_LOCK_FIELDSET( \
    dst_data_ptr,                                           \
    src_data_ptr,                                           \
    content_length,                                         \
    dst_fieldset_length)                                    \
  ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_DOOR_LOCK_LOCK_STATE(     \
      dst_data_ptr,                                         \
      src_data_ptr,                                         \
      content_length,                                       \
      dst_fieldset_length)

/** @brief Copies Thermostat.OccupiedCoolingSetpoint attribute value from
    Scenes.AddScene command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_THERMOSTAT_OCCUPIED_COOLING_SETPOINT( \
    dst_data_ptr,                                                             \
    src_data_ptr,                                                             \
    content_length,                                                           \
    dst_fieldset_length)                                                      \
  {                                                                           \
    if ((content_length) >= sizeof(zb_uint16_t))                              \
    {                                                                         \
      ZB_HTOLE16(                                                             \
        &(((zb_zcl_scenes_fieldset_thermostat_t*)(dst_data_ptr))->            \
              specific.occupied_cooling_setpoint),                            \
        &(((zb_zcl_scenes_fieldset_thermostat_t*)(src_data_ptr))->            \
              specific.occupied_cooling_setpoint));                           \
      (content_length) -= sizeof(zb_uint16_t);                                \
      *(dst_fieldset_length) += sizeof(zb_uint16_t);                          \
    }                                                                         \
  }

/** @brief Copies Thermostat.OccupiedHeatingSetpoint attribute value from
    Scenes.AddScene command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_THERMOSTAT_OCCUPIED_HEATING_SETPOINT( \
    dst_data_ptr,                                                             \
    src_data_ptr,                                                             \
    content_length,                                                           \
    dst_fieldset_length)                                                      \
  {                                                                           \
    if ((content_length) >= sizeof(zb_uint16_t))                              \
    {                                                                         \
      ZB_HTOLE16(                                                             \
        &(((zb_zcl_scenes_fieldset_thermostat_t*)(dst_data_ptr))->            \
              specific.occupied_heating_setpoint),                            \
        &(((zb_zcl_scenes_fieldset_thermostat_t*)(src_data_ptr))->            \
              specific.occupied_heating_setpoint));                           \
      (content_length) -= sizeof(zb_uint16_t);                                \
      *(dst_fieldset_length) += sizeof(zb_uint16_t);                          \
    }                                                                         \
  }

/** @brief Copies Thermostat.SystemMode attribute value from
    Scenes.AddScene command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_THERMOSTAT_SYSTEM_MODE(                         \
    dst_data_ptr,                                                                       \
    src_data_ptr,                                                                       \
    content_length,                                                                     \
    dst_fieldset_length)                                                                \
  {                                                                                     \
    if (content_length)                                                                 \
    {                                                                                   \
      ((zb_zcl_scenes_fieldset_thermostat_t*)(dst_data_ptr))->specific.system_mode =    \
          ((zb_zcl_scenes_fieldset_thermostat_t*)(src_data_ptr))->specific.system_mode; \
      --(content_length);                                                               \
      ++(*(dst_fieldset_length));                                                       \
    }                                                                                   \
  }

/** @brief Extracts Thermostat fieldset value from Scenes.AddScene command
    payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_THERMOSTAT_FIELDSET(              \
    dst_data_ptr,                                                         \
    src_data_ptr,                                                         \
    content_length,                                                       \
    dst_fieldset_length)                                                  \
  {                                                                       \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_THERMOSTAT_OCCUPIED_COOLING_SETPOINT( \
      dst_data_ptr,                                                       \
      src_data_ptr,                                                       \
      content_length,                                                     \
      dst_fieldset_length);                                               \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_THERMOSTAT_OCCUPIED_HEATING_SETPOINT( \
      dst_data_ptr,                                                       \
      src_data_ptr,                                                       \
      content_length,                                                     \
      dst_fieldset_length);                                               \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_THERMOSTAT_SYSTEM_MODE(               \
      dst_data_ptr,                                                       \
      src_data_ptr,                                                       \
      content_length,                                                     \
      dst_fieldset_length);                                               \
  }

/** @brief Copies WindowCovering.LiftPercentage attribute value from
    Scenes.AddScene command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_WINDOW_COVERING_LIFT_PERCENTAGE(    \
    dst_data_ptr,                                                           \
    src_data_ptr,                                                           \
    content_length,                                                         \
    dst_fieldset_length)                                                    \
  {                                                                         \
    if (content_length)                                                     \
    {                                                                       \
      ((zb_zcl_scenes_fieldset_window_covering_t*)(dst_data_ptr))->         \
          specific.current_position_lift_percentage =                       \
              ((zb_zcl_scenes_fieldset_window_covering_t*)(src_data_ptr))-> \
                  specific.current_position_lift_percentage;                \
      --(content_length);                                                   \
      ++(*(dst_fieldset_length));                                           \
    }                                                                       \
  }

/** @brief Copies WindowCovering.TiltPercentage attribute value from
    Scenes.AddScene command payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_WINDOW_COVERING_TILT_PERCENTAGE(    \
    dst_data_ptr,                                                           \
    src_data_ptr,                                                           \
    content_length,                                                         \
    dst_fieldset_length)                                                    \
  {                                                                         \
    if (content_length)                                                     \
    {                                                                       \
      ((zb_zcl_scenes_fieldset_window_covering_t*)(dst_data_ptr))->         \
          specific.current_position_tilt_percentage =                       \
              ((zb_zcl_scenes_fieldset_window_covering_t*)(src_data_ptr))-> \
                  specific.current_position_tilt_percentage;                \
      ++(*(dst_fieldset_length));                                           \
      --(content_length);                                                   \
    }                                                                       \
  }

/** @brief Extracts WindowCovering fieldset value from Scenes.AddScene command
    payload into the specified point in memory.
    @param dst_data_ptr - address to put value to
    @param src_data_ptr - address to get value from
    @param content_length - length of the content wsiting processing
    @param dst_fieldset_length - pointer to the length of the destination
    fieldset
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_WINDOW_COVERING_FIELDSET(    \
    dst_data_ptr,                                                    \
    src_data_ptr,                                                    \
    content_length,                                                  \
    dst_fieldset_length)                                             \
  {                                                                  \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_WINDOW_COVERING_LIFT_PERCENTAGE( \
      dst_data_ptr,                                                  \
      src_data_ptr,                                                  \
      content_length,                                                \
      dst_fieldset_length)                                           \
    ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_WINDOW_COVERING_TILT_PERCENTAGE( \
      dst_data_ptr,                                                  \
      src_data_ptr,                                                  \
      content_length,                                                \
      dst_fieldset_length)                                           \
  }

/*! @brief View scene command payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_view_scene_req_s
{
  zb_uint16_t group_id; /*!< Scene group identifier */
  zb_uint8_t scene_id;  /*!< Scene identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_view_scene_req_t;

/*! @brief Send View Scene command

    @note According to a 3.7.2.4.2. subclause of ZCL spec, the command shall be
    addressed to a single device (not a group).
    The command can be sent to device or group of devices either
    @param buffer to put packet to
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - enable/disable default response
    @param callback for getting command send status
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_VIEW_SCENE_REQ(               \
    buffer,                                              \
    dst_addr,                                            \
    dst_ep,                                              \
    ep,                                                  \
    prof_id,                                             \
    dis_default_resp,                                    \
    callback,                                            \
    group_id,                                            \
    scene_id)                                            \
  {                                                      \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)      \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL( \
        ptr,                                             \
        dis_default_resp)                               \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                     \
        ptr,                                             \
        ZB_ZCL_GET_SEQ_NUM(),                            \
        ZB_ZCL_CMD_SCENES_VIEW_SCENE);                   \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));       \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));            \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                   \
    ZB_ZCL_SEND_COMMAND_SHORT(                           \
        buffer,                                          \
        dst_addr,                                        \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,                \
        dst_ep,                                          \
        ep,                                              \
        prof_id,                                         \
        ZB_ZCL_CLUSTER_ID_SCENES,                        \
        callback);                                       \
  }

/** @brief Parses Scenes.ViewScene command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_view_scene_req_s) or NULL pointer if payload size is
    too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_VIEW_SCENE_REQ(buffer, cmd_struct_ptr)       \
  {                                                                    \
    if (sizeof(zb_zcl_scenes_view_scene_req_t) > ZB_BUF_LEN((buffer))) \
    {                                                                  \
      (cmd_struct_ptr) = NULL;                                         \
    }                                                                  \
    else                                                               \
    {                                                                  \
      (cmd_struct_ptr) =                                               \
        (zb_zcl_scenes_view_scene_req_t*)ZB_BUF_BEGIN(buffer);         \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);             \
    }                                                                  \
  }

/*! @brief Remove scene command payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_remove_scene_req_s
{
  zb_uint16_t group_id; /*!< Scene group identifier */
  zb_uint8_t scene_id;  /*!< Scene identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_remove_scene_req_t;

/*! @brief Send Remove scene command

    The command can be sent to device or group of devices either
    @param buffer to put packet to
    @param dst_addr - address to send packet to
    @param dst_addr_mode - addressing mode. Either @ref
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT or @ref
    ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT will fit
    @param dst_ep - destination endpoint (if sendting to a particular device)
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - enable/disable default response
    @param callback for getting command send status
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_REMOVE_SCENE_REQ(             \
    buffer,                                              \
    dst_addr,                                            \
    dst_addr_mode,                                       \
    dst_ep,                                              \
    ep,                                                  \
    prof_id,                                             \
    dis_default_resp,                                    \
    callback,                                            \
    group_id,                                            \
    scene_id)                                            \
  {                                                      \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer);       \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL( \
        ptr,                                             \
        dis_default_resp)                               \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                     \
        ptr,                                             \
        ZB_ZCL_GET_SEQ_NUM(),                            \
        ZB_ZCL_CMD_SCENES_REMOVE_SCENE);                 \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));       \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));            \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                   \
    ZB_ZCL_SEND_COMMAND_SHORT(                           \
        buffer,                                          \
        dst_addr,                                        \
        dst_addr_mode,                                   \
        dst_ep,                                          \
        ep,                                              \
        prof_id,                                         \
        ZB_ZCL_CLUSTER_ID_SCENES,                        \
        callback);                                       \
  }

/** @brief Parses Scenes.RemoveScene command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_remove_scene_req_s) or NULL pointer if payload size
    is too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_REMOVE_SCENE_REQ(buffer, cmd_struct_ptr)       \
  {                                                                      \
    if (sizeof(zb_zcl_scenes_remove_scene_req_t) > ZB_BUF_LEN((buffer))) \
    {                                                                    \
      (cmd_struct_ptr) = NULL;                                           \
    }                                                                    \
    else                                                                 \
    {                                                                    \
      (cmd_struct_ptr) =                                                 \
        (zb_zcl_scenes_remove_scene_req_t*)ZB_BUF_BEGIN(buffer);         \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);               \
    }                                                                    \
  }

/*! @brief Remove all scenes command payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_remove_all_scenes_req_s
{
  zb_uint16_t group_id; /*!< Scene group identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_remove_all_scenes_req_t;

/*! @brief Send Remove all scenes command

    The command can be sent to device or group of devices either
    @param buffer to put packet to
    @param dst_addr - address to send packet to
    @param dst_addr_mode - addressing mode. Either @ref
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT or @ref
    ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT will fit
    @param dst_ep - destination endpoint (if sendting to a particular device)
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - enable/disable default response
    @param callback for getting command send status
    @param group_id - group identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_REMOVE_ALL_SCENES_REQ(        \
    buffer,                                              \
    dst_addr,                                            \
    dst_addr_mode,                                       \
    dst_ep,                                              \
    ep,                                                  \
    prof_id,                                             \
    dis_default_resp,                                    \
    callback,                                            \
    group_id)                                            \
  {                                                      \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)       \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL( \
        ptr,                                             \
        dis_default_resp)                               \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                     \
        ptr,                                             \
        ZB_ZCL_GET_SEQ_NUM(),                            \
        ZB_ZCL_CMD_SCENES_REMOVE_ALL_SCENES);            \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));       \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                   \
    ZB_ZCL_SEND_COMMAND_SHORT(                           \
        buffer,                                          \
        dst_addr,                                        \
        dst_addr_mode,                                   \
        dst_ep,                                          \
        ep,                                              \
        prof_id,                                         \
        ZB_ZCL_CLUSTER_ID_SCENES,                        \
        callback);                                       \
  }

/** @brief Parses Scenes.RemoveScene command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_remove_scene_req_s) or NULL pointer if payload size
    is too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_REMOVE_ALL_SCENES_REQ(buffer, cmd_struct_ptr) \
  {                                                                     \
    if (  sizeof(zb_zcl_scenes_remove_all_scenes_req_t)                 \
        > ZB_BUF_LEN((buffer)))                                         \
    {                                                                   \
      (cmd_struct_ptr) = NULL;                                          \
    }                                                                   \
    else                                                                \
    {                                                                   \
      (cmd_struct_ptr) =                                                \
        (zb_zcl_scenes_remove_all_scenes_req_t*)ZB_BUF_BEGIN(buffer);   \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);              \
    }                                                                   \
  }

/*! @brief Store scene command payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_store_scene_req_s
{
  zb_uint16_t group_id; /*!< Group identifier */
  zb_uint8_t scene_id;  /*!< Scene identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_store_scene_req_t;

/*! @brief Send Store Scene command

    The command can be sent to device or group of devices either
    @param buffer to put packet to
    @param dst_addr - address to send packet to
    @param dst_addr_mode - addressing mode. Either @ref
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT or @ref
    ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT will fit
    @param dst_ep - destination endpoint (if sendting to a particular device)
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - enable/disable default response
    @param callback for getting command send status
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_STORE_SCENE_REQ(              \
    buffer,                                              \
    dst_addr,                                            \
    dst_addr_mode,                                       \
    dst_ep,                                              \
    ep,                                                  \
    prof_id,                                             \
    dis_default_resp,                                    \
    callback,                                            \
    group_id,                                            \
    scene_id)                                            \
  {                                                      \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)       \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL( \
        ptr,                                             \
        dis_default_resp)                               \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                     \
        ptr,                                             \
        ZB_ZCL_GET_SEQ_NUM(),                            \
        ZB_ZCL_CMD_SCENES_STORE_SCENE);                  \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));       \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));            \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                   \
    ZB_ZCL_SEND_COMMAND_SHORT(                           \
        buffer,                                          \
        dst_addr,                                        \
        dst_addr_mode,                                   \
        dst_ep,                                          \
        ep,                                              \
        prof_id,                                         \
        ZB_ZCL_CLUSTER_ID_SCENES,                        \
        callback);                                       \
  }

/** @brief Parse Store Scene command
    @param buffer containing Store scene command payload
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_store_scene_req_s) or NULL pointer if payload size
    is too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_STORE_SCENE_REQ(buffer, cmd_struct_ptr)        \
  {                                                                      \
    if (sizeof(zb_zcl_scenes_store_scene_req_t) != ZB_BUF_LEN((buffer))) \
    {                                                                    \
      (cmd_struct_ptr) = NULL;                                           \
    }                                                                    \
    else                                                                 \
    {                                                                    \
      ZB_ZCL_HTOLE16_INPLACE(ZB_BUF_BEGIN((buffer)));                    \
      (cmd_struct_ptr) =                                                 \
        (zb_zcl_scenes_store_scene_req_t*)ZB_BUF_BEGIN(buffer);          \
      ZB_BUF_CUT_LEFT2(                                                  \
          (buffer),                                                      \
          sizeof(zb_zcl_scenes_store_scene_req_t));                      \
    }                                                                    \
  }

/*! @brief Recall scene command payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_recall_scene_req_s
{
  zb_uint16_t group_id; /*!< Scene group identifier */
  zb_uint8_t scene_id;  /*!< Scene identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_recall_scene_req_t;

/*! @brief Send Recall scene command

    The command can be sent to device or group of devices either
    @param buffer to put packet to
    @param dst_addr - address to send packet to
    @param dst_addr_mode - addressing mode. Either @ref
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT or @ref
    ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT will fit
    @param dst_ep - destination endpoint (if sendting to a particular device)
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - enable/disable default response
    @param callback for getting command send status
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_RECALL_SCENE_REQ(            \
    buffer,                                             \
    dst_addr,                                           \
    dst_addr_mode,                                      \
    dst_ep,                                             \
    ep,                                                 \
    prof_id,                                            \
    dis_default_resp,                                   \
    callback,                                           \
    group_id,                                           \
    scene_id)                                           \
  {                                                     \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)      \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL \
    (ptr                                                \
     , dis_default_resp)                               \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                    \
        ptr,                                            \
        ZB_ZCL_GET_SEQ_NUM(),                           \
        ZB_ZCL_CMD_SCENES_RECALL_SCENE);                \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));      \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));           \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                  \
    ZB_ZCL_SEND_COMMAND_SHORT(                          \
        buffer,                                         \
        dst_addr,                                       \
        dst_addr_mode,                                  \
        dst_ep,                                         \
        ep,                                             \
        prof_id,                                        \
        ZB_ZCL_CLUSTER_ID_SCENES,                       \
        callback);                                      \
  }

/** @brief Parse Recall Scene command
    @param buffer containing Store scene command payload
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_recall_scene_req_s) or NULL pointer if payload size
    is too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_RECALL_SCENE_REQ(buffer, cmd_struct_ptr)        \
  {                                                                       \
    if (sizeof(zb_zcl_scenes_recall_scene_req_t) != ZB_BUF_LEN((buffer))) \
    {                                                                     \
      (cmd_struct_ptr) = NULL;                                            \
    }                                                                     \
    else                                                                  \
    {                                                                     \
      ZB_ZCL_HTOLE16_INPLACE(ZB_BUF_BEGIN((buffer)));                     \
      (cmd_struct_ptr) =                                                  \
        (zb_zcl_scenes_recall_scene_req_t*)ZB_BUF_BEGIN(buffer);          \
      ZB_BUF_CUT_LEFT2(                                                   \
          (buffer),                                                       \
          sizeof(zb_zcl_scenes_recall_scene_req_t));                      \
    }                                                                     \
  }

/*! @brief Get scene membership command payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_get_scene_membership_req_s
{
  zb_uint16_t group_id; /*!< Scene group identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_get_scene_membership_req_t;

/*! @brief Send Remove all scenes command

    The command can be sent to device or group of devices either
    @param buffer to put packet to
    @param dst_addr - address to send packet to
    @param dst_addr_mode - addressing mode. Either @ref
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT or @ref
    ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT will fit
    @param dst_ep - destination endpoint (if sendting to a particular device)
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param dis_default_resp - enable/disable default response
    @param callback for getting command send status
    @param group_id - group identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_GET_SCENE_MEMBERSHIP_REQ(     \
    buffer,                                              \
    dst_addr,                                            \
    dst_addr_mode,                                       \
    dst_ep,                                              \
    ep,                                                  \
    prof_id,                                             \
    dis_default_resp,                                    \
    callback,                                            \
    group_id)                                            \
  {                                                      \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)       \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL( \
        ptr,                                             \
        dis_default_resp)                               \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(                     \
        ptr,                                             \
        ZB_ZCL_GET_SEQ_NUM(),                            \
        ZB_ZCL_CMD_SCENES_GET_SCENE_MEMBERSHIP);         \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));       \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                   \
    ZB_ZCL_SEND_COMMAND_SHORT(                           \
        buffer,                                          \
        dst_addr,                                        \
        dst_addr_mode,                                   \
        dst_ep,                                          \
        ep,                                              \
        prof_id,                                         \
        ZB_ZCL_CLUSTER_ID_SCENES,                        \
        callback);                                       \
  }

/** @brief Parses Scenes.GetSceneMembership command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_get_scene_membership_req_s) or NULL pointer if payload size
    is too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_GET_SCENE_MEMBERSHIP_REQ(buffer, cmd_struct_ptr) \
  {                                                                        \
    if (  sizeof(zb_zcl_scenes_get_scene_membership_req_t)                 \
        > ZB_BUF_LEN((buffer)))                                            \
    {                                                                      \
      (cmd_struct_ptr) = NULL;                                             \
    }                                                                      \
    else                                                                   \
    {                                                                      \
      (cmd_struct_ptr) =                                                   \
        (zb_zcl_scenes_get_scene_membership_req_t*)ZB_BUF_BEGIN(buffer);   \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);                 \
    }                                                                      \
  }

/*! @brief Add scene response payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_add_scene_res_s
{
  zb_uint8_t status;    /*!< Add scene status */
  zb_uint16_t group_id; /*!< Scene group identifier */
  zb_uint8_t scene_id;  /*!< Scene identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_add_scene_res_t;

/*! @brief Send Add Scene response

    @param buffer to put packet to
    @param seq_num - ZCL sequence number
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param callback for getting command send status
    @param status of the corresponding Add scene command execution. Appropriate
    values (defined in @ref zb_zcl_status_e enumeration)are:
    @li @ref ZB_ZCL_STATUS_SUCCESS for successfull result
    @li @ref ZB_ZCL_STATUS_INSUFF_SPACE if the scene table is full
    @li @ref ZB_ZCL_STATUS_INVALID_FIELD if the group is not present in the
    Group Table
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_ADD_SCENE_RES(                     \
    buffer,                                                   \
    seq_num,                                                  \
    dst_addr,                                                 \
    dst_ep,                                                   \
    ep,                                                       \
    prof_id,                                                  \
    callback,                                                 \
    status,                                                   \
    group_id,                                                 \
    scene_id)                                                 \
  {                                                           \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);            \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr); \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER(                          \
        ptr,                                                  \
        seq_num,                                              \
        ZB_ZCL_CMD_SCENES_ADD_SCENE_RESPONSE);                \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (status));                   \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));            \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));                 \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                        \
    ZB_ZCL_SEND_COMMAND_SHORT(                                \
        buffer,                                               \
        dst_addr,                                             \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,                     \
        dst_ep,                                               \
        ep,                                                   \
        prof_id,                                              \
        ZB_ZCL_CLUSTER_ID_SCENES,                             \
        callback);                                            \
  }

/** @brief Parses Scenes.AddSceneResponse command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_add_scene_res_s) or NULL pointer if payload size is
    too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_ADD_SCENE_RES(buffer, cmd_struct_ptr)       \
  {                                                                   \
    if (sizeof(zb_zcl_scenes_add_scene_res_t) > ZB_BUF_LEN((buffer))) \
    {                                                                 \
      (cmd_struct_ptr) = NULL;                                        \
    }                                                                 \
    else                                                              \
    {                                                                 \
      (cmd_struct_ptr) =                                              \
        (zb_zcl_scenes_add_scene_res_t*)ZB_BUF_BEGIN(buffer);         \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);            \
    }                                                                 \
  }

/** @brief Mandatory part of the Scenes.ViewSceneResponse payload */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_view_scene_res_fixed_size_s
{
  zb_uint8_t status;    /*!< Command execution status */
  zb_uint16_t group_id; /*!< Scene group identifier */
  zb_uint8_t scene_id;  /*!< Scene identifier */
  /** @brief Scene transition time
      @attention Valid if status is @ref ZB_ZCL_STATUS_SUCCESS only
  */
  zb_uint16_t transition_time;
  /** @brief Scene name field.

      Since scene names are not supported, will contain only zero length byte on
      formation. If received packet with scene name filled from some other
      stack, the fields will be skipped during packet parsing, and length byte
      will be set to zero.
      @attention Valid if status is @ref ZB_ZCL_STATUS_SUCCESS only
  */
  zb_uint8_t scene_name[1];
} ZB_PACKED_STRUCT zb_zcl_scenes_view_scene_res_fixed_size_t;

/** @brief Optional part of the Scenes.ViewSceneResponse payload */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_view_scene_res_variable_size_s
{
  zb_uint8_t fieldset_begin[1]; /**< Pointer to the start of fieldsets */
} ZB_PACKED_STRUCT zb_zcl_scenes_view_scene_res_variable_size_t;

/*! @brief View scene response payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_view_scene_res_s
{
  /** @brief Mandatory part of the payload */
  zb_zcl_scenes_view_scene_res_fixed_size_t mandatory;
  /** @brief Optional part of the payload.

      Filled if and only if status field in the mandatory part contains
      ZB_ZCL_STATUS_SUCCESS value.
  */
  zb_zcl_scenes_view_scene_res_variable_size_t optional;
} ZB_PACKED_STRUCT zb_zcl_scenes_view_scene_res_t;

/*! @brief Start filling View scene response frame

    Fills the mandatory part of the View scene response payload.

    @param buffer to put packet to
    @param cmd_struct_ptr - pointer to the place variable-structure part of the
    command payload (fieldsets) should be placed.
    @param seq_num - ZCL sequence number
    @param status of the command execution. Appropriate values (defined in @ref
    zb_zcl_status_e enumeration) are:
    @li ZB_ZCL_STATUS_SUCCESS for successfull result
    @li ZB_ZCL_STATUS_INSUFF_SPACE if fieldsets don't fit into packet
    @li ZB_ZCL_STATUS_INVALID_FIELD if the group is not present in the
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
*/
#define ZB_ZCL_SCENES_INIT_VIEW_SCENE_RES(                    \
    buffer,                                                   \
    cmd_struct_ptr,                                           \
    seq_num,                                                  \
    status,                                                   \
    group_id,                                                 \
    scene_id)                                                 \
  {                                                           \
    cmd_struct_ptr = ZB_ZCL_START_PACKET(buffer);             \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(      \
        cmd_struct_ptr);                                      \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER(                          \
        cmd_struct_ptr,                                       \
        seq_num,                                              \
        ZB_ZCL_CMD_SCENES_VIEW_SCENE_RESPONSE);               \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, (status));        \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_struct_ptr, (group_id)); \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, (scene_id));      \
  }

/*! @brief Sends View scene response

    @param buffer with prepared command payload
    @param cmd_struct_ptr - pointer to the place variable-structure part of the
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param callback for getting command send status
*/
#define ZB_ZCL_SCENES_SEND_VIEW_SCENE_RES(        \
    buffer,                                       \
    cmd_struct_ptr,                               \
    dst_addr,                                     \
    dst_ep,                                       \
    ep,                                           \
    prof_id,                                      \
    callback)                                     \
  {                                               \
    ZB_ZCL_FINISH_PACKET(buffer, cmd_struct_ptr) \
    ZB_ZCL_SEND_COMMAND_SHORT(                    \
        buffer,                                   \
        dst_addr,                                 \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,         \
        dst_ep,                                   \
        ep,                                       \
        prof_id,                                  \
        ZB_ZCL_CLUSTER_ID_SCENES,                 \
        callback);                                \
  }

/** @brief Adds transition time into Scenes.ViewSceneResponse payload.
    @param cmd_struct_ptr - pointer to the place in buffer to put data to
    @param trans_time - transition time to put
*/
#define ZB_ZCL_SCENES_ADD_TRANSITION_TIME_VIEW_SCENE_RES(     \
    cmd_struct_ptr,                                           \
    trans_time)                                               \
    ZB_ZCL_PACKET_PUT_DATA16((cmd_struct_ptr), &(trans_time))

/** @brief Adds scene name into Scenes.ViewSceneResponse payload.

    Since we don't support scene names, puts  only zero scene name length byte
    to preserve payload format.
    @param cmd_struct_ptr - pointer to the place in buffer to put data to
    @param scene_name - scene name to put (currently ignored)
*/
#define ZB_ZCL_SCENES_ADD_SCENE_NAME_VIEW_SCENE_RES( \
    cmd_struct_ptr,                                  \
    scene_name)                                      \
    ZB_ZCL_PACKET_PUT_DATA8((cmd_struct_ptr), 0)

/* TODO Fix possible issue with buffer length */

/** @brief Parses comon part of the Scenes.ViewSceneResponse
    @param buffer containing data
    @param response_ptr - pointer to the extracted data
*/
#define ZB_ZCL_SCENES_GET_VIEW_SCENE_RES_COMMON(buffer, response_ptr)                  \
  {                                                                                    \
    if (ZB_BUF_LEN((buffer)) <                                                         \
        (2 * sizeof(zb_uint8_t) + sizeof(zb_uint16_t)))                                \
    {                                                                                  \
      (response_ptr) = NULL;                                                           \
    }                                                                                  \
    else                                                                               \
    {                                                                                  \
      (response_ptr) =                                                                 \
          (zb_zcl_scenes_view_scene_res_fixed_size_t*)                                 \
          ZB_BUF_BEGIN((buffer));                                                      \
      ZB_ZCL_HTOLE16_INPLACE(&((response_ptr)->group_id));                             \
      if ((response_ptr)->status == ZB_ZCL_STATUS_SUCCESS)                             \
      {                                                                                \
        ZB_ZCL_HTOLE16_INPLACE(&((response_ptr)->transition_time));                    \
        ZB_BUF_CUT_LEFT2((buffer), sizeof(zb_zcl_scenes_view_scene_res_fixed_size_t) + \
            (response_ptr)->scene_name[0]);                                            \
        (response_ptr)->scene_name[0] = 0;                                             \
      }                                                                                \
      else                                                                             \
      {                                                                                \
        ZB_BUF_CUT_LEFT2(                                                              \
            (buffer),                                                                  \
            (2 * sizeof(zb_uint8_t) + sizeof(zb_uint16_t)));                           \
      }                                                                                \
    }                                                                                  \
  }

typedef ZB_PACKED_PRE struct zb_zcl_scenes_fieldset_view_scene_res_s
{
  zb_zcl_scenes_fieldset_common_t common;
  ZB_PACKED_PRE union
  {
    zb_zcl_scenes_on_off_fieldset_content_t on_off;
    zb_zcl_scenes_level_control_fieldset_content_t level_control;
    zb_zcl_scenes_color_control_fieldset_content_t color_control;
    zb_zcl_scenes_door_lock_fieldset_content_t door_lock;
    zb_zcl_scenes_thermostat_fieldset_content_t thermostat;
    zb_zcl_scenes_window_covering_fieldset_content_t window_covering;
  } fieldset;
} ZB_PACKED_STRUCT zb_zcl_scenes_fieldset_view_scene_res_t;

#define ZB_ZCL_SCENES_GET_NEXT_FIELDSET_VIEW_SCENE_RES(buffer, fieldset_ptr)           \
  {                                                                                    \
    zb_uint16_t tmp_16bit;                                                             \
    (fieldset_ptr) =                                                                   \
      (zb_zcl_scenes_fieldset_view_scene_res_t*)ZB_BUF_BEGIN((buffer));                \
    ZB_ZCL_HTOLE16_INPLACE(&((fieldset_ptr)->common.cluster_id));                      \
    ZB_BUF_CUT_LEFT2(                                                                  \
        (buffer),                                                                      \
        sizeof(zb_zcl_scenes_fieldset_common_t) +                                      \
          fieldset_ptr->common.fieldset_length);                                       \
    ZB_ASSIGN_UINT16(&tmp_16bit, &((fieldset_ptr)->common.cluster_id));                \
    switch (tmp_16bit)                                                                 \
    {                                                                                  \
      case ZB_ZCL_CLUSTER_ID_COLOR_CONTROL:                                            \
      {                                                                                \
        if ((fieldset_ptr)->common.fieldset_length >= sizeof(zb_uint16_t))             \
        {                                                                              \
          ZB_ZCL_HTOLE16_INPLACE(&((fieldset_ptr)->fieldset.color_control.current_x)); \
        }                                                                              \
        if ((fieldset_ptr)->common.fieldset_length >= 2 * sizeof(zb_uint16_t))         \
        {                                                                              \
          ZB_ZCL_HTOLE16_INPLACE(&((fieldset_ptr)->fieldset.color_control.current_y)); \
        }                                                                              \
      }                                                                                \
        break;                                                                         \
      case ZB_ZCL_CLUSTER_ID_THERMOSTAT:                                               \
      {                                                                                \
        if ((fieldset_ptr)->common.fieldset_length >= sizeof(zb_uint16_t))             \
        {                                                                              \
          ZB_ZCL_HTOLE16_INPLACE(                                                      \
              &((fieldset_ptr)->fieldset.thermostat.occupied_cooling_setpoint));       \
        }                                                                              \
        if ((fieldset_ptr)->common.fieldset_length >= 2 * sizeof(zb_uint16_t))         \
        {                                                                              \
          ZB_ZCL_HTOLE16_INPLACE(                                                      \
              &((fieldset_ptr)->fieldset.thermostat.occupied_heating_setpoint));       \
        }                                                                              \
      }                                                                                \
        break;                                                                         \
      default:                                                                         \
        /* Simply formal */                                                            \
        break;                                                                         \
    }                                                                                  \
  }

/*! @brief Remove scene response payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_remove_scene_res_s
{
  zb_uint8_t status;    /** Command execution status */
  zb_uint16_t group_id; /** Scene group identifier */
  zb_uint8_t scene_id;  /** Scene identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_remove_scene_res_t;

/*! @brief Send Remove Scene response

    @param buffer to put packet to
    @param seq_num - ZCL sequence number
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param callback for getting command send status
    @param status of the corresponding Add scene command execution. Appropriate
    values (defined in @ref zb_zcl_status_e enumeration)are:
    @li @ref ZB_ZCL_STATUS_SUCCESS for successfull result
    @li @ref ZB_ZCL_STATUS_NOT_FOUND if the scene is not present in the scene table
    @li @ref ZB_ZCL_STATUS_INVALID_FIELD if the group is not present in the
    Group Table
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_REMOVE_SCENE_RES(                  \
    buffer,                                                   \
    seq_num,                                                  \
    dst_addr,                                                 \
    dst_ep,                                                   \
    ep,                                                       \
    prof_id,                                                  \
    callback,                                                 \
    status,                                                   \
    group_id,                                                 \
    scene_id)                                                 \
  {                                                           \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);            \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr); \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER(                          \
        ptr,                                                  \
        seq_num,                                              \
        ZB_ZCL_CMD_SCENES_REMOVE_SCENE_RESPONSE);             \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (status));                   \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));            \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));                 \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                        \
    ZB_ZCL_SEND_COMMAND_SHORT(                                \
        buffer,                                               \
        dst_addr,                                             \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,                     \
        dst_ep,                                               \
        ep,                                                   \
        prof_id,                                              \
        ZB_ZCL_CLUSTER_ID_SCENES,                             \
        callback);                                            \
  }

/** @brief Parses Scenes.RemoveSceneResponse command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_remove_scene_res_s) or NULL pointer if payload
    size is too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_REMOVE_SCENE_RES(buffer, cmd_struct_ptr)       \
  {                                                                      \
    if (sizeof(zb_zcl_scenes_remove_scene_res_t) > ZB_BUF_LEN((buffer))) \
    {                                                                    \
      (cmd_struct_ptr) = NULL;                                           \
    }                                                                    \
    else                                                                 \
    {                                                                    \
      (cmd_struct_ptr) =                                                 \
        (zb_zcl_scenes_remove_scene_res_t*)ZB_BUF_BEGIN(buffer);         \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);               \
    }                                                                    \
  }

/** @brief Remove all scenes response payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_remove_all_scenes_res_s
{
  zb_uint8_t status;    /** Command execution status */
  zb_uint16_t group_id; /** Group identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_remove_all_scenes_res_t;

/*! @brief Send Remove all scenes response

    @param buffer to put packet to
    @param seq_num - ZCL sequence number
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param callback for getting command send status
    @param status of the corresponding Add scene command execution. Appropriate
    values (defined in @ref zb_zcl_status_e enumeration)are:
    @li @ref ZB_ZCL_STATUS_SUCCESS for successfull result
    @li @ref ZB_ZCL_STATUS_INVALID_FIELD if the group is not present in the
    Group Table
    @param group_id - group identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_REMOVE_ALL_SCENES_RES(             \
    buffer,                                                   \
    seq_num,                                                  \
    dst_addr,                                                 \
    dst_ep,                                                   \
    ep,                                                       \
    prof_id,                                                  \
    callback,                                                 \
    status,                                                   \
    group_id)                                                 \
  {                                                           \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);            \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr); \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER(                          \
        ptr,                                                  \
        seq_num,                                              \
        ZB_ZCL_CMD_SCENES_REMOVE_ALL_SCENES_REPONSE);         \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (status));                   \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));            \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                        \
    ZB_ZCL_SEND_COMMAND_SHORT(                                \
        buffer,                                               \
        dst_addr,                                             \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,                     \
        dst_ep,                                               \
        ep,                                                   \
        prof_id,                                              \
        ZB_ZCL_CLUSTER_ID_SCENES,                             \
        callback);                                            \
  }

/** @brief Parses Scenes.RemoveAllScenesResponse command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_remove_all_scenes_res_s) or NULL pointer if payload
    size is too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_REMOVE_ALL_SCENES_RES(buffer, cmd_struct_ptr) \
  {                                                                     \
    if (  sizeof(zb_zcl_scenes_remove_all_scenes_res_t)                 \
        > ZB_BUF_LEN((buffer)))                                         \
    {                                                                   \
      (cmd_struct_ptr) = NULL;                                          \
    }                                                                   \
    else                                                                \
    {                                                                   \
      (cmd_struct_ptr) =                                                \
        (zb_zcl_scenes_remove_all_scenes_res_t*)ZB_BUF_BEGIN(buffer);   \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);              \
    }                                                                   \
  }

/** @brief Store scene response payload structure */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_store_scene_res_s
{
  zb_uint8_t status;    /*!< Command execution status */
  zb_uint16_t group_id; /*!< Scene group identifier */
  zb_uint8_t scene_id;  /*!< Scene identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_store_scene_res_t;

/*! @brief Send Store scene response

    @param buffer to put packet to
    @param seq_num - ZCL sequence number
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param callback for getting command send status
    @param status of the corresponding Add scene command execution. Appropriate
    values (defined in @ref zb_zcl_status_e enumeration)are:
    @li @ref ZB_ZCL_STATUS_SUCCESS for successfull result
    @li ZB_ZCL_STATUS_INSUFF_SPACE if fieldsets don't fit into packet
    @li @ref ZB_ZCL_STATUS_INVALID_FIELD if the group is not present in the
    Group Table
    @param group_id - group identifier for the scene to store
    @param scene_id - scene identifier for the scene to store
*/
#define ZB_ZCL_SCENES_SEND_STORE_SCENE_RES(                   \
    buffer,                                                   \
    seq_num,                                                  \
    dst_addr,                                                 \
    dst_ep,                                                   \
    ep,                                                       \
    prof_id,                                                  \
    callback,                                                 \
    status,                                                   \
    group_id,                                                 \
    scene_id)                                                 \
  {                                                           \
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET(buffer);            \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(ptr); \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER(                          \
        ptr,                                                  \
        seq_num,                                              \
        ZB_ZCL_CMD_SCENES_STORE_SCENE_RESPONSE);              \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (status));                   \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));            \
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));                 \
    ZB_ZCL_FINISH_PACKET(buffer, ptr)                        \
    ZB_ZCL_SEND_COMMAND_SHORT(                                \
        buffer,                                               \
        dst_addr,                                             \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,                     \
        dst_ep,                                               \
        ep,                                                   \
        prof_id,                                              \
        ZB_ZCL_CLUSTER_ID_SCENES,                             \
        callback);                                            \
  }

/** @brief Parses Scenes.StoreScene command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_store_scene_res_s) or NULL pointer if payload size
    is too small
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_STORE_SCENE_RES(buffer, cmd_struct_ptr)       \
  {                                                                     \
    if (sizeof(zb_zcl_scenes_store_scene_res_t) > ZB_BUF_LEN((buffer))) \
    {                                                                   \
      (cmd_struct_ptr) = NULL;                                          \
    }                                                                   \
    else                                                                \
    {                                                                   \
      (cmd_struct_ptr) =                                                \
        (zb_zcl_scenes_store_scene_res_t*)ZB_BUF_BEGIN(buffer);         \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->group_id);              \
    }                                                                   \
  }

/** @brief Scene table capacity: at least one scene can be added

    Exact number is unknown
*/
#define ZB_ZCL_SCENES_CAPACITY_ONE_MORE ((zb_uint8_t)0xfe)

/** @brief Scene table capacity unknown */
#define ZB_ZCL_SCENES_CAPACITY_UNKNOWN ((zb_uint8_t)0xff)

/** @brief Get scene membership response fixed-structure payload part
   representation */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_get_scene_membership_res_mandatory_s
{
  /** @brief Command execution status

      Appropriate values are:
      @li ZB_ZCL_STATUS_SUCCESS - successfully executed
      @li ZB_ZCL_STATUS_INVALID_FIELD - group is not present in the Group Table
      see @ref zb_zcl_status_e enumeration
  */
  zb_uint8_t status;
  /** @brief Scene table capacity

      Permitted values are:
      @li 0..0xfd direct scene table capacity
      @li @ref ZB_ZCL_SCENES_CAPACITY_ONE_MORE at least one more scene can be added
      @li @ref ZB_ZCL_SCENES_CAPACITY_UNKNOWN scene table capacity is unknown
  */
  zb_uint8_t capacity;
  zb_uint16_t group_id; /**< Group identifier */
} ZB_PACKED_STRUCT zb_zcl_scenes_get_scene_membership_res_mandatory_t;

/** @brief Get scene membership response optional payload part
   representation */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_get_scene_membership_res_optional_s
{
  /** @brief Number of scenes in the response.
      @note Valid if status has ZB_ZCL_STATUS_SUCCESS value only.
  */
  zb_uint8_t scene_count;
  /** @brief Array of scenes corresponding to the group identifier.

      Array contains strictly @ref scene_count elements.
      @note Valid if status has ZB_ZCL_STATUS_SUCCESS value only.
  */
  zb_uint8_t scene_list[1];
} ZB_PACKED_STRUCT zb_zcl_scenes_get_scene_membership_res_optional_t;

/** @brief Get scene membership response */
typedef ZB_PACKED_PRE struct zb_zcl_scenes_get_scene_membership_res_s
{
  /** @brief Mandatory payload part */
  zb_zcl_scenes_get_scene_membership_res_mandatory_t mandatory;
  /** @brief Optional payload part */
  zb_zcl_scenes_get_scene_membership_res_optional_t optional;
} ZB_PACKED_STRUCT zb_zcl_scenes_get_scene_membership_res_t;

/** @brief Start filling Get scene membership response frame

    Fills the mandatory part of the Get scene membership scene response payload.

    @param buffer to put packet to
    @param cmd_struct_ptr - pointer to the place variable-structure part of the
    command payload (fieldsets) should be placed.
    @param seq_num - ZCL sequence number
    @param cap_ptr - pointer to the capacity fields of the response
    @param status of the command execution. Appropriate values (defined in @ref
    zb_zcl_status_e enumeration) are:
    @li ZB_ZCL_STATUS_SUCCESS for successfull result
    @li ZB_ZCL_STATUS_INVALID_FIELD if the group is not present in the
    @param capacity of the scene table
    @param group_id - group identifier for the scene to store
*/
#define ZB_ZCL_SCENES_INIT_GET_SCENE_MEMBERSHIP_RES(          \
    buffer,                                                   \
    cmd_struct_ptr,                                           \
    seq_num,                                                  \
    cap_ptr,                                                  \
    status,                                                   \
    capacity,                                                 \
    group_id)                                                 \
  {                                                           \
    cmd_struct_ptr = ZB_ZCL_START_PACKET(buffer);             \
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(      \
        cmd_struct_ptr);                                      \
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER(                          \
        cmd_struct_ptr,                                       \
        seq_num,                                              \
        ZB_ZCL_CMD_SCENES_GET_SCENE_MEMBERSHIP_RESPONSE);     \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, (status));        \
    (cap_ptr) = (cmd_struct_ptr);                             \
    ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, (capacity));      \
    ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_struct_ptr, (group_id)); \
  }

/** @brief Appends Scene count value to Get scene membership response
    constant-structure payload part
    @note Should not be used if command status is not ZB_ZCL_STATUS_SUCCESS
    @param cmd_struct_ptr - pointer to the memory area SceneCount value should be put to
    @param scene_count - number of scenes in the scene list
*/
#define ZB_ZCL_SCENES_ADD_SCENE_COUNT_GET_SCENE_MEMBERSHIP_RES( \
    cmd_struct_ptr,                                             \
    scene_count)                                                \
  ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, (scene_count))

/** @brief Appends a scene identifier to the Scene list in Get scene membership
    response payload
    @note Should be preceded with @ref
    ZB_ZCL_SCENES_ADD_SCENE_COUNT_GET_SCENE_MEMBERSHIP_RES macro invocation.
    @param cmd_struct_ptr - pointer to the memory area scene identifier should
    be put to
    @param scene_id - scene identifier
*/
#define ZB_ZCL_SCENES_ADD_SCENE_ID_GET_SCENE_MEMBERSHIP_RES( \
    cmd_struct_ptr,                                          \
    scene_id)                                                \
  ZB_ZCL_PACKET_PUT_DATA8(cmd_struct_ptr, (scene_id))

/*! @brief Sends Get scene membership response

    @param buffer with prepared command payload
    @param cmd_struct_ptr - pointer to the place directly after the payload
    @param dst_addr - address to send packet to
    @param dst_ep - destination endpoint
    @param ep - sending endpoint
    @param prof_id - profile identifier
    @param callback for getting command send status
*/
#define ZB_ZCL_SCENES_SEND_GET_SCENE_MEMBERSHIP_RES( \
    buffer,                                          \
    cmd_struct_ptr,                                  \
    dst_addr,                                        \
    dst_ep,                                          \
    ep,                                              \
    prof_id,                                         \
    callback)                                        \
  {                                                  \
    ZB_ZCL_FINISH_PACKET(buffer, cmd_struct_ptr)    \
    ZB_ZCL_SEND_COMMAND_SHORT(                       \
        buffer,                                      \
        dst_addr,                                    \
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,            \
        dst_ep,                                      \
        ep,                                          \
        prof_id,                                     \
        ZB_ZCL_CLUSTER_ID_SCENES,                    \
        callback);                                   \
  }

/** @brief Parses Scenes.RemoveAllScenesResponse command payload
    @param buffer to get data from
    @param cmd_struct_ptr - pointer to the request representation structure (of
    type @ref zb_zcl_scenes_get_scene_membership_res_s) or NULL pointer if
    payload size is too small to contain mandatory part
    @attention The macro changes content of the buffer
*/
#define ZB_ZCL_SCENES_GET_GET_SCENE_MEMBERSHIP_RES(buffer, cmd_struct_ptr) \
  {                                                                        \
    if (ZB_BUF_LEN((buffer)) <                                             \
        sizeof(zb_zcl_scenes_get_scene_membership_res_t))                  \
    {                                                                      \
      (cmd_struct_ptr) = NULL;                                             \
    }                                                                      \
    else                                                                   \
    {                                                                      \
      (cmd_struct_ptr) =                                                   \
        (zb_zcl_scenes_get_scene_membership_res_t*)ZB_BUF_BEGIN((buffer)); \
      ZB_ZCL_HTOLE16_INPLACE(&(cmd_struct_ptr)->mandatory.group_id);       \
    }                                                                      \
  }

/*! @} */ /* Scenes cluster commands */

/*! @} */ /* ZCL Scenes cluster definitions */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

zb_void_t zb_zcl_scenes_init_server();
zb_void_t zb_zcl_scenes_init_client();
#define ZB_ZCL_CLUSTER_ID_SCENES_SERVER_ROLE_INIT zb_zcl_scenes_init_server
#define ZB_ZCL_CLUSTER_ID_SCENES_CLIENT_ROLE_INIT zb_zcl_scenes_init_client

#endif /* ZB_ZCL_SCENES_H */
