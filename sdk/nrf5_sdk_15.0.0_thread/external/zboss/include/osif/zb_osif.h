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
 * PURPOSE: Main header for OS and platform depenednt stuff
*/

#ifndef ZB_OSIF_H
#define ZB_OSIF_H 1

/**
 @internal

 \addtogroup ZB_OSIF */
/*! @{ */

/**
@par OS/HW abstraction
OS/HW platform abstraction is necessary to achieve high portability.
C language tool to separate abstraction layer is C preprocessor.

Main idea is to avoid number of ifdefs related to portability in the code, and
decrease number of ifdefs in the header files not related to the OS abstraction
layer.

Platform abstraction is implemented as C functions placed into OS abstraction
layers and platform-dependent global typedefs and definitions placed into header
files.  All platform-related stuff is in osif/ directory.  Global definitions
and typedefs can be used anywhere - that is why on the architecture picture OS
abstraction layer depicted as global.

Following things are platform-dependent:
*   typedefs for base types (8-bit controller vs 32-bit Linux device)
*   definitions for different 8051 compilers (SDCC and Keil)
*   transceiver i/o (interrupts handling for 8051 vs file i/o in Linux);
*   wait for i/o (device sleep for 8051, wait in select() in Linux)
*   trace i/o (UART for 8051, file in Linux);
*   MAC traffic dump (UART for 8051, file in Linux);
*   Timer (8051 timer at device, select() timeout in Linux)

 */

#ifdef __IAR_SYSTEMS_ICC__
#ifndef ZB_IAR
#define ZB_IAR
#endif
#endif
 /**
OSIF platform selection. One of pre-defined platform should be selected in
zb_config.h configurations
*/
#if   defined ZB_NRF_52840
  #include "zb_osif_nrf52840.h"

#else
  #error Port me!
#endif

#include "zb_ringbuffer.h"

#ifndef ZB_PLATFORM_INIT
#error 1
//#define ZB_PLATFORM_INIT()
#endif

#ifndef DOXYGEN

#define MAIN() int main(int argc, char *argv[])
#define FAKE_ARGV
#define ARGV_UNUSED ZVUNUSED(argc) ; ZVUNUSED(argv)
#define MAIN_RETURN(v) return (v)

#endif /* ndef DOXYGEN */

#ifndef ZB_SET_TRACE_TRANSPORT
/*
 * If platform supports choosing transport at runtime
 * (i.e. Telink can choose between UART and USB debug interface)
 * define this macro.
 * Note that this is intended to be called before ZB_TRACE_INIT is called as
 * only one interface will be configured and the rest of them will be left as is
 * so that it would be usable from non-ZigBee purposes (application) if needed.
 */
#define ZB_SET_TRACE_TRANSPORT(transport)
#endif

/*! @} */

/* common osif API */
zb_uint32_t zb_random_seed();
zb_uint32_t zb_get_utc_time();

zb_uint32_t osif_get_time_ms();

/* note: that api is only for some platforms */
zb_ret_t osif_set_transmit_power(zb_uint8_t channel, zb_int8_t power);
void osif_set_default_trasnmit_powers(zb_int8_t *tx_powers);

/*! \addtogroup uart */
/*! @{ */

#if defined ZB_HAVE_SERIAL || defined DOXYGEN

/* Serial interface (trace, traffic dump, serial transport) */

/**
   Initialize UART low level.

   If ZBOSS uses UART for trace or traffic dump, it calls zb_osif_serial_init()
   itself.
   If UART is used by application, application must call zb_osif_serial_init().
 */
void zb_osif_serial_init();

/**
   Setup callback to be called when single byte reecived over UART

   @param hnd user's rx callback
 */
void zb_osif_set_uart_byte_received_cb(void (*hnd)(zb_uint8_t));

