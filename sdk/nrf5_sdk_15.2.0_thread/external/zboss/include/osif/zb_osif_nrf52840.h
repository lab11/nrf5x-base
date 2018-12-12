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
PURPOSE: Platform specific for Nordic nRF52840 SoC
*/

#ifndef __ZB_OSIF_NRF52840_H
#define __ZB_OSIF_NRF52840_H

#include <string.h>             /* memcpy */

#include "zb_types.h"
#include "zb_ringbuffer.h"

#ifdef ZB_HAVE_SERIAL
#define ZB_HAVE_IOCTX
#endif

#define ZB_VOLATILE
#define ZB_SDCC_XDATA
#define ZB_CALLBACK
#define ZB_SDCC_BANKED
#define ZB_KEIL_REENTRANT

/* At ARM all types from 1 to 4 bytes are passed to vararg with casting to 4 bytes */
typedef zb_uint32_t zb_minimal_vararg_t;

/* use macros to be able to redefine */
#ifdef KEIL
#define ZB_MEMCPY(dst, src, size)   memcpy((void*)(dst), (void*)(src), (size))
#define ZB_MEMSET(dst, val, size)   memset((void*)(dst), (val), (size))
#define ZB_MEMMOVE(dst, src, size)  memmove((void*)(dst), (void*)(src), (size))
#define ZB_MEMCMP(ptr1, ptr2, size)	memcmp((void*)(ptr1), (void*)(ptr2), (size))
#else
#define ZB_MEMCPY   memcpy
#define ZB_MEMSET   memset
#define ZB_MEMMOVE  memmove
#define ZB_MEMCMP   memcmp
#endif

#define ZB_BZERO(s,l) ZB_MEMSET((char*)(s), 0, (l))
#define ZB_BZERO2(s) ZB_BZERO(s, 2)

#define ZVUNUSED(v) (void)v

void zb_nrf52840_init(void);
void zb_nrf52840_general_init(void);
void zb_nrf52840_rf_init(void);
void zb_nrf52840_abort(void);
void zb_osif_go_idle(void);

#define ZB_PLATFORM_INIT() zb_nrf52840_init()

void zb_osif_timer_init(void);
void zb_nrf52840_timer_stop(void);
void zb_nrf52840_timer_start(void);
zb_bool_t zb_nrf52840_timer_is_on(void);

#ifdef ZB_USE_SLEEP
void zb_nrf52840_sleep_init(void);
zb_uint32_t zb_nrf52840_sleep(zb_uint32_t sleep_tmo);
#endif /*ZB_USE_SLEEP*/

void zb_nrf52840_rf_tx(zb_uint8_t channel, zb_uint8_t *buf, zb_uint8_t len);
void zb_nrf52840_rf_setChannel(zb_uint8_t channel);
void zb_nrf52840_enable_all_inter(void);
void zb_nrf52840_disable_all_inter(void);
zb_uint16_t zb_nrf52840_getRssi(void);


#if defined ZB_TRACE_OVER_USART && defined ZB_TRACE_LEVEL
#define ZB_SERIAL_FOR_TRACE
#endif

#define ZB_START_HW_TIMER()    zb_nrf52840_timer_start()
#define ZB_STOP_HW_TIMER()     zb_nrf52840_timer_stop()
#define ZB_CHECK_TIMER_IS_ON() zb_nrf52840_timer_is_on()

#define ZB_TRANSPORT_NONBLOCK_ITERATION() 0

#define ZB_ENABLE_ALL_INTER()  zb_nrf52840_enable_all_inter()
#define ZB_DISABLE_ALL_INTER() zb_nrf52840_disable_all_inter()

#define ZB_OSIF_GLOBAL_LOCK()         ZB_DISABLE_ALL_INTER()
#define ZB_OSIF_GLOBAL_UNLOCK()       ZB_ENABLE_ALL_INTER()

#define ZB_ABORT zb_nrf52840_abort

#define ZB_GO_IDLE() zb_osif_go_idle()

#if defined ZB_HAVE_SERIAL

#ifndef ZB_NRF_TRACE
#define SERIAL_BUF_SIZE 256

/*Declare ring buffer for UART operations*/
ZB_RING_BUFFER_DECLARE(serial_iobuf, zb_uint8_t, SERIAL_BUF_SIZE);

/*User UART interrupt handler definition*/
typedef void (*zb_osif_uart_user_irq_handler)(void);
#endif

