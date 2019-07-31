/**
* \copyright
* Copyright (c) 2018, Infineon Technologies AG
* All rights reserved.                                                         
*
* This software is provided with terms and conditions as specified in OPTIGA(TM) Trust X Evaluation Kit License Agreement.
* \endcopyright
*
* \author Infineon AG
*
* \file ifx_i2c_data_link_layer.h
*
* \brief   This file defines the API prototype for data link layer of the Infineon I2C Protocol Stack library.
*
* \addtogroup  grIFXI2C
* @{
*/

#ifndef _IFX_I2C_DATA_LINK_LAYER_H_
#define _IFX_I2C_DATA_LINK_LAYER_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/***********************************************************************************************************************
* HEADER FILES
**********************************************************************************************************************/
#include "ifx_i2c_config.h"

/***********************************************************************************************************************
* MACROS
**********************************************************************************************************************/
/** @brief Error event propagated to upper layer */
#define IFX_I2C_DL_EVENT_ERROR              (0x01)
/** @brief Transmit success event propagated to upper layer (bit field 1) */
#define IFX_I2C_DL_EVENT_TX_SUCCESS         (0x02)
/** @brief Receive success event propagated to upper layer (bit field 3)*/
#define IFX_I2C_DL_EVENT_RX_SUCCESS         (0x04)

/***********************************************************************************************************************
* ENUMS
**********************************************************************************************************************/
/***********************************************************************************************************************
* DATA STRUCTURES
***********************************************************************************************************************/
/***********************************************************************************************************************
* API PROTOTYPES
**********************************************************************************************************************/
/**
 * @brief Function for initializing the module.
 *
 * Function initializes and enables the module and registers
 * an event handler to receive events from this module.
 * @attention This function must be called before using the module.
 *
 * @param[in,out] p_ctx     Pointer to ifx i2c context.
 * @param[in] handler     Function pointer to the event handler of the upper layer.
 *
 * @retval  IFX_I2C_STACK_SUCCESS If initialization was successful.
 * @retval  IFX_I2C_STACK_ERROR If the module is already initialized.
 */
host_lib_status_t ifx_i2c_dl_init(ifx_i2c_context_t *p_ctx,ifx_i2c_event_handler_t handler);

/**
 * @brief Function for sending a frame.
 *
 * Asynchronous function to send a frame. The function returns immediately.
 * One of the following events is propagated to the event handler registered
 * with @ref ifx_i2c_dl_init.
 *
 * @param[in,out] p_ctx     Pointer to ifx i2c context.
 * @param[in] frame_len     Frame length.
 *
 * @retval  IFX_I2C_STACK_SUCCESS If function was successful.
 * @retval  IFX_I2C_STACK_ERROR If the module is busy.
 */
host_lib_status_t ifx_i2c_dl_send_frame(ifx_i2c_context_t *p_ctx,uint16_t frame_len);

/**
 * @brief Function for receiving a frame.
 *
 * Asynchronous function to receive a frame. The function returns immediately.
 * One of the following events is propagated to the event handler registered
 * with @ref ifx_i2c_dl_init.
 *
 * @param[in,out] p_ctx     Pointer to ifx i2c context.
 *
 * @retval  IFX_I2C_STACK_SUCCESS If function was successful.
 * @retval  IFX_I2C_STACK_ERROR If the module is busy.
 */
host_lib_status_t ifx_i2c_dl_receive_frame(ifx_i2c_context_t *p_ctx);

#ifdef __cplusplus
}
#endif

#endif /* _IFX_I2C_DATA_LINK_LAYER_H_ */


/**
 * @}
 **/
