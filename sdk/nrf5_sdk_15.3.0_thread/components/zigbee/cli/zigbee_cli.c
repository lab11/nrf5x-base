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
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "sdk_config.h"
#include "nrf.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_fstorage_nvmc.h"

#include "app_timer.h"

#include "app_error.h"
#include "app_util.h"

#include "nrf_cli.h"
#include "nrf_cli_types.h"

#include "boards.h"

#include "nrf_log_instance.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_flash.h"

#if defined(APP_USBD_ENABLED) && APP_USBD_ENABLED
#define CLI_OVER_USB_CDC_ACM 1
#else
#define CLI_OVER_USB_CDC_ACM 0
#endif

#if CLI_OVER_USB_CDC_ACM
#include "nrf_cli_cdc_acm.h"
#include "nrf_drv_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"
#endif //CLI_OVER_USB_CDC_ACM

#if defined(TX_PIN_NUMBER) && defined(RX_PIN_NUMBER)
#define CLI_OVER_UART 1
#else
#define CLI_OVER_UART 0
#endif

#if CLI_OVER_UART
#include "nrf_cli_uart.h"
#endif

#include "zigbee_cli.h"

#if CLI_OVER_USB_CDC_ACM
#if NRF_LOG_ENABLED
/** @brief Name of the submodule used for logger messaging.
 */
#define NRF_LOG_SUBMODULE_NAME cli

NRF_LOG_INSTANCE_REGISTER(ZIGBEE_CLI_LOG_NAME, NRF_LOG_SUBMODULE_NAME,
                          ZIGBEE_CLI_CONFIG_INFO_COLOR,
                          ZIGBEE_CLI_CONFIG_DEBUG_COLOR,
                          ZIGBEE_CLI_CONFIG_LOG_INIT_FILTER_LEVEL,
                          ZIGBEE_CLI_CONFIG_LOG_ENABLED ?
                                  ZIGBEE_CLI_CONFIG_LOG_LEVEL : NRF_LOG_SEVERITY_NONE);

// This structure keeps reference to the logger instance used by this module.
typedef struct {
    NRF_LOG_INSTANCE_PTR_DECLARE(p_log)
} log_ctx_t;

// Logger instance used by this module.
static log_ctx_t m_log = {
    NRF_LOG_INSTANCE_PTR_INIT(p_log, ZIGBEE_CLI_LOG_NAME, NRF_LOG_SUBMODULE_NAME)
};
#endif // NRF_LOG_ENABLED
#endif // CLI_OVER_USB_CDC_ACM

/**@file
 * @defgroup zigbee_cli Zigbee CLI zigbee_cli.c
 *
 * @{
 *
 */

/* CLI Agent endpoint */
static zb_uint8_t cli_ep;

/* Counter timer. */
APP_TIMER_DEF(m_timer_0);

#ifdef ZIGBEE_CLI_DEBUG
/* Debug mode indicator. */
static zb_bool_t m_debug_mode = ZB_FALSE;
#endif

#ifdef ZIGBEE_CLI_DEBUG
/* Zigbee stack processing suspension indicator. */
static zb_bool_t m_suspended = ZB_FALSE;
#endif

#if CLI_OVER_USB_CDC_ACM

/**
 * @brief Enable power USB detection
 *
 * Configure if example supports USB port connection
 */
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif


static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            app_usbd_stop();
            break;
        case APP_USBD_EVT_POWER_READY:
            app_usbd_start();
            break;
        default:
            break;
    }
}

#endif //CLI_OVER_USB_CDC_ACM

/**
 * @brief Command line interface instance
 * */
#define CLI_EXAMPLE_LOG_QUEUE_SIZE  (4)

#if CLI_OVER_USB_CDC_ACM
NRF_CLI_CDC_ACM_DEF(m_cli_cdc_acm_transport);
NRF_CLI_DEF(m_cli_cdc_acm,
            "> ",
            &m_cli_cdc_acm_transport.transport,
            '\r',
            CLI_EXAMPLE_LOG_QUEUE_SIZE);
#endif //CLI_OVER_USB_CDC_ACM

#if CLI_OVER_UART
NRF_CLI_UART_DEF(m_cli_uart_transport, 0, 64, 256);
NRF_CLI_DEF(m_cli_uart,
            "> ",
            &m_cli_uart_transport.transport,
            '\r',
            CLI_EXAMPLE_LOG_QUEUE_SIZE);
#endif

static void timer_handle(void * p_context)
{
    UNUSED_PARAMETER(p_context);
}

static void cli_start(void)
{
    ret_code_t ret;

#if CLI_OVER_USB_CDC_ACM
    ret = nrf_cli_start(&m_cli_cdc_acm);
    APP_ERROR_CHECK(ret);
#endif

#if CLI_OVER_UART
    ret = nrf_cli_start(&m_cli_uart);
    APP_ERROR_CHECK(ret);
#endif
}

