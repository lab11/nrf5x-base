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

#include "csl_wake.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <openthread/ip6.h>
#include <openthread/link_raw.h>
#include <openthread/thread.h>

#include "app_timer.h"
#include "app_util.h"
#include "app_scheduler.h"
#include "cpu_utilization.h"
#include "cli_suppress.h"
#include "nrf_log.h"
#include "thread_utils.h"

#define ENABLE_DEBUG_PINS 0

#if ENABLE_DEBUG_PINS
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_ppi.h"
#define TP1 NRF_GPIO_PIN_MAP(1,1)
#define TP2 NRF_GPIO_PIN_MAP(1,2)
#define TP3 NRF_GPIO_PIN_MAP(1,3)
#define TP4 NRF_GPIO_PIN_MAP(1,4)
#define TP5 NRF_GPIO_PIN_MAP(1,5)
#endif

// This is modified version of APP_TIMER_TICKS that allows to use time in us instead of ms.

#ifndef FREERTOS
#define APP_TIMER_TICKS_US(US)                                  \
               ((uint32_t)ROUNDED_DIV(                          \
               (US) * (uint64_t)APP_TIMER_CLOCK_FREQ,           \
               1000000 * (APP_TIMER_CONFIG_RTC_FREQUENCY + 1)))
#else
#include "FreeRTOSConfig.h"
#define APP_TIMER_TICKS_US(US) (uint32_t)ROUNDED_DIV((US)*configTICK_RATE_HZ, 1000000)
#endif

#define CSL_WAKE_BURST_PSDU_LEN                20   ///< PSDU length of wake packet
#define CSL_WAKE_BURST_PSDU_BYTE_0             0x41 ///< First byte of simulated wake packet - empty packet with AR not set
#define CSL_WAKE_BURST_PSDU_BYTE_1             0x98 ///< Second byte of simulated wake packet - empty packet with AR not set
#define CSL_WAKE_BURST_PSDU_BYTE_0_AR_BIT_MASK 0x20 ///< Mask for AR bit in 802.15.4 PSDU

#define BURST_DELAY_MS                         100  ///< Delay in ms between starting/ending test and disabling/enabling OpenThread

typedef struct
{
    bool     csma_ca_enabled;
    uint32_t period;
    uint32_t inter_frame_spacing;
} csl_wake_burst_desc_t;

typedef struct
{
    bool                  is_running;
    bool                  set_ar_bit;
    volatile bool         transmit_in_progress;
    size_t                bursts_cnt;
    size_t                current_burst;
    uint32_t              frames_cnt;
    uint32_t              ifs_error_cnt;
    uint16_t              old_pan_id;
    uint16_t              old_short_addr;
    csl_wake_burst_desc_t bursts_desc[CSL_WAKE_BURST_MAX_CNT];
} csl_wake_burst_state_t;

APP_TIMER_DEF(m_ifs_timer);
APP_TIMER_DEF(m_period_timer);
APP_TIMER_DEF(m_delayed_start_timer);
APP_TIMER_DEF(m_delayed_stop_timer);

csl_wake_burst_state_t m_state;

// Frame TX handling

static void send_frame(void);

static void send_frame_scheduled(void * p_event_data, uint16_t event_size)
{
    (void)p_event_data;

    if (m_state.is_running)
    {
        send_frame();
    }
}

static void link_raw_transmit_done(otInstance   * aInstance,
                                   otRadioFrame * aFrame,
                                   otRadioFrame * aAckFrame,
                                   otError        aError)
{
    (void)aInstance;
    (void)aFrame;
    (void)aAckFrame;

    m_state.transmit_in_progress = false;

#if ENABLE_DEBUG_PINS
    nrf_gpio_pin_toggle(TP3);
#endif

    if (aError == OT_ERROR_NONE)
    {
        m_state.frames_cnt++;
    }

    if (m_state.bursts_desc[m_state.current_burst].csma_ca_enabled)
    {
        ret_code_t err_code = app_sched_event_put(NULL, 0, send_frame_scheduled);
        APP_ERROR_CHECK(err_code);
    }
}

