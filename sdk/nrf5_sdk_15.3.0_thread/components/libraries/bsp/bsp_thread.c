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

#include "bsp_thread.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "bsp.h"
#include "bsp_config.h"
#include "nrf_assert.h"

#include <openthread/instance.h>
#include <openthread/icmp6.h>
#include <openthread/thread.h>

#ifndef BSP_SIMPLE
#include "app_timer.h"
#endif

static bsp_indication_thread_t        m_connection_state;
static bsp_indication_commissioning_t m_commissioning_state;
static uint8_t                        m_indicating_ping;

#if (LEDS_NUMBER > 0) && !(defined BSP_SIMPLE)
APP_TIMER_DEF(m_commissioning_led_timer_id);
APP_TIMER_DEF(m_ping_led_timer_id);

static otIcmp6Handler m_icmp6_handler;
#endif // LEDS_NUMBER > 0 && !(defined BSP_SIMPLE)

#if LEDS_NUMBER > 0 && !(defined BSP_SIMPLE)

/**@brief       Configure leds to indicate required commissioning state.
 * @param[in]   indicate   State to be indicated.
 */
static uint32_t bsp_commissioning_led_indication(bsp_indication_commissioning_t indicate)
{
    uint32_t err_code   = NRF_SUCCESS;
    uint32_t next_delay = 0;

    m_commissioning_state = indicate;

    switch (indicate)
    {
        case BSP_INDICATE_COMMISSIONING_NONE:
            bsp_board_led_off(BSP_LED_THREAD_COMMISSIONING);
            break;

        case BSP_INDICATE_COMMISSIONING_NOT_COMMISSIONED:
            if (bsp_board_led_state_get(BSP_LED_THREAD_COMMISSIONING))
            {
                bsp_board_led_off(BSP_LED_THREAD_COMMISSIONING);
                next_delay = THREAD_NOT_COMMISSIONED_LED_OFF_INTERVAL;
            }
            else
            {
                bsp_board_led_on(BSP_LED_THREAD_COMMISSIONING);
                next_delay = THREAD_NOT_COMMISSIONED_LED_ON_INTERVAL;
            }

            err_code = app_timer_start(m_commissioning_led_timer_id,
                                       APP_TIMER_TICKS(next_delay),
                                       NULL);
            break;

        case BSP_INDICATE_COMMISSIONING_IN_PROGRESS:
            bsp_board_led_invert(BSP_LED_THREAD_COMMISSIONING);

            err_code = app_timer_start(m_commissioning_led_timer_id,
                                       APP_TIMER_TICKS(THREAD_COMMISSIONING_IN_PROGRESS_INTERVAL),
                                       NULL);
            break;

        case BSP_INDICATE_COMMISSIONING_SUCCESS:
            bsp_board_led_on(BSP_LED_THREAD_COMMISSIONING);
            break;
    }

    return err_code;
}

/**@brief Handle events from commissioning led timer.
 *
 * @note Timer handler does not support returning an error code.
 * Errors from bsp_led_indication() are not propagated.
 *
 * @param[in]   p_context   Parameter registered in timer start function.
 */
static void commissioning_led_timer_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    UNUSED_VARIABLE(bsp_commissioning_led_indication(m_commissioning_state));
}

/**@brief Configure leds to indicate received ping state.
 */
static uint32_t bsp_ping_led_indication(void)
{
    uint32_t err_code = NRF_SUCCESS;

    m_indicating_ping--;

    if (m_indicating_ping)
    {
        if (bsp_board_led_state_get(BSP_LED_THREAD_COMMISSIONING))
        {
            bsp_board_led_off(BSP_LED_THREAD_CONNECTION);
            bsp_board_led_off(BSP_LED_THREAD_COMMISSIONING);
        }
        else
        {
            bsp_board_led_on(BSP_LED_THREAD_CONNECTION);
            bsp_board_led_on(BSP_LED_THREAD_COMMISSIONING);
        }

        err_code = app_timer_start(m_ping_led_timer_id,
                                   APP_TIMER_TICKS(THREAD_PING_INTERVAL),
                                   NULL);
    }
    else
    {
        err_code = bsp_commissioning_led_indication(m_commissioning_state);

        if (err_code == NRF_SUCCESS)
        {
            err_code = bsp_indication_set((bsp_indication_t) m_connection_state);
        }
    }

    return err_code;
}

/**@brief Handle events from ping led timer.
 *
 * @note Timer handler does not support returning an error code.
 * Errors from bsp_led_indication() are not propagated.
 */
static void ping_led_timer_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    UNUSED_VARIABLE(bsp_ping_led_indication());
}

