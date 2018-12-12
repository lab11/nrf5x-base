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
 PURPOSE: Support for nRF52840 SoC. Code dependent on sdk_config.h so
 it must be included into application in the source form.
*/

#define ZB_TRACE_FILE_ID 21452
#include "zboss_api.h"

#include "nrf.h"
/* switch off warnings in Nordic headers */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#include "nrf_drv_timer.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#if !defined SOFTDEVICE_PRESENT
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#endif /* !defined SOFTDEVICE_PRESENT */

#if defined ZB_TRACE_LEVEL && defined ZB_TRACE_OVER_USART && !defined ZB_NRF_TRACE
#include "nordic_common.h"
#include "nrf_drv_uart.h"
#endif

/* Safety checks. ZBOSS requires some modules. */
#if !NRF_MODULE_ENABLED(RNG)
#error RNG must be enabled!
#endif
#if !NRF_MODULE_ENABLED(NRF_CLOCK)
#error CLOCK must be enabled!
#endif
#if !NRF_MODULE_ENABLED(TIMER)
#error TIMER must be enabled!
#endif

/* We can take here timer 0 or timer 1 depending on defined sin sdk_config.h */
static const nrf_drv_timer_t gs_zboss_timer = ZIGBEE_TIMER_INSTANCE;

const nrf_drv_timer_t *zb_nrf_cfg_get_zboss_timer(void)
{
  return &gs_zboss_timer;
}


nrf_drv_timer_config_t zb_nrf_cfg_get_timer_default_config(void)
{
  nrf_drv_timer_config_t c =
  {
    .frequency          = (nrf_timer_frequency_t)4,          /* 1 MHz */
    .mode               = (nrf_timer_mode_t)     0,          /* Timer */
    .bit_width          = (nrf_timer_bit_width_t)3,          /* 32 bit */
    .interrupt_priority = TIMER_DEFAULT_CONFIG_IRQ_PRIORITY, /* default - 7 */
    .p_context          = NULL
  };
  return c;
}


#if defined ZB_TRACE_LEVEL && defined ZB_TRACE_OVER_USART && !defined ZB_NRF_TRACE

#if !NRF_MODULE_ENABLED(UART)
#error UART must be enabled!
#endif


/*UART parameters*/
#define NRF52840_DRV_UART_CONFIG                                      \
  {                                                                   \
    .pseltxd            = 6,                                          \
    .pselrxd            = 8,                                          \
    .pselcts            = 7,                                          \
    .pselrts            = 5,                                          \
    .p_context          = NULL,                                       \
    .hwfc               = NRF_UART_HWFC_DISABLED,                     \
    .parity             = NRF_UART_PARITY_EXCLUDED,                   \
    .baudrate           = (nrf_uart_baudrate_t)UART_BAUDRATE_BAUDRATE_Baud115200, \
    .interrupt_priority = UART_DEFAULT_CONFIG_IRQ_PRIORITY,           \
  }

/*Declare nRF UART configuration*/
static const nrf_drv_uart_t gs_nrf52840_uart_inst   = NRF_DRV_UART_INSTANCE(0);
static const nrf_drv_uart_config_t gs_nrf52840_uart_config = NRF52840_DRV_UART_CONFIG;

const nrf_drv_uart_t* zb_nrf_cfg_get_nrf52840_uart(void)
{
  return &gs_nrf52840_uart_inst;
}

const nrf_drv_uart_config_t* zb_nrf_cfg_get_nrf52840_uart_config(void)
{
  return &gs_nrf52840_uart_config;
}

void zb_osif_uart_int_disable()
{
  NVIC_DisableIRQ(UARTE0_UART0_IRQn);
  __DSB();
  __ISB();
}

void zb_osif_uart_int_enable()
{
  NVIC_EnableIRQ(UARTE0_UART0_IRQn);
}


#endif  /* using if serial port */