static void send_frame(void)
{
    if (!m_state.transmit_in_progress)
    {
        otError error;

#if ENABLE_DEBUG_PINS
        nrf_gpio_pin_toggle(TP2);
#endif

        otRadioFrame * frame = otLinkRawGetTransmitBuffer(thread_ot_instance_get());
        ASSERT(frame != NULL);

        frame->mChannel                     = otLinkGetChannel(thread_ot_instance_get());
        frame->mLength                      = CSL_WAKE_BURST_PSDU_LEN;
        frame->mInfo.mTxInfo.mCsmaCaEnabled = m_state.bursts_desc[m_state.current_burst].csma_ca_enabled;

        memset(frame->mPsdu, 0, CSL_WAKE_BURST_PSDU_LEN);

        frame->mPsdu[0] = CSL_WAKE_BURST_PSDU_BYTE_0;
        frame->mPsdu[1] = CSL_WAKE_BURST_PSDU_BYTE_1;

        if (m_state.set_ar_bit)
        {
            frame->mPsdu[0] |= CSL_WAKE_BURST_PSDU_BYTE_0_AR_BIT_MASK;
        }

        error = otLinkRawTransmit(thread_ot_instance_get(), link_raw_transmit_done);

        if (error != OT_ERROR_NONE)
        {
            ret_code_t err_code = app_sched_event_put(NULL, 0, send_frame_scheduled);
            APP_ERROR_CHECK(err_code);
        }
        else
        {
            m_state.transmit_in_progress = true;
        }
    }
}

static void burst_start(void)
{
    ret_code_t err_code;

    err_code = app_timer_start(m_period_timer,
                               APP_TIMER_TICKS_US(m_state.bursts_desc[m_state.current_burst].period),
                                                  NULL);
    APP_ERROR_CHECK(err_code);

    if (!m_state.bursts_desc[m_state.current_burst].csma_ca_enabled)
    {
        err_code = app_timer_start(m_ifs_timer,
                                   APP_TIMER_TICKS_US(m_state.bursts_desc[m_state.current_burst].inter_frame_spacing),
                                                      NULL);
        APP_ERROR_CHECK(err_code);
    }

    send_frame();
}

static void burst_stop(csl_wake_burst_res_t result)
{
    ret_code_t err_code;

    err_code = cpu_utilization_stop();
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_stop(m_ifs_timer);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_stop(m_period_timer);
    APP_ERROR_CHECK(err_code);

    m_state.is_running = false;

    err_code = app_timer_start(m_delayed_stop_timer, APP_TIMER_TICKS(BURST_DELAY_MS), NULL);
    APP_ERROR_CHECK(err_code);

    cli_suppress_disable();
    csl_wake_burst_ended(result);
}

// Timers handlers

static void ifs_timer_handler(void * p_context)
{
#if ENABLE_DEBUG_PINS
    nrf_gpio_pin_toggle(TP1);
#endif

    if (!m_state.is_running || m_state.bursts_desc[m_state.current_burst].csma_ca_enabled)
    {
        // Handler may be called after stop due to app scheduler
        return;
    }

    if (m_state.transmit_in_progress)
    {
        m_state.ifs_error_cnt++;
        return;
    }

    send_frame();
}

static void period_timer_handler(void * p_context)
{
    if (m_state.current_burst < (m_state.bursts_cnt - 1))
    {
        ret_code_t err_code = app_timer_stop(m_ifs_timer);
        APP_ERROR_CHECK(err_code);

        m_state.current_burst++;
        burst_start();
    }
    else
    {
        burst_stop(CSL_WAKE_BURST_OK);
    }
}

static void delayed_stop_timer_handler(void * p_context)
{
    otError error;

    otPlatRadioSetPanId(thread_ot_instance_get(), m_state.old_pan_id);
    otPlatRadioSetShortAddress(thread_ot_instance_get(), m_state.old_short_addr);

    error = otLinkRawSetEnable(thread_ot_instance_get(), false);
    ASSERT(error == OT_ERROR_NONE);

    error = otPlatRadioEnable(thread_ot_instance_get());
    ASSERT(error == OT_ERROR_NONE);

    error = otIp6SetEnabled(thread_ot_instance_get(), true);
    ASSERT(error == OT_ERROR_NONE);

    error = otThreadSetEnabled(thread_ot_instance_get(), true);
    ASSERT(error == OT_ERROR_NONE);
}

