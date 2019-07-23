/**
 * Copyright (c) 2018, Nordic Semiconductor ASA
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

#include "timer_api.h"
#include "zboss_api.h"
#include "nrf_log_ctrl.h"
#include "zb_error_handler.h"

static timer_cb_t m_timer_cb;
static uint32_t   m_timeout_ms;
static uint32_t   m_timeout_beacon_int;
static uint32_t   m_delta_beacon_int;
static uint32_t   m_stop_beacon_int;
static zb_bool_t  m_enabled = ZB_FALSE;

/**
 * @brief A callback called on timer timeout.
 *
 * @param[in] a reference number to ZBOSS memory buffer (unused).
 */
static zb_void_t timer_timeout_zboss_cb(zb_uint8_t param)
{
    UNUSED_PARAMETER(param);

    if ((m_enabled == ZB_TRUE) && (m_timer_cb))
    {
        m_timer_cb();
    }
}

uint32_t timer_capture(void)
{
    if (m_enabled)
    {
        return (ZB_TIMER_GET() - m_delta_beacon_int);
    }
    else
    {
        return (m_stop_beacon_int - m_delta_beacon_int);
    }
}

void timer_init(void)
{
    m_timer_cb           = NULL;
    m_timeout_ms         = 0;
    m_delta_beacon_int   = 0;
    m_timeout_beacon_int = 0;
    m_enabled            = ZB_FALSE;
    m_stop_beacon_int    = ZB_TIMER_GET();
}

void timer_set(uint32_t timeout_ms, timer_cb_t p_callback)
{
    zb_ret_t zb_err_code;

    m_timer_cb   = p_callback;
    m_timeout_ms = timeout_ms;

    if (m_enabled == ZB_TRUE)
    {
        m_timeout_beacon_int = ZB_TIMER_GET() + ZB_MILLISECONDS_TO_BEACON_INTERVAL(m_timeout_ms);
        zb_err_code = ZB_SCHEDULE_ALARM(timer_timeout_zboss_cb, 0, ZB_MILLISECONDS_TO_BEACON_INTERVAL(m_timeout_ms));
        ZB_ERROR_CHECK(zb_err_code);
        m_timeout_ms = 0;
    }
}

void timer_start(void)
{
    zb_ret_t zb_err_code;

    if (m_enabled == ZB_FALSE)
    {
        m_delta_beacon_int += ZB_TIMER_GET() - m_stop_beacon_int;
        m_enabled          = ZB_TRUE;

        if (m_timeout_ms != 0)
        {
            m_timeout_beacon_int = ZB_TIMER_GET() + ZB_MILLISECONDS_TO_BEACON_INTERVAL(m_timeout_ms);
            zb_err_code = ZB_SCHEDULE_ALARM(timer_timeout_zboss_cb, 0, ZB_MILLISECONDS_TO_BEACON_INTERVAL(m_timeout_ms));
            ZB_ERROR_CHECK(zb_err_code);
            m_timeout_ms = 0;
        }
    }
}

void timer_stop(void)
{
    if (m_enabled == ZB_TRUE)
    {
        m_stop_beacon_int = ZB_TIMER_GET();
        m_enabled = ZB_FALSE;
        if (m_timeout_beacon_int > ZB_TIMER_GET())
        {
            m_timeout_ms = ZB_TIME_BEACON_INTERVAL_TO_MSEC(m_timeout_beacon_int - ZB_TIMER_GET());
        }
        else
        {
            m_timeout_ms = 0;
        }
        UNUSED_RETURN_VALUE(ZB_SCHEDULE_ALARM_CANCEL(timer_timeout_zboss_cb, ZB_ALARM_ANY_PARAM));
    }
}

uint32_t timer_ticks_to_ms(uint32_t ticks)
{
    return ZB_TIME_BEACON_INTERVAL_TO_MSEC(ticks);
}