static void cli_init(void)
{
    ret_code_t ret;

#if CLI_OVER_USB_CDC_ACM
    ret = nrf_cli_init(&m_cli_cdc_acm, NULL, true, true, NRF_LOG_SEVERITY_ERROR);
    APP_ERROR_CHECK(ret);
#endif

#if CLI_OVER_UART
    nrf_drv_uart_config_t uart_config = NRF_DRV_UART_DEFAULT_CONFIG;
    uart_config.pseltxd = TX_PIN_NUMBER;
    uart_config.pselrxd = RX_PIN_NUMBER;
    #ifdef HWFC
    uart_config.pselcts = CTS_PIN_NUMBER;
    uart_config.pselrts = RTS_PIN_NUMBER;
    uart_config.hwfc    = NRF_UART_HWFC_ENABLED;
    #endif
    ret = nrf_cli_init(&m_cli_uart, &uart_config, true, true, NRF_LOG_SEVERITY_ERROR);
    APP_ERROR_CHECK(ret);
#endif
}


static void usbd_init(void)
{
#if CLI_OVER_USB_CDC_ACM
    ret_code_t ret;
    static const app_usbd_config_t usbd_config = {
        .ev_handler = app_usbd_event_execute,
        .ev_state_proc = usbd_user_ev_handler
    };

    app_usbd_serial_num_generate();

    ret = app_usbd_init(&usbd_config);
    APP_ERROR_CHECK(ret);

    app_usbd_class_inst_t const * class_cdc_acm =
            app_usbd_cdc_acm_class_inst_get(&nrf_cli_cdc_acm);
    ret = app_usbd_class_append(class_cdc_acm);
    APP_ERROR_CHECK(ret);

    if (USBD_POWER_DETECTION)
    {
        ret = app_usbd_power_events_enable();
        APP_ERROR_CHECK(ret);
    }
    else
    {
        NRF_LOG_INST_INFO(m_log.p_log, "No USB power detection enabled\r\nStarting USB now");

        app_usbd_enable();
        app_usbd_start();
    }

    /* Give some time for the host to enumerate and connect to the USB CDC port */
    nrf_delay_ms(1000);
#endif
}

static void cli_process(void)
{
#if CLI_OVER_USB_CDC_ACM
    nrf_cli_process(&m_cli_cdc_acm);
#endif

#if CLI_OVER_UART
    nrf_cli_process(&m_cli_uart);
#endif
}

void zb_cli_init(uint8_t ep)
{
    ret_code_t ret;

    /* Set the endpoint number */
    cli_ep = ep;

    ret = app_timer_create(&m_timer_0, APP_TIMER_MODE_REPEATED, timer_handle);
    APP_ERROR_CHECK(ret);

    ret = app_timer_start(m_timer_0, APP_TIMER_TICKS(1000), NULL);
    APP_ERROR_CHECK(ret);

    cli_init();

    usbd_init();

    cli_start();
}

void zb_cli_process(void)
{
#if CLI_OVER_USB_CDC_ACM && APP_USBD_CONFIG_EVENT_QUEUE_ENABLE
        while (app_usbd_event_queue_process())
        {
            /* Nothing to do */
        }
#endif
        cli_process();
}

/**@brief Returns the number of the Endpoint used by the CLI.
 */
zb_uint8_t zb_get_cli_endpoint(void)
{
    return cli_ep;
}

#ifdef ZIGBEE_CLI_DEBUG
/**@brief Sets the debug mode.
 */
zb_void_t zb_cli_debug_set(zb_bool_t debug)
{
    m_debug_mode = debug;
}
#endif

#ifdef ZIGBEE_CLI_DEBUG
/**@brief Gets the debug mode.
 */
zb_bool_t zb_cli_debug_get(zb_void_t)
{
    return m_debug_mode;
}
#endif

#ifdef ZIGBEE_CLI_DEBUG
/**@brief Function for suspending the processing of the Zigbee main loop.
 */
zb_void_t zb_cli_suspend(zb_void_t)
{
    m_suspended = ZB_TRUE;
}
#endif

#ifdef ZIGBEE_CLI_DEBUG
/**@brief Function for resuming the processing of the Zigbee main loop.
 */
zb_void_t zb_cli_resume(zb_void_t)
{
    m_suspended = ZB_FALSE;
}
#endif

#ifdef ZIGBEE_CLI_DEBUG
/**@brief Function for getting the state of the Zigbee stack processing suspension.
 */
zb_bool_t zb_cli_stack_is_suspended(zb_void_t)
{
    return m_suspended;
}
#endif

/** @} */
