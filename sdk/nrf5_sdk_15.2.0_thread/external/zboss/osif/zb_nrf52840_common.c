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

#define ZB_TRACE_FILE_ID 15198

#include "zboss_api.h"
#include "zb_nrf52840_internal.h"
#include "zb_nrf52840_zboss_deps.h"

#if !defined SOFTDEVICE_PRESENT
#include <stdbool.h>
#include "nrf_ecb.h"
#else
#include "nrf_fstorage.h"
#endif

#include "nrf_log_ctrl.h"

static void zb_osif_rng_init(void);
static zb_uint32_t zb_osif_read_rndval(void);
static void zb_osif_aes_init(void);

#if defined SOFTDEVICE_PRESENT
/**
 * @brief Function used to inform radio driver about Softdevice's SoC events.
 *        Copied from nRF radio driver header files to avoid additional external dependencies inside SDK examples.
 *
 */
extern void nrf_raal_softdevice_soc_evt_handler(uint32_t evt_id);

static zb_void_t zb_zboss_radio_driver_callback(uint32_t sys_evt)
{
  nrf_raal_softdevice_soc_evt_handler(sys_evt);
}

static void soc_evt_handler(uint32_t sys_evt, void * p_context)
{
  UNUSED_PARAMETER(p_context);

  switch (sys_evt)
  {
    case NRF_EVT_FLASH_OPERATION_SUCCESS:
    case NRF_EVT_FLASH_OPERATION_ERROR:
      break;
    case NRF_EVT_HFCLKSTARTED:
    case NRF_EVT_RADIO_BLOCKED:
    case NRF_EVT_RADIO_CANCELED:
    case NRF_EVT_RADIO_SIGNAL_CALLBACK_INVALID_RETURN:
    case NRF_EVT_RADIO_SESSION_IDLE:
    case NRF_EVT_RADIO_SESSION_CLOSED:
      /* nRF Radio Driver softdevice event handler. */
      zb_zboss_radio_driver_callback(sys_evt);
      break;
    default:
      ZB_ASSERT(0);
      break;
  }
}
#endif

/**
   SoC general initialization
*/
void zb_nrf52840_general_init(void)
{
  /* Initialise system timer */
  zb_osif_timer_init();
#if defined ZB_TRACE_OVER_USART
  /* Initialise serial trace */
  zb_osif_serial_init();
#endif
  /* Initialise random generator */
  zb_osif_rng_init();

  /* Initialise AES ECB */
  zb_osif_aes_init();

#ifdef SOFTDEVICE_PRESENT
#ifdef ZB_ENABLE_SOFTDEVICE_FROM_ZBOSS
  /* Enable softdevice */
  {
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();

    APP_ERROR_CHECK(err_code);
  }
#endif
  NRF_SDH_SOC_OBSERVER(m_soc_observer, NRF_SDH_SOC_STACK_OBSERVER_PRIO, soc_evt_handler, NULL);
#endif
}

/**
   SoC sleep subsystem initialization
*/
void zb_nrf52840_sleep_init(void)
{

}

void zb_nrf52840_abort(void)
{
  while(1)
  {
#ifdef ZB_NRF_TRACE
    /* Flush all remaining logs, including reason of abort. */
    zb_osif_serial_flush();
#endif /* ZB_NRF_TRACE */
  }
}

zb_void_t zb_reset(zb_uint8_t param)
{
  ZVUNUSED(param);
}


void zb_nrf52840_enable_all_inter(void)
{
  __enable_irq();
}

void zb_nrf52840_disable_all_inter(void)
{
  __disable_irq();
}

zb_uint32_t zb_random_seed(void)
{
  return zb_osif_read_rndval();
}

/* Timer */

zb_uint32_t zb_get_utc_time(void)
{
  return ZB_TIME_BEACON_INTERVAL_TO_MSEC(ZB_TIMER_GET()) / 1000;
}

/**
   Get current time, us.

   Use transceiver timer when possible.
*/
zb_time_t osif_transceiver_time_get(void)
{
/* 04/03/2018 EE CR:MAJOR Here we have timer accuracy == 1 beacon
 * interval. Transceiver timer is to be used fot GreenPower TX. It
 * needs more accuracy. Usually we use both current timer value (in
 * beacon intervals) and SySTick counter. Need to check how it can be
 * implemented for Nordic. */
  return ZB_TIME_BEACON_INTERVAL_TO_USEC(ZB_TIMER_GET());
}


zb_time_t osif_sub_trans_timer(zb_time_t t2, zb_time_t t1)
{
  return ZB_TIME_SUBTRACT(t2, t1);
}


void osif_busy_loop_delay(zb_uint32_t count)
{
  ZVUNUSED(count);
}


