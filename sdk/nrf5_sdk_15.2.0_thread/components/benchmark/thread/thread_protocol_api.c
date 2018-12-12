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

#include "app_error.h"
#include "bsp_thread.h"
#include "csl_wake.h"
#include "nrf_log.h"
#include "protocol_api.h"
#include "thread_utils.h"
#include "thread_coap_utils.h"

#include <openthread/platform/platform-softdevice.h>
#include <openthread/thread.h>

static void thread_bsp_init(void)
{
    uint32_t err_code = bsp_thread_init(thread_ot_instance_get());
    APP_ERROR_CHECK(err_code);
}

static void thread_coap_handler_default(void                * p_context,
                                        otCoapHeader        * p_header,
                                        otMessage           * p_message,
                                        const otMessageInfo * p_message_info)
{
    UNUSED_VARIABLE(p_context);
    UNUSED_VARIABLE(p_header);
    UNUSED_VARIABLE(p_message);
    UNUSED_VARIABLE(p_message_info);

    NRF_LOG_INFO("Received CoAP message that does not match any request or resource");
}

static void thread_coap_init(void)
{
    otError error = otCoapStart(thread_ot_instance_get(), OT_DEFAULT_COAP_PORT);
    ASSERT(error == OT_ERROR_NONE);

    otCoapSetDefaultHandler(thread_ot_instance_get(), thread_coap_handler_default, NULL);
}

void protocol_state_changed_callback(uint32_t flags, void * p_context)
{
    if (flags & OT_CHANGED_THREAD_ROLE)
    {
        NRF_LOG_INFO("State changed! Flags: 0x%08x Current role: %d",
                     flags,
                     otThreadGetDeviceRole(p_context));
    }
}

static void thread_instance_init(void)
{
    thread_configuration_t thread_configuration =
    {
        .role                  = RX_ON_WHEN_IDLE,
        .autocommissioning     = true,
        .poll_period           = 2500,
        .default_child_timeout = 10,
    };

    thread_init(&thread_configuration);
    thread_state_changed_callback_set(protocol_state_changed_callback);
}

bool protocol_is_error(uint32_t error_code)
{
    return (otError)error_code != OT_ERROR_NONE;
}

void protocol_init(void)
{
    thread_instance_init();
    thread_coap_init();
    thread_bsp_init();
    csl_wake_burst_init();
}

void protocol_process(void)
{
    thread_process();
}

void protocol_sleep(void)
{
    thread_sleep();
}

void protocol_soc_evt_handler(uint32_t soc_evt)
{
    otSysSoftdeviceSocEvtHandler(soc_evt);
}