static void icmp_receive_callback(void                * p_context,
                                  otMessage           * p_message,
                                  const otMessageInfo * p_message_info,
                                  const otIcmp6Header * p_icmp_header)
{
    (void) p_context;
    (void) p_message;
    (void) p_message_info;

    if (p_icmp_header->mType == OT_ICMP6_TYPE_ECHO_REQUEST)
    {
        UNUSED_VARIABLE(bsp_thread_ping_indication_set());
    }
}
#endif // LEDS_NUMBER > 0 && !(defined BSP_SIMPLE)

/**@brief Set Thread connection LED according to current connection state.
 */
static void handle_role_change(void * p_context, otDeviceRole role)
{
    (void) p_context;

    switch(role)
    {
        case OT_DEVICE_ROLE_CHILD:
        case OT_DEVICE_ROLE_ROUTER:
        case OT_DEVICE_ROLE_LEADER:
            UNUSED_VARIABLE(bsp_thread_indication_set(BSP_INDICATE_THREAD_CONNECTED));
            break;

        case OT_DEVICE_ROLE_DISABLED:
            UNUSED_VARIABLE(bsp_thread_indication_set(BSP_INDICATE_THREAD_DISABLED));
            break;

        case OT_DEVICE_ROLE_DETACHED:
        default:
            UNUSED_VARIABLE(bsp_thread_indication_set(BSP_INDICATE_THREAD_DETACHED));
            break;
    }
}

/**@brief Callback from OpenThread notifying that device state changed.
 */
static void state_changed_callback(uint32_t flags, void * p_context)
{
    if (flags & OT_CHANGED_THREAD_ROLE)
    {
        handle_role_change(p_context, otThreadGetDeviceRole(p_context));
    }
}


ret_code_t bsp_thread_init(otInstance * p_instance)
{
    uint32_t err_code = NRF_SUCCESS;

    ASSERT(p_instance);

    m_connection_state    = BSP_INDICATE_THREAD_DISABLED;
    m_commissioning_state = BSP_INDICATE_COMMISSIONING_NONE;

#if (LEDS_NUMBER > 0) && !(defined BSP_SIMPLE)
    otError error;

    memset(&m_icmp6_handler, 0, sizeof(m_icmp6_handler));
    m_icmp6_handler.mReceiveCallback = icmp_receive_callback;

    err_code = app_timer_create(&m_commissioning_led_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                commissioning_led_timer_handler);

    if (err_code == NRF_SUCCESS)
    {
        err_code = app_timer_create(&m_ping_led_timer_id,
                                    APP_TIMER_MODE_SINGLE_SHOT,
                                    ping_led_timer_handler);
    }

    error = otSetStateChangedCallback(p_instance, state_changed_callback, p_instance);
    ASSERT(error == OT_ERROR_NONE);

    error = otIcmp6RegisterHandler(p_instance, &m_icmp6_handler);
    ASSERT((error == OT_ERROR_NONE) || (error == OT_ERROR_ALREADY));
#endif // LEDS_NUMBER > 0 && !(defined BSP_SIMPLE)

    return err_code;
}

void bsp_thread_deinit(otInstance * p_instance)
{
#if (LEDS_NUMBER > 0) && !(defined BSP_SIMPLE)
    UNUSED_VARIABLE(app_timer_stop(m_commissioning_led_timer_id));
    UNUSED_VARIABLE(app_timer_stop(m_ping_led_timer_id));

    bsp_board_led_off(BSP_LED_THREAD_CONNECTION);
    bsp_board_led_off(BSP_LED_THREAD_COMMISSIONING);

    otRemoveStateChangeCallback(p_instance, state_changed_callback, p_instance);
#endif // LEDS_NUMBER > 0 && !(defined BSP_SIMPLE)

    UNUSED_VARIABLE(bsp_thread_indication_set(BSP_INDICATE_THREAD_DISABLED));
}

ret_code_t bsp_thread_indication_set(bsp_indication_thread_t indicate)
{
    m_connection_state = indicate;

    return bsp_indication_set((bsp_indication_t) indicate);
}

ret_code_t bsp_thread_commissioning_indication_set(bsp_indication_commissioning_t indicate)
{
    uint32_t err_code = NRF_SUCCESS;

    m_commissioning_state = indicate;

#if (LEDS_NUMBER > 0) && !(defined BSP_SIMPLE)
    err_code = bsp_commissioning_led_indication(indicate);
#endif // LEDS_NUMBER > 0 && !(defined BSP_SIMPLE)

    return err_code;
}

ret_code_t bsp_thread_ping_indication_set(void)
{
    uint32_t err_code = NRF_SUCCESS;

    m_indicating_ping = THREAD_PING_NUM_TOGGLES;

#if (LEDS_NUMBER > 0) && !(defined BSP_SIMPLE)
    err_code = bsp_ping_led_indication();
#endif // LEDS_NUMBER > 0 && !(defined BSP_SIMPLE)

    return err_code;
}