void osif_sleep_using_transc_timer(zb_time_t timeout)
{
  zb_time_t tend = osif_transceiver_time_get() + timeout;

  if (tend < osif_transceiver_time_get())
  {
    while (tend < osif_transceiver_time_get())
    {
      osif_busy_loop_delay(10);
    }
  }
  else
  {
    while (osif_transceiver_time_get() < tend)
    {
      osif_busy_loop_delay(10);
    }
  }
}


static void zb_osif_rng_init(void)
{
  ret_code_t err_code;
  err_code = nrf_drv_rng_init(NULL);
  NRF_ERR_CHECK(err_code);
}


static zb_uint32_t zb_osif_read_rndval()
{
  zb_uint32_t rnd_val  = 0;
  zb_uint32_t err_code = 0;
  zb_uint8_t  length   = 0;

  /*wait complete randomization opration*/
  while (length < sizeof(rnd_val))
  {
    nrf_drv_rng_bytes_available(&length);
  }

  /*read random value*/
  err_code = nrf_drv_rng_rand((uint8_t *)&rnd_val, sizeof(rnd_val));
  if (err_code != NRF_SUCCESS)
    return 0;

  return rnd_val;
}

static void zb_osif_aes_init(void)
{
#if !defined SOFTDEVICE_PRESENT
  ret_code_t err_code;
  err_code = nrf_ecb_init();
  NRF_ERR_CHECK_BOOL(err_code);
#endif
}

void hw_aes128(zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c)
{
#if !defined SOFTDEVICE_PRESENT
  ret_code_t err_code;
#endif
  ZB_ASSERT(key);
  ZB_ASSERT(msg);
  ZB_ASSERT(c);
  if (!(c && msg && key))
  {
    return;
  }
#if !defined SOFTDEVICE_PRESENT
  nrf_ecb_set_key(key);
  err_code = nrf_ecb_crypt(c, msg);
  NRF_ERR_CHECK_BOOL(err_code);
#else
  {
    nrf_ecb_hal_data_t ecb_data;

    ZB_MEMCPY(ecb_data.key, key, SOC_ECB_KEY_LENGTH );
    ZB_MEMCPY(ecb_data.cleartext, msg, SOC_ECB_CLEARTEXT_LENGTH );
    if (sd_ecb_block_encrypt(&ecb_data)!= NRF_SUCCESS)
    {
      ZB_ASSERT(0);
    }
    ZB_MEMCPY(c, ecb_data.ciphertext, SOC_ECB_CIPHERTEXT_LENGTH );
  }
#endif
}

zb_uint32_t zb_nrf52840_sleep(zb_uint32_t sleep_tmo)
{
  return zb_nrf52840_sched_sleep(sleep_tmo);
}

zb_bool_t zb_osif_is_inside_isr(void)
{
/*
  return (zb_bool_t)(!!(SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk));
*/
  return (zb_bool_t)(__get_IPSR() != 0);
}

/**@brief Read IEEE long address from FICR registers. */
void zb_osif_get_ieee_eui64(zb_ieee_addr_t ieee_eui64)
{
  uint64_t factoryAddress;

  // Read random address from FICR.
  factoryAddress  = (uint64_t)NRF_FICR->DEVICEID[0] << 32;
  factoryAddress |= NRF_FICR->DEVICEID[1];

  // Set constant manufacturer ID to use MAC compression mechanisms.
  factoryAddress &= 0x000000FFFFFFFFFFLL;
  factoryAddress |= 0x0B010E0000000000LL;

  memcpy(ieee_eui64, &factoryAddress, sizeof(factoryAddress));
}

#if defined ZB_NRF52840_RADIO_STATISTICS
static zb_nrf52840_radio_stats_t g_nrf52840_radio_statistics;

zb_nrf52840_radio_stats_t* zb_nrf52840_get_radio_stats(void)
{
  return &g_nrf52840_radio_statistics;
}
#endif /* defined ZB_NRF52840_RADIO_STATISTICS */

/**@brief Function which waits for event -- essential implementation of sleep on NRF52 */
zb_void_t zb_osif_wait_for_event(zb_void_t)
{
  #ifdef SOFTDEVICE_PRESENT
      UNUSED_RETURN_VALUE(sd_app_evt_wait());
  #else
      __SEV();
      __WFE();
      __WFE();
  #endif
}

/*lint -save -e(14) Weak linkage */
/**@brief Function which tries to sleep down the MCU 
 *
 * Function is defined as weak; to be redefined if someone wants to implement their own
 * going-to-sleep policy.
 */
__WEAK zb_void_t zb_osif_go_idle(zb_void_t)
{
  if (ZB_PIBCACHE_RX_ON_WHEN_IDLE())
  {
    if (NRF_LOG_PROCESS() == false)
    {
      zb_osif_wait_for_event();
    }
  }
}
/*lint -restore */