/**
   Set user's buffer to be used by UART TX logic.

   ZBOSS normally uses its internal UART buffer. The buffer is not too big -
   about 200 bytes which is enough for its usage by ZBOSS (trace).
   Some applications needs large io buffers. So declare there type placeholder for use ringbuffer zb_byte_array_t;
   by default application will set user tx buffer to stack tx buffer and access it through pointer;
   User can override this pointer to use it's own serial buffer.

   @param buf_ptr user's buffer
   @param capacity buffer capacity
 */
void zb_osif_set_user_io_buffer(zb_byte_array_t *buf_ptr, zb_ushort_t capacity);

/**
   Set user callback that will be called from uart IRQ handler.
   Replaces zboss uart irq handler.

   Note: that call is to be used when application initializes and uses UART
   itself.
   Do not use this call if use ZBOSS serial port API.
   Call to that functions disables ZBOSS debug trace and traffic dump.
*/
void zb_osif_set_uart_user_irq_handler(void (*irq_hnd)());

#endif
#if defined ZB_BINARY_TRACE || defined ZB_HAVE_SERIAL || defined ZB_TRACE_OVER_USART || defined DOXYGEN || defined ZB_NSNG

/**
   TX data over UART

   Put data to internal buffer to be transmitted over UART.
   It is guaranteed that all data will be sent.
   Block is no space in the buffer waiting for previous TX complete.

   Note: this is low level routine. Its direct usage may conflict with ZBOSS
   debug trace and traffic dump (if enabled).

   @param buf data buffer
   @param len data length.
 */
void zb_osif_serial_put_bytes(zb_uint8_t *buf, zb_short_t len);
#endif

/*! @} */

#ifdef ZB_TRACE_OVER_SIF
void zb_osif_sif_put_bytes(zb_uint8_t *buf, zb_short_t len);
void zb_osif_sif_init();
zb_void_t zb_osif_sif_debug_trace(zb_uint8_t param);
#endif

#ifdef ZB_HAVE_FILE
/* File  */
zb_osif_file_t *zb_osif_file_open(zb_char_t *name, zb_char_t *mode);
zb_osif_file_t *zb_osif_init_trace(zb_char_t *name);
zb_osif_file_t *zb_osif_file_stdout();
void zb_osif_file_close(zb_osif_file_t *f);
void zb_osif_trace_printf(zb_osif_file_t *f, zb_char_t *format, ...);
void zb_osif_trace_vprintf(zb_osif_file_t *f, zb_char_t *format, va_list arglist);
void zb_osif_trace_lock();
void zb_osif_trace_unlock();
zb_osif_file_t *zb_osif_init_dump(zb_char_t *name);
int zb_osif_file_write(zb_osif_file_t *f, zb_uint8_t *buf, zb_uint_t len);
int zb_osif_file_flush(zb_osif_file_t *f);
void zb_osif_trace_get_time(zb_uint_t *sec, zb_uint_t *msec);
zb_osif_file_t *zb_osif_popen(zb_char_t *arg);
#endif

/*! \addtogroup zb_platform */
/*! @{ */

/**
   Platform dependent soft reset
*/
#if defined ZB_PLATFORM_CORTEX_M3 || defined ZB_PLATFORM_CORTEX_M4 || \
    defined ZB_PLATFORM_LCGW_REMOTE_SOLUTION
zb_void_t zb_reset_silently(zb_uint8_t param);
#endif /*#if defined ZB_PLATFORM_CORTEX_M3 || defined ZB_PLATFORM_CORTEX_M4*/
zb_void_t zb_reset(zb_uint8_t param);

/*! @} */

/**
 *
 *  @brief Get stack current parameters.
 *
 *  @param s_head      [OUT] - pointer to stack head.
 *  @param s_size      [OUT] - current size of stack.
 *  @param s_direction [OUT] - stack growing direction (ZB_TRUE - UP, ZB_FALSE - DOWN).
 */
void zb_osif_get_stack(zb_uint8_t **s_head, zb_uint32_t *s_size, zb_uint8_t *s_direction);

#if defined ZB_USE_NVRAM || defined doxygen
/**
 * @brief osif NVRAM initializer
 */
