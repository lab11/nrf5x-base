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
PURPOSE: OTA functions for nRF52840 SoC.
*/

#define ZB_TRACE_FILE_ID 51463

#include "zboss_api.h"
#include "zb_nrf52840_internal.h"
#include "zb_nrf52840_zboss_deps.h"

#ifdef ZB_USE_OSIF_OTA_ROUTINES
static zb_osif_flash_device_t flash_device;

void *zb_osif_ota_open_storage()
{
  /* TODO: implement */
  void *dev = &flash_device;
  return dev;
}

zb_bool_t zb_osif_ota_fw_size_ok(zb_uint32_t image_size)
{
  /* TODO: implement */
  ZVUNUSED(image_size);
  return ZB_TRUE;
}

zb_uint32_t zb_osif_ota_get_erase_portion()
{
  zb_uint32_t p_size = 0;
  /* TODO: implement - return erase portion size */
  return p_size;
}

void zb_osif_ota_erase_fw(void *dev, zb_uint_t offset, zb_uint32_t size)
{
  TRACE_MSG(TRACE_COMMON2, "zb_osif_ota_erase_fw dev %p offset %d size %ld", (FMT__P_L_D, dev, offset, size));
  /* TODO: implement */
}

void zb_osif_ota_write(void *dev, zb_uint8_t *data, zb_uint_t off, zb_uint_t size, zb_uint32_t image_size)
{
  ZVUNUSED(image_size);
  ZVUNUSED(data);
  TRACE_MSG(TRACE_COMMON2, "zb_osif_ota_write dev %p offset %d size %ld", (FMT__P_L_D, dev, off, size));
  /* TODO: implement */
}

void zb_osif_ota_mark_fw_ready(void *dev, zb_uint32_t size, zb_uint32_t revision)
{
  TRACE_MSG(TRACE_COMMON1, "zb_osif_ota_mark_fw_ready dev %p size %ld revision %ld",
            (FMT__P_L_L, dev, size, revision));
  /* TODO: implement */
}

void zb_osif_ota_mark_fw_absent()
{
  TRACE_MSG(TRACE_COMMON2, "zb_osif_ota_mark_fw_absent", (FMT__0));
  /* TODO: implement */
}


void zb_osif_ota_mark_fw_updated()
{
  TRACE_MSG(TRACE_COMMON2, "zb_osif_ota_mark_fw_updated", (FMT__0));
  /* TODO: implement */
}

void zb_osif_ota_close_storage(void *dev)
{
  TRACE_MSG(TRACE_COMMON2, "zb_osif_ota_close_storage %p", (FMT__P, dev));
  /* TODO: implement */
}

/*
 * Weak-implementation need for Keil-build, because without this when you
 * try to build an application Keil-compiler will return the follow error:
 * L6218E: Undefined symbol zb_osif_ota_verify_integrity_done.
 *
 * Reimplement this function without __weak in your application if need.
 */
ZB_WEAK_PRE zb_void_t ZB_WEAK zb_osif_ota_verify_integrity_done(zb_uint8_t integrity_is_ok)
{
	ZVUNUSED(integrity_is_ok);
}

zb_void_t zb_sec_b6_hash_iter_done(void *dev, zb_uint32_t input_len, zb_uint8_t *calc_hash)
{
  zb_ret_t zb_err_code;

  ZVUNUSED(dev);
  ZVUNUSED(input_len);
  ZVUNUSED(calc_hash);
  TRACE_MSG(TRACE_COMMON2, "zb_sec_b6_hash_iter_done", (FMT__0));
  /* TODO: implement: need to compare calculated hash and hash from the image */
  zb_err_code = ZB_SCHEDULE_CALLBACK(zb_osif_ota_verify_integrity_done, ZB_TRUE);
  ZVUNUSED(zb_err_code);
}

zb_bool_t zb_osif_ota_verify_integrity_async(void *dev, zb_uint32_t raw_len)
{
  ZVUNUSED(dev);
  TRACE_MSG(TRACE_COMMON2, "zb_osif_ota_verify_integrity_async size %ld",
            (FMT__L, raw_len - OTA_UPGRADE_TRAILING_DATA));
  /* TODO: implement: need to call zb_sec_b6_hash_iter_start() */
  return ZB_TRUE;
}

void zb_osif_ota_read(void *dev, zb_uint8_t *data, zb_uint32_t addr, zb_uint32_t size)
{
  TRACE_MSG(TRACE_COMMON2, "zb_osif_ota_read dev %p data %p addr %ld size %ld",
            (FMT__P_P_L_L, dev, data, addr, size));
  /* TODO: implement */
}

zb_uint8_t *zb_osif_ota_srv_get_image_header(void *dev)
{
  /* TODO: implement: read ota_file_header and put it to flash device (or return pointer to memory
   * if have direct access) */
  return ((zb_osif_flash_device_t*)dev)->ota_file_header;
}

zb_uint8_t *zb_osif_ota_srv_get_image(void *dev, zb_uint32_t offset, zb_uint32_t len)
{
  ZVUNUSED(offset);
  ZVUNUSED(len);
  /* TODO: implement: read ota portion and put it to flash device (or return pointer to memory
   * if have direct access) */
  return ((zb_osif_flash_device_t*)dev)->transmit_buffer;
}
#endif
