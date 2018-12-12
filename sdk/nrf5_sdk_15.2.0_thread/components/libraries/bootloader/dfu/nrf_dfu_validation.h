/**
 * Copyright (c) 2017 - 2018, Nordic Semiconductor ASA
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
/**@file
 *
 * @defgroup sdk_nrf_dfu_validation Validation
 * @{
 * @ingroup  nrf_dfu
 */

#ifndef __NRF_DFU_VALIDATION_H
#define __NRF_DFU_VALIDATION_H

#include "stdint.h"
#include "sdk_errors.h"
#include "dfu-cc.pb.h"
#include "nrf_dfu_handling_error.h"

/**
 * @brief Function for module initialization.
 *
 * Function checks if there is a valid init packet in DFU settings written in flash.
 */
void nrf_dfu_validation_init(void);

/**
 * @brief Function called on reception of init command creation request.
 *
 * @param[in] size Size of incoming init packet.
 *
 * @return       Operation result. See @ref nrf_dfu_result_t
 */
nrf_dfu_result_t nrf_dfu_validation_init_cmd_create(uint32_t size);

/**
 * @brief Function called on reception of fragment of init command.
 *
 * @param[in] p_data Init command fragment.
 * @param[in] length Init command fragment size.
 *
 * @return       Operation result. See @ref nrf_dfu_result_t
 */
nrf_dfu_result_t nrf_dfu_validation_init_cmd_append(uint8_t const * p_data, uint32_t length);

/**
 * @brief Function for getting init command status.
 *
 * @param[out] p_offset   Current offset.
 * @param[out] p_crc      Current CRC.
 * @param[out] p_max_size Maximum size of init command.
 */
void nrf_dfu_validation_init_cmd_status_get(uint32_t * p_offset,
                                            uint32_t * p_crc,
                                            uint32_t * p_max_size);

/**
 * @brief Function for inquiring whether a valid init command has been received.
 *
 * @return true  if there is a valid init command. This can be true at boot time
 *               if the device was reset during a DFU operation.
 */
bool nrf_dfu_validation_init_cmd_present(void);

/**
 * @brief Function for validating init command.
 *
 * If init command is successfully validated Bank 1 details are written to out parameters.
 *
 * Until @ref nrf_dfu_validation_init_cmd_create is called, this function can be called
 * again after the first time without side effects to retrieve address and length.
 *
 * @param[out] p_dst_data_addr Bank 1 start address if validation is successful.
 * @param[out] p_data_len  Bank 1 length if validation is successful.
 *
 * @return       Operation result. See @ref nrf_dfu_result_t
 */
nrf_dfu_result_t nrf_dfu_validation_init_cmd_execute(uint32_t * p_dst_data_addr,
                                                     uint32_t * p_data_len);

/**
 * @brief Function for postvalidating the update. Function is called once all data is received.
 *
 * @param[in] dst_data_addr Bank 1 start address.
 * @param[in] data_len      Bank 1 length.
 *
 * @return       Operation result. See @ref nrf_dfu_result_t
 */
nrf_dfu_result_t nrf_dfu_validation_post_data_execute(uint32_t dst_data_addr, uint32_t data_len);


/**
 * @brief Function to execute on validated external app.
 *
 * @details Function is called one all data is received with the parameter
 *          is_boot set to false. Function is called during bootup with the parameter
 *          set to true.
 *
 *
 *
 * @note This function requires @ref NRF_DFU_SUPPORTS_EXTERNAL_APP is set to 1.
 *       It is up to the user to implement this function.
 *
 * @warning is_trusted must be used to ensure that no loss of security of process can happen.
 *          This parameter should *only* be set if the function is called after a root-of-trust
 *          reset on the device.
 *
 *          is_trusted can be used for the following:
 *          - Ensuring that an external application is only run once (after root-of-trust)
 *          - Ensure that a bank flag or any other FLASH access can only happen after root-of-trust
 *          - Ensure that the device reaches the correct state after a a power-failure on the device
 *
 * @param[in] p_init        InitCommand for the FW upgrade
 * @param[in] is_trusted    Must be set to true if this is called after root-of-trust boot
 *                          Must be set to false if this is called from DFU mode or background
 *                          DFU operation.
 *
 * @return      Operation result. see @ref nrf_dfu_result_t
 */
nrf_dfu_result_t nrf_dfu_validation_post_external_app_execute(dfu_init_command_t const * p_init, bool is_trusted);

/**
* @brief Function to see if there is a valid external app in bank 1
*
* @returns True if valid external app, otherwise false.
*/
bool nrf_dfu_validation_valid_external_app(void);

/**
 * @brief Function to invalidate external app in bank 1
 *
 * @note This will set the bank 1 slot to @ref NRF_DFU_BANK_INVALID in FLASH.
 *
 * @note @ref nrf_dfu_settings_init must be called before using this function
 *
 * @param[in] callback  Pointer to a function that is called after completing the write operation.
 *
 * @return  Operational result. See @nrf_dfu_settings_write.
 */
ret_code_t nrf_dfu_validation_invalidate_external_app(nrf_dfu_flash_callback_t callback);

#endif //__NRF_DFU_VALIDATION_H

/** @} */