void zb_osif_nvram_init(zb_char_t *name);

/**
 * @brief Get NVRAM page length
 *
 * @return NVRAM page length
 */
zb_uint32_t zb_get_nvram_page_length();

/**
 * @brief Get NVRAM page count
 *
 * @return NVRAM page count
 */
zb_uint8_t zb_get_nvram_page_count();

/**
 * @brief Read from NVRAM directly, by address
 * Read some bytes from NVRAM use address
 *
 * @param address - NVRAM address
 * @param len - count bytes from read data
 * @param buf - (in) buffer for contains read data
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_read_memory(zb_uint32_t address, zb_uint16_t len, zb_uint8_t *buf );

/**
 * @brief Read from NVRAM page
 * Read some bytes from NVRAM
 *
 * @param page - NVRAM page
 * @param pos - Start position
 * @param buf - (in) buffer for contains read data
 * @param len - count bytes from read data
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_read(zb_uint8_t page, zb_uint32_t pos, zb_uint8_t *buf, zb_uint16_t len );

/**
 * @brief Read from NVRAM page with test
 * Read some bytes from NVRAM with test contents.
 * If all byte equal 0xFF then return RET_ERROR
 * Exists not for all platforms.
 *
 * @param page - NVRAM page
 * @param pos - Start position
 * @param buf - (in) buffer for contains read data
 * @param len - count bytes from read data
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_read_test(zb_uint8_t page, zb_uint32_t pos, zb_uint8_t *buf, zb_uint16_t len );

/**
 * @brief Write from NVRAM page
 * Write some bytes to NVRAM
 *
 * @param page - NVRAM page
 * @param pos - Start position
 * @param buf - buffer contains data for write
 * @param len - count bytes for write data
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_write(zb_uint8_t page, zb_uint32_t pos, void *buf, zb_uint16_t len );

/**
 * @brief Erase NVRAM page
 * Fill NVRAM page by 0xFF.
 *
 * @param page - page index
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_osif_nvram_erase_async(zb_uint8_t page);

void zb_osif_nvram_wait_for_last_op();

/**
 * @brief Flush NVRAM page
 * Flish NVRAM page to file or NVRAM.
 * Different to hardware device
 *
 */
void zb_osif_nvram_flush();

#endif

#ifdef ZB_USE_OSIF_OTA_ROUTINES
/**
 *  @addtogroup ll
 *  @{
 */

/**
 *  @defgroup ota Low level API for OTA
 */
/*! @{ */

#define OTA_UPGRADE_HEADING_DATA \
  (sizeof(zb_zcl_ota_upgrade_file_header_t) + 2 /* fw tag Id */ + 4 /* fw length */)
#define OTA_UPGRADE_HASH_SIZE 16
#define OTA_UPGRADE_TRAILING_DATA \
  (2 /* hash tag Id */ + 4 /* hash length */ + OTA_UPGRADE_HASH_SIZE /* Hash */)

/**
   Open device to be used for OTA image write or read.

   Take into account which flash (A or B) current FW is running at.
   @see zb_osif_ota_close_storage

   @return handle
 */
void *zb_osif_ota_open_storage();

/**
   Close device used for OTA image write

   @param dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
*/
void zb_osif_ota_close_storage(void *dev);

/**
   Erase flash to be used for OTA image store.

   Note: offset and size must be aligned to minimal eraseable page boundary -
   @see zb_osif_ota_get_erase_portion().

   @param dev flash device handle @see zb_osif_ota_open_storage
   @param image_size OTA file image size
 */
void zb_osif_ota_erase_fw(void *dev, zb_uint_t offset, zb_uint_t size);

/**
   Get portion of OTA storage which can be erased at once.

   @return erase block size in bytes
 */
zb_uint32_t zb_osif_ota_get_erase_portion();

/**
   Write OTA image data to flash.

   Deal with alignment.

   @param dev flash device handle @see zb_osif_ota_open_storage
   @param data pointer to the data to write
   @param off offset in OTA file
   @param block_size amount of data to write
   @param image_size total image size
 */