#if !defined ZB_NRF_TRACE || defined ZB_NRF_TRACE_RX_ENABLE

/*UART Receive data callback function definition*/
typedef void (*zb_osif_uart_byte_received_cb_t)(zb_uint8_t byte);

#endif

typedef struct zb_serial_ctx_s
{
  /* 04/02/2018 EE CR:MINOR Move it after tx_buf_cap: align big-to-small struct members. */
  zb_uint8_t    inited;
#ifndef ZB_NRF_TRACE
  serial_iobuf_t   tx_buf;
  zb_byte_array_t* p_tx_buf;
  zb_ushort_t      tx_buf_cap;
/* 04/02/2018 EE CR:MINOR If 7 bits are not used, why not use the entire byte for a flag? */
  zb_bitfield_t    tx_in_progress:1; /* if set, we are waiting for tx complete int */
  zb_bitfield_t    align:7; /* if set, we are waiting for tx complete int */
  zb_osif_uart_byte_received_cb_t byte_received_cb;
#endif
}
zb_serial_ctx_t;

typedef struct zb_io_ctx_s
{
    zb_serial_ctx_t serial_ctx;
}
zb_io_ctx_t;

#if defined ZB_SERIAL_FOR_TRACE
void zb_osif_serial_flush(void);
#if defined ZB_NRF_TRACE && (defined ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST)
#define ZB_OSIF_SERIAL_FLUSH()    zb_osif_serial_flush()
#else
#define ZB_OSIF_SERIAL_FLUSH()
#endif /*defined ZB_NRF_TRACE && (defined ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST)*/
#endif /*defined ZB_SERIAL_FOR_TRACE*/

#endif  /* ZB_HAVE_SERIAL */

#ifdef ZB_USE_OSIF_OTA_ROUTINES
#define OTA_HEADER_SIZE 56      /* sizeof(zb_zcl_ota_upgrade_file_header_t), no optional fields */
#define OTA_IMAGE_BLOCK_DATA_SIZE_MAX 60 /* from zb_zcl_ota_upgrade.h */

typedef struct zb_osif_flash_device_s {
  zb_uint32_t base_address;
  zb_uint32_t img_srv_address;
  /* TODO: these fields are used only for OTA server, may put it under some define... */
  zb_uint8_t ota_file_header[OTA_HEADER_SIZE];
  zb_uint8_t transmit_buffer[OTA_IMAGE_BLOCK_DATA_SIZE_MAX];
} zb_osif_flash_device_t;
#endif

/**
 * @brief Read IEEE long address from FICR registers.
 *
 * @param[out] ieee_eui64  Pointer to a long address structure to be filled.
 */
void zb_osif_get_ieee_eui64(zb_ieee_addr_t ieee_eui64);

#if defined ZB_NRF52840_RADIO_STATISTICS
typedef struct zb_nrf52840_radio_stats_s
{
  zb_uint32_t rx_successful;
  zb_uint32_t rx_err_none; /* Error Code: 0x00 */
  zb_uint32_t rx_err_invalid_frame; /* Error Code: 0x01 */
  zb_uint32_t rx_err_invalid_fcs; /* Error Code: 0x02 */
  zb_uint32_t rx_err_invalid_dest_addr; /* Error Code: 0x03 */
  zb_uint32_t rx_err_runtime; /* Error Code: 0x04 */
  zb_uint32_t rx_err_timeslot_ended; /* Error Code: 0x05 */
  zb_uint32_t rx_err_aborted; /* Error Code: 0x06 */
  
  zb_uint32_t tx_successful;
  zb_uint32_t tx_err_none; /* Error Code: 0x00 */
  zb_uint32_t tx_err_busy_channel; /* Error Code: 0x01 */
  zb_uint32_t tx_err_invalid_ack; /* Error Code: 0x02 */
  zb_uint32_t tx_err_no_mem; /* Error Code: 0x03 */
  zb_uint32_t tx_err_timeslot_ended; /* Error Code: 0x04 */
  zb_uint32_t tx_err_no_ack; /* Error Code: 0x05 */
  zb_uint32_t tx_err_aborted; /* Error Code: 0x06 */
  zb_uint32_t tx_err_timeslot_denied; /* Error Code: 0x07 */
  
} zb_nrf52840_radio_stats_t;

zb_nrf52840_radio_stats_t* zb_nrf52840_get_radio_stats();
#endif /* defined ZB_NRF52840_RADIO_STATISTICS */

#endif /* __ZB_OSIF_NRF52840_H */
