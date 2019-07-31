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
 *  PURPOSE: Timer functions for nRF52840 SoC
*/

#define ZB_TRACE_FILE_ID 52493

#include "zboss_api.h"
#include "zb_nrf52840_internal.h"
#include "zb_nrf52840_zboss_deps.h"

#define ZB_NRF52840_TIMER_VALUE    15360  /*microseconds in one beacon interval*/

static void timer_event_handler(nrf_timer_event_t event_type, void* p_context);

#ifdef ZB_USE_SLEEP
#define RTC_MS_TO_TICKS(ms)        (((ms) * zb_nrf_cfg_get_rtc_default_config_frequency()) / 1000)
#define RTC_TICKS_TO_MS(ms)        (((ms) * 1000) / zb_nrf_cfg_get_rtc_default_config_frequency())

static void rtc_event_handler(nrf_drv_rtc_int_type_t int_type);
#endif /*ZB_USE_SLEEP*/


void zb_nrf52840_timer_stop()
{
  nrf_drv_timer_disable(zb_nrf_cfg_get_zboss_timer());
}

void zb_nrf52840_timer_start()
{
  if (!nrf_drv_timer_is_enabled(zb_nrf_cfg_get_zboss_timer()))
  {
    nrf_drv_timer_enable(zb_nrf_cfg_get_zboss_timer());
  }
}

zb_bool_t zb_nrf52840_timer_is_on()
{
  if (!nrf_drv_timer_is_enabled(zb_nrf_cfg_get_zboss_timer()))
  {
    return ZB_FALSE;
  }

  return ZB_TRUE;
}

void zb_osif_timer_init()
{
  uint32_t time_ticks;
  uint32_t time_us = ZB_NRF52840_TIMER_VALUE;

  /*Configure Timer1 at 15360 us = 1 beacon intervel*/
  nrf_drv_timer_config_t timer_cfg = zb_nrf_cfg_get_timer_default_config();
  nrf_drv_timer_init(zb_nrf_cfg_get_zboss_timer(), &timer_cfg, timer_event_handler);
  time_ticks = nrf_drv_timer_us_to_ticks(zb_nrf_cfg_get_zboss_timer(), time_us);
  /*
    NRF_TIMER_CC_CHANNEL1 is simple enum in components/drivers_nrf/hal/nrf_timer.h and does not depend on sdk_config.h.
    NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK is a mask defined in components/drivers_nrf/hal/nrf_timer.h and does not depend on sdk_config.h.
   */
  nrf_drv_timer_extended_compare(zb_nrf_cfg_get_zboss_timer(), NRF_TIMER_CC_CHANNEL1, time_ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);
  nrf_drv_timer_enable(zb_nrf_cfg_get_zboss_timer());

#ifdef ZB_USE_SLEEP
  /*Configure clock for RTC*/
  nrf_drv_clock_init();
  nrf_drv_clock_lfclk_request(NULL);
  /*Configure RTC*/
  nrf_drv_rtc_config_t rtc_cfg = zb_nrf_cfg_get_rtc_default_config();
  nrf_drv_rtc_init(&zb_nrf_cfg_get_zboss_rtc(), &rtc_cfg, rtc_event_handler);
  nrf_drv_rtc_enable(&zb_nrf_cfg_get_zboss_rtc());
#endif /*ZB_USE_SLEEP*/
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

#ifdef ZB_USE_SLEEP
static void rtc_event_handler(nrf_drv_rtc_int_type_t int_type)
{

}

zb_uint32_t zb_nrf52840_rtc_sleep(zb_uint32_t sleep_tmo)
{
  uint32_t rtc_prev_val = 0;
  uint32_t rtc_wait_val = 0;

  if (!sleep_tmo)
  {
    return sleep_tmo;
  }

  /*previously stop timer*/
  zb_nrf52840_timer_stop();

  rtc_prev_val = nrf_drv_rtc_counter_get(&zb_nrf_cfg_get_zboss_rtc());
  rtc_wait_val = RTC_MS_TO_TICKS(sleep_tmo) + rtc_prev_val;

  /*schedule wake up event after timeout*/
  nrf_drv_rtc_cc_set(&zb_nrf_cfg_get_zboss_rtc(), 0, rtc_wait_val, true);

  /*wait event*/
  __SEV();
  __WFE();
  __WFE();

  /*calculate real completed time*/
  rtc_wait_val = nrf_drv_rtc_counter_get(&zb_nrf_cfg_get_zboss_rtc()) - rtc_prev_val;

  /*restore timer*/
  zb_nrf52840_timer_start();

  return RTC_TICKS_TO_MS(rtc_wait_val);
}

#endif /*ZB_USE_SLEEP*/