static void delayed_start_timer_handler(void * p_context)
{
    ret_code_t err_code;

    m_state.transmit_in_progress = false;

    cli_suppress_enable();

    m_state.old_pan_id     = otLinkGetPanId(thread_ot_instance_get());
    m_state.old_short_addr = otLinkGetShortAddress(thread_ot_instance_get());

    otPlatRadioSetPanId(thread_ot_instance_get(), 0);
    otPlatRadioSetShortAddress(thread_ot_instance_get(), 0);

    err_code = cpu_utilization_stop();
    APP_ERROR_CHECK(err_code);

    cpu_utilization_clear();

    err_code = cpu_utilization_start();
    APP_ERROR_CHECK(err_code);

    burst_start();
}

// Global API

csl_wake_burst_res_t csl_wake_burst_cnt_set(size_t cnt)
{
    if (m_state.is_running)
    {
        return CSL_WAKE_BURST_ALREADY_RUNNING;
    }

    if (cnt <= CSL_WAKE_BURST_MAX_CNT)
    {
        m_state.bursts_cnt = cnt;
        return CSL_WAKE_BURST_OK;
    }

    return CSL_WAKE_BURST_BAD_IDX;
}

size_t csl_wake_burst_cnt_get(void)
{
    return m_state.bursts_cnt;
}

csl_wake_burst_res_t csl_wake_burst_period_set(size_t idx, uint32_t period)
{
    if (m_state.is_running)
    {
        return CSL_WAKE_BURST_ALREADY_RUNNING;
    }

    if (idx < CSL_WAKE_BURST_MAX_CNT)
    {
        m_state.bursts_desc[idx].period = period;
        return CSL_WAKE_BURST_OK;
    }

    return CSL_WAKE_BURST_BAD_IDX;
}

uint32_t csl_wake_burst_period_get(size_t idx)
{
    if (idx < CSL_WAKE_BURST_MAX_CNT)
    {
        return m_state.bursts_desc[idx].period;
    }

    return 0;
}

csl_wake_burst_res_t csl_wake_burst_ifs_set(size_t idx, uint32_t ifs)
{
    if (m_state.is_running)
    {
        return CSL_WAKE_BURST_ALREADY_RUNNING;
    }

    if (idx < CSL_WAKE_BURST_MAX_CNT)
    {
        m_state.bursts_desc[idx].inter_frame_spacing = ifs;
        return CSL_WAKE_BURST_OK;
    }

    return CSL_WAKE_BURST_BAD_IDX;
}

uint32_t csl_wake_burst_ifs_get(size_t idx)
{
    if (idx < CSL_WAKE_BURST_MAX_CNT)
    {
        return m_state.bursts_desc[idx].inter_frame_spacing;
    }

    return 0;
}

csl_wake_burst_res_t csl_wake_burst_csma_ca_set(size_t idx, bool cca)
{
    if (m_state.is_running)
    {
        return CSL_WAKE_BURST_ALREADY_RUNNING;
    }

    if (idx < CSL_WAKE_BURST_MAX_CNT)
    {
        m_state.bursts_desc[idx].csma_ca_enabled = cca;
        return CSL_WAKE_BURST_OK;
    }

    return CSL_WAKE_BURST_BAD_IDX;
}

bool csl_wake_burst_csma_ca_get(size_t idx)
{
    if (idx < CSL_WAKE_BURST_MAX_CNT)
    {
        return m_state.bursts_desc[idx].csma_ca_enabled;
    }

    return false;
}

csl_wake_burst_res_t csl_wake_burst_ar_bit_set(bool ar)
{
    if (m_state.is_running)
    {
        return CSL_WAKE_BURST_ALREADY_RUNNING;
    }

    m_state.set_ar_bit = ar;
    return CSL_WAKE_BURST_OK;
}