void zb_osif_ota_write(void *dev, zb_uint8_t *data, zb_uint32_t off, zb_uint32_t block_size, zb_uint32_t image_size);

/**
   Mark FW as downloaded and ready to use by bootloader.

   To be called when finishing OTA, after FW loaded and verified.

   @param ota_dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
   @param size OTA image size, without CRC
   @param revision OTA image revision
 */
void zb_osif_ota_mark_fw_ready(void *ota_dev, zb_uint32_t size, zb_uint32_t revision);

/**
   Mark new FW as absent so bootloader does not attempt to re-flash it.

   To be called when starting OTA.
 */
void zb_osif_ota_mark_fw_absent();

/**
   Mark FW upgrade finished, so bootloader just loads current FW.

   To be called from newly started application FW.
   Usually not need to be called by the application. ZBOSS kernel cares about it.
 */
void zb_osif_ota_mark_fw_updated();

/**
   Verify OTA file image written to device dev.

   @param dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
   @param raw_len - OTA image file length
   @return ZB_TRUE if ok, ZB_FALSE if error
 */
zb_bool_t zb_osif_ota_verify_integrity(void *dev, zb_uint32_t raw_len);

zb_bool_t zb_osif_ota_verify_integrity_async(void *dev, zb_uint32_t raw_len);
zb_void_t zb_osif_ota_verify_integrity_done(zb_uint8_t integrity_is_ok);

void zb_osif_ota_read(void *dev, zb_uint8_t *data, zb_uint32_t addr, zb_uint32_t size);

/**
   Check that image size is not too big

   @param image_size OTA file image size
   @return ZB_TRUE if ok, ZB_FALSE if error
 */
zb_bool_t zb_osif_ota_fw_size_ok(zb_uint32_t image_size);

/**
   Get OTA image data at OTA server

   @param dev flash device handle used to write OTA image @see zb_osif_ota_open_storage
   @param offset offset in the OTA image
   @param len - length of image block to get
   @return pointer to image block
 */
zb_uint8_t *zb_osif_ota_srv_get_image(void *dev, zb_uint32_t offset, zb_uint32_t len);
zb_uint8_t *zb_osif_ota_srv_get_image_header(void *dev);

/*! @} */
/*! @} */
void osif_set_reset_at_crash();

#endif /* ZB_USE_OSIF_OTA_ROUTINES */

#if defined ZB_PROFILE_STACK_USAGE
/* Both functions are platform-specific */

/**
 * @brief Prepare stack profiler.
 * Fill stack area with a predefined pattern.
 *
 */
zb_void_t zb_stack_profiler_pre();

/**
 * @brief Get stack usage by previously called function.
 *
 * @return Count of bytes in stack area used
 */
zb_uint16_t zb_stack_profiler_usage();

#endif  /* ZB_PROFILE_STACK_USAGE */

#ifdef ZB_USE_SLEEP
zb_uint32_t zb_osif_sleep(zb_uint32_t sleep_tmo);
zb_void_t zb_osif_wake_up();
#endif

#ifdef ZB_PRODUCTION_CONFIG

/* Check whether production configuration block is present in memory */
zb_bool_t zb_osif_production_configuration_check_header();

/* Read data from production configuration block
 *
 * @param buffer - buffer to read into
 * @param len - length of block to read
 * @param offset - offset to read from
 *
 * @return true if config header has valid data
 */
zb_void_t zb_osif_production_configuration_read(zb_uint8_t *buffer, zb_uint16_t len, zb_uint16_t offset);

/* Set transmit power of radio on selected channel
 *
 * @param channel - channle on which radio applies new transmit power
 * @param power - transmit power in dBm
 *
 * return RET_OK if power was set successfully, RET_ERROR otherwise
 */
zb_ret_t zb_osif_set_transmit_power(zb_uint8_t channel, zb_int8_t power);

#endif

#endif /* ZB_OSIF_H */
