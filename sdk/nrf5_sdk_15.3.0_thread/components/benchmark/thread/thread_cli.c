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

#include <openthread/cli.h>
#include <openthread/ip6.h>
#include <openthread/thread.h>

#include "commands.h"
#include "cli_api.h"
#include "benchmark_api.h"
#include "bsp.h"
#include "nrf_cli.h"
#include "nrf_cli_rtt.h"
#include "nrf_cli_uart.h"
#include "nrf_cli_coap.h"

#include "thread_utils.h"

/**@brief CLI configuration. */
#define CLI_UART_INSTANCE_ID        1
#define CLI_UART_TX_BUF_SIZE        64
#define CLI_UART_RX_BUF_SIZE        512
#define CLI_COAP_RX_BUF_SIZE        128

/**@brief CLI instance. */
#define CLI_LOG_QUEUE_SIZE          4

/**@ brief OpenThread command buffer size. */
#define OT_CMD_BUFFER_SIZE          256

static char              m_ot_cmd_buffer[OT_CMD_BUFFER_SIZE];
static nrf_cli_t const * mp_cli_print;

NRF_CLI_UART_DEF(m_cli_uart_transport,
                 CLI_UART_INSTANCE_ID,
                 CLI_UART_TX_BUF_SIZE,
                 CLI_UART_RX_BUF_SIZE);
NRF_CLI_DEF(m_cli_uart,
            "> ",
            &m_cli_uart_transport.transport,
            '\r',
            CLI_LOG_QUEUE_SIZE);

NRF_CLI_RTT_DEF(m_cli_rtt_transport);
NRF_CLI_DEF(m_cli_rtt,
            "rtt_cli:~$ ",
            &m_cli_rtt_transport.transport,
            '\n',
            CLI_LOG_QUEUE_SIZE);

NRF_CLI_COAP_DEF_MULTI(CLI_REMOTE_INSTANCES,
                       m_cli_coap_transport,
                       CLI_COAP_RX_BUF_SIZE);
NRF_CLI_DEF_MULTI(CLI_REMOTE_INSTANCES,
                  m_cli_coap,
                  "",
                  m_cli_coap_transport,
                  '\n',
                  CLI_LOG_QUEUE_SIZE);


/**@brief OpenThread CLI output callback.
 *
 * Used by the OpenThread stack to return requested information.
 */
int ot_console_output_callback(const char * p_buff, uint16_t buff_len, void * p_context)
{
	UNUSED_VARIABLE(p_context);

	nrf_cli_fprintf(mp_cli_print, NRF_CLI_INFO, "%s", p_buff);

	return buff_len;
}

/**@brief Function for initializing the CLI.
 *
 * Serial connection and RTT console are supported.
 */
void cli_init(void)
{
    ret_code_t err_code = nrf_cli_init(&m_cli_rtt, NULL, true, true, NRF_LOG_SEVERITY_INFO);
    APP_ERROR_CHECK(err_code);

    nrf_drv_uart_config_t uart_config = NRF_DRV_UART_DEFAULT_CONFIG;
    uart_config.pseltxd               = TX_PIN_NUMBER;
    uart_config.pselrxd               = RX_PIN_NUMBER;
    uart_config.pselcts               = CTS_PIN_NUMBER;
    uart_config.pselrts               = RTS_PIN_NUMBER;
    uart_config.hwfc                  = NRF_UART_HWFC_ENABLED;

    err_code = nrf_cli_init(&m_cli_uart, &uart_config, true, true, NRF_LOG_SEVERITY_INFO);
    APP_ERROR_CHECK(err_code);

    // Init OpenThread console.
    otCliConsoleInit(thread_ot_instance_get(), ot_console_output_callback, NULL);
}

void cli_remote_init(void)
{
    for (size_t i = 0; i < CLI_REMOTE_INSTANCES; i++)
    {
        ret_code_t err_code = nrf_cli_init(m_cli_coap[i], thread_ot_instance_get(), true, false, NRF_LOG_SEVERITY_INFO);
        APP_ERROR_CHECK(err_code);

        m_cli_coap[i]->p_ctx->internal.flag.echo = true;
    }
}

/**@brief Function for starting the CLI.
 */
void cli_start(void)
{
    ret_code_t err_code = nrf_cli_start(&m_cli_uart);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_cli_start(&m_cli_rtt);
    APP_ERROR_CHECK(err_code);
}

void cli_remote_start(void)
{
    for (size_t i = 0; i < CLI_REMOTE_INSTANCES; i++)
    {
        ret_code_t err_code = nrf_cli_start(m_cli_coap[i]);
        APP_ERROR_CHECK(err_code);
    }
}

/**@brief  Function for processing CLI operations.
 */
void cli_process(void)
{
    nrf_cli_process(&m_cli_uart);
    nrf_cli_process(&m_cli_rtt);

    for(size_t i = 0; i < CLI_REMOTE_INSTANCES; i++)
    {
        nrf_cli_process(m_cli_coap[i]);
    }
}

nrf_cli_t const * cli_remote_get(size_t idx)
{
    if (idx < CLI_REMOTE_INSTANCES)
    {
        return m_cli_coap[idx];
    }
    else
    {
        return NULL;
    }
}

void cli_remote_peer_set(nrf_cli_t const * p_peer_cli, benchmark_address_context_t * p_peer_address)
{
    cli_coap_peer_set(p_peer_cli, (otIp6Address *)p_peer_address);
}

/**@brief  Function for passing the command to the OpenThread interpreter.
 */
static void cmd_ot(nrf_cli_t const * p_cli, size_t argc, char ** argv)
{
    // Remeber cli used to issue a command so results can be printed on the same interface.
    mp_cli_print    = p_cli;
	char * p_buff   = m_ot_cmd_buffer;
	size_t buff_len = OT_CMD_BUFFER_SIZE;
	size_t arg_len  = 0;

	memset(m_ot_cmd_buffer, 0, OT_CMD_BUFFER_SIZE);

	for (size_t i = 1; i < argc; i++)
	{
		if (arg_len)
		{
			buff_len -= arg_len + 1;

			if (buff_len)
			{
				p_buff[arg_len] = ' ';
			}
			p_buff += arg_len + 1;
		}

		arg_len = snprintf(p_buff, buff_len, "%s", argv[i]);

		if (arg_len >= buff_len)
		{
			nrf_cli_fprintf(p_cli, NRF_CLI_WARNING, "OT shell buffer full.");
			return;
		}
	}

	buff_len -= arg_len;

	otCliConsoleInputLine(m_ot_cmd_buffer, OT_CMD_BUFFER_SIZE - buff_len);
}

NRF_CLI_CMD_REGISTER(ot, NULL, "Issue an OpenThread CLI command", cmd_ot);