bool csl_wake_burst_ar_bit_get(void)
{
    return m_state.set_ar_bit;
}

void csl_wake_burst_init(void)
{
    ret_code_t err_code;

    err_code = app_timer_create(&m_ifs_timer, APP_TIMER_MODE_REPEATED, ifs_timer_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_period_timer, APP_TIMER_MODE_SINGLE_SHOT, period_timer_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_delayed_start_timer, APP_TIMER_MODE_SINGLE_SHOT, delayed_start_timer_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_delayed_stop_timer, APP_TIMER_MODE_SINGLE_SHOT, delayed_stop_timer_handler);
    APP_ERROR_CHECK(err_code);

    UNUSED_RETURN_VALUE(csl_wake_burst_cnt_set(CSL_WAKE_BURST_DEFAULT_CNT));

    for (size_t i = 0; i < CSL_WAKE_BURST_MAX_CNT; i++)
    {
        UNUSED_RETURN_VALUE(csl_wake_burst_period_set(i, CSL_WAKE_BURST_DEFAULT_PERIOD));
        UNUSED_RETURN_VALUE(csl_wake_burst_ifs_set(i, CSL_WAKE_BURST_DEFAULT_IFS));
        UNUSED_RETURN_VALUE(csl_wake_burst_csma_ca_set(i, i >= CSL_WAKE_BURST_DEFAULT_NO_CSMA_CNT));
    }

#if ENABLE_DEBUG_PINS
    nrf_gpio_cfg_output(TP1);
    nrf_gpio_cfg_output(TP2);
    nrf_gpio_cfg_output(TP3);
    nrf_gpio_cfg_output(TP4);
    nrf_gpio_cfg_output(TP5);

    nrf_gpiote_task_configure(2, TP4, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_HIGH);
    nrf_gpiote_task_configure(3, TP5, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_HIGH);

    nrf_gpiote_task_enable(2);
    nrf_gpiote_task_enable(3);

    nrf_ppi_channel_endpoint_setup(NRF_PPI_CHANNEL2, (uint32_t) &NRF_RADIO->EVENTS_FRAMESTART, nrf_gpiote_task_addr_get(NRF_GPIOTE_TASKS_OUT_2));
    nrf_ppi_channel_endpoint_setup(NRF_PPI_CHANNEL3, (uint32_t) &NRF_RADIO->EVENTS_PHYEND,     nrf_gpiote_task_addr_get(NRF_GPIOTE_TASKS_OUT_3));

    nrf_ppi_channel_enable(NRF_PPI_CHANNEL2);
    nrf_ppi_channel_enable(NRF_PPI_CHANNEL3);
#endif
}

csl_wake_burst_res_t csl_wake_burst_send(void)
{
    otError error;

    if (m_state.is_running)
    {
        return CSL_WAKE_BURST_ALREADY_RUNNING;
    }

    if (m_state.bursts_cnt)
    {
        m_state.is_running           = true;
        m_state.current_burst        = 0;
        m_state.transmit_in_progress = false;
        m_state.frames_cnt           = 0;
        m_state.ifs_error_cnt        = 0;

        error = otThreadSetEnabled(thread_ot_instance_get(), false);
        ASSERT(error == OT_ERROR_NONE);

        error = otIp6SetEnabled(thread_ot_instance_get(), false);
        ASSERT(error == OT_ERROR_NONE);

        error = otLinkRawSetEnable(thread_ot_instance_get(), true);
        ASSERT(error == OT_ERROR_NONE);

        send_frame();
        
        ret_code_t err_code = app_timer_start(m_delayed_start_timer, APP_TIMER_TICKS(BURST_DELAY_MS), NULL);
        APP_ERROR_CHECK(err_code);

        return CSL_WAKE_BURST_OK;
    }

    return CSL_WAKE_BURST_BAD_IDX;
}

uint32_t csl_wake_txed_frames(void)
{
    return m_state.frames_cnt;
}

uint32_t csl_wake_ifs_errors(void)
{
    return m_state.ifs_error_cnt;
}

void csl_wake_burst_abort(void)
{
    burst_stop(CSL_WAKE_BURST_ABORTED);
}
