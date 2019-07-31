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
PURPOSE: Platform specific for NRF52840 SoC.
*/

#ifndef ZB_NRF52840_INTERNAL_H
#define ZB_NRF52840_INTERNAL_H 1

#ifdef __GNUC__
/* Switch off warnings produced by Nordic headers */
#pragma GCC system_header
#endif

#include "nrf.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_rng.h"
#include "zboss_api.h"
#include "nrf_assert.h"
#if defined SOFTDEVICE_PRESENT
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#endif

const nrf_drv_timer_t *zb_nrf_cfg_get_zboss_timer(void);
nrf_drv_timer_config_t zb_nrf_cfg_get_timer_default_config(void);

zb_uint32_t zb_nrf52840_sched_sleep(zb_uint32_t sleep_tmo);
zb_void_t zb_osif_wait_for_event(zb_void_t);

void mac_nrf52840_trans_set_rx_on_off(zb_bool_t rx_on);
#if defined SOFTDEVICE_PRESENT
/**@brief Function for informing the radio driver about the SoC events of the SoftDevice.
 *        Copied from the nRF radio driver header files to avoid additional external dependencies inside SDK examples.
 */
void nrf_raal_softdevice_soc_evt_handler(uint32_t evt_id);
#endif

#if defined ZB_TRACE_LEVEL && defined ZB_TRACE_OVER_USART && !defined ZB_NRF_TRACE
#include "nordic_common.h"
#include "nrf_drv_uart.h"

const nrf_drv_uart_t* zb_nrf_cfg_get_nrf52840_uart(void);
const nrf_drv_uart_config_t* zb_nrf_cfg_get_nrf52840_uart_config(void);
void zb_osif_uart_int_disable(void);
void zb_osif_uart_int_enable(void);
#endif

#if (defined(DEBUG_NRF) || defined(DEBUG_NRF_USER))
#define NRF_ERR_CHECK(err_code) ASSERT(err_code == NRF_SUCCESS)
#define NRF_ERR_CHECK_BOOL(err_code) ASSERT(err_code == true)
#else
#define NRF_ERR_CHECK(err_code) if (err_code != NRF_SUCCESS) {ZB_ABORT();}
#define NRF_ERR_CHECK_BOOL(err_code) if (err_code != true) {ZB_ABORT();}
#endif

#endif /* ZB_NRF52840_INTERNAL_H */
