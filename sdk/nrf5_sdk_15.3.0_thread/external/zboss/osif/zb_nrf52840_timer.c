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
 PURPOSE: Timer functions for nRF52840 SoC
*/

#define ZB_TRACE_FILE_ID 52493

/* Fixes to make Lint passable - radio driver headers are not linted */
#ifndef RAAL_SOFTDEVICE
#define RAAL_SOFTDEVICE 0
#endif /* RAAL_SOFTDEVICE */

#include "zboss_api.h"
#include "zb_nrf52840_internal.h"
#include "zb_nrf52840_zboss_deps.h"
#include "timer_scheduler/nrf_802154_timer_sched.h"
#include "fem/nrf_fem_control_api.h"

#if defined(NRF_LOG_BACKEND_UART_ENABLED) && NRF_LOG_BACKEND_UART_ENABLED
#include "nrf_log_backend_uart.h"
#include "nrf_drv_uart.h"
//TODO: get rid of extern by https://projecttools.nordicsemi.no/jira/browse/KRKNWK-2060
extern nrf_drv_uart_t m_uart;
#endif

#define ZB_NRF52840_TIMER_VALUE    15360  /*microseconds in one beacon interval*/

static void timer_event_handler(nrf_timer_event_t event_type, void* p_context);
static void rtc_event_handler(void * p_context);

static nrf_802154_timer_t m_timer;
static zb_bool_t m_timer_is_init = ZB_FALSE;

void zb_nrf52840_timer_stop()
{
  nrf_drv_timer_disable(zb_nrf_cfg_get_zboss_timer());
  nrf_drv_timer_uninit(zb_nrf_cfg_get_zboss_timer());
  m_timer_is_init = ZB_FALSE;
}

void zb_nrf52840_timer_start()
{
  if (!m_timer_is_init)
  {
    zb_osif_timer_init();
  }
  if (!nrf_drv_timer_is_enabled(zb_nrf_cfg_get_zboss_timer()))
  {
    nrf_drv_timer_enable(zb_nrf_cfg_get_zboss_timer());
  }
}

zb_bool_t zb_nrf52840_timer_is_on()
{
  return m_timer_is_init;
}

void zb_osif_timer_init()
{
  uint32_t time_ticks;
  uint32_t time_us = ZB_NRF52840_TIMER_VALUE;
  ret_code_t err_code;

  /*Configure Timer1 at 15360 us = 1 beacon intervel*/
  nrf_drv_timer_config_t timer_cfg = zb_nrf_cfg_get_timer_default_config();
  err_code = nrf_drv_timer_init(zb_nrf_cfg_get_zboss_timer(), &timer_cfg, timer_event_handler);
  NRF_ERR_CHECK(err_code);
  time_ticks = nrf_drv_timer_us_to_ticks(zb_nrf_cfg_get_zboss_timer(), time_us);
  /*
    NRF_TIMER_CC_CHANNEL1 is simple enum in components/drivers_nrf/hal/nrf_timer.h and does not depend on sdk_config.h.
    NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK is a mask defined in components/drivers_nrf/hal/nrf_timer.h and does not depend on sdk_config.h.
   */
  nrf_drv_timer_extended_compare(zb_nrf_cfg_get_zboss_timer(), NRF_TIMER_CC_CHANNEL1, time_ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);
  nrf_drv_timer_enable(zb_nrf_cfg_get_zboss_timer());

  m_timer_is_init = ZB_TRUE;
}

/*
  timer interrupt handler
*/
static void timer_event_handler(nrf_timer_event_t event_type, void* p_context)
{
  ZVUNUSED(p_context);
  switch (event_type)
  {
    case NRF_TIMER_EVENT_COMPARE1:
      zb_osif_zboss_timer_tick();
      break;

    default:
      /*Do nothing*/
      break;
  }
}

static void rtc_event_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
}

zb_uint32_t zb_nrf52840_sched_sleep(zb_uint32_t sleep_tmo)
{
  uint32_t rtc_prev_val = 0;
  uint32_t rtc_wait_val = 0;
  ret_code_t err_code;

  if (!sleep_tmo)
  {
    return sleep_tmo;
  }

  /* Previously stop timer */
  zb_nrf52840_timer_stop();

  /* Previously deinitialise Random generator */
  nrf_drv_rng_uninit();

#if defined ZB_NRF_TRACE
#if defined(NRF_LOG_BACKEND_UART_ENABLED) && NRF_LOG_BACKEND_UART_ENABLED
    /* Deinitialise the UART logging */
    nrf_drv_uart_uninit(&m_uart);
#endif /* #if defined(NRF_LOG_BACKEND_UART_ENABLED) && NRF_LOG_BACKEND_UART_ENABLED */

#else /* defined ZB_NRF_TRACE */
    zb_osif_uart_int_disable();
#endif

  rtc_prev_val = nrf_802154_timer_sched_time_get();

  m_timer.callback  = rtc_event_handler;
  m_timer.p_context = NULL;
  m_timer.t0        = rtc_prev_val;
  m_timer.dt        = 1000 * sleep_tmo;

  nrf_802154_timer_sched_add(&m_timer, false);

  /* Handle the FEM sleep-wake requirements */
  nrf_fem_control_deactivate();

  /*wait event*/
  zb_osif_wait_for_event();

  /* Handle the FEM sleep-wake requirements */
  nrf_fem_control_activate();

  /* Calculate real completed time (us) */
  rtc_wait_val = nrf_802154_timer_sched_time_get() - rtc_prev_val;

  /* Restore the Timer */
  zb_nrf52840_timer_start();

  /* Restore the Random generator */
  err_code = nrf_drv_rng_init(NULL);
  NRF_ERR_CHECK(err_code);

#if defined ZB_NRF_TRACE
#if defined(NRF_LOG_BACKEND_UART_ENABLED) && NRF_LOG_BACKEND_UART_ENABLED
    /* Restore UART logging */
    nrf_log_backend_uart_init();
#endif /* #if defined(NRF_LOG_BACKEND_UART_ENABLED) && NRF_LOG_BACKEND_UART_ENABLED */

#else /* defined ZB_NRF_TRACE */
    zb_osif_uart_int_enable();
#endif /* defined ZB_NRF_TRACE */

  return rtc_wait_val/1000;
}
