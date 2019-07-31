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
 * PURPOSE: NVRAM functions for nRF52840 SoC.
*/

#define ZB_TRACE_FILE_ID 51401
#include "zboss_api.h"
#include "zb_nrf52840_internal.h"
#include "zb_nrf52840_zboss_deps.h"
#include "nrf_fstorage.h"

#if defined SOFTDEVICE_PRESENT
#include "nrf_fstorage_sd.h"
#else
#include "nrf_fstorage_nvmc.h"
#endif

#ifdef ZB_NVRAM_FLASH_AUTO_ADDRESS
#include "nrf.h"
#include "sdk_config.h" // Provides FDS_VIRTUAL_PAGE_SIZE and FDS_VIRTUAL_PAGES_RESERVED used to verify FDS settings.
#endif

/*nRF52840 general flash parameters*/
#define ZB_NVRAM_FLASH_START_ADDRESS      0x0000
#define ZB_NVRAM_FLASH_PAGES_COUNT        0x100
#define ZB_NVRAM_FLASH_PAGE_SIZE          0x1000
#define ZB_NVRAM_FLASH_FULL_SIZE          (ZB_NVRAM_FLASH_PAGES_COUNT * ZB_NVRAM_FLASH_PAGE_SIZE)

/*Software is stored from null address in flash! Need to offset for ZB NVRAM usage.
  Reserved max 256Kb high flash memory for ZB NVRAM*/

/* ZB NVRAM parameters. Page size must be a multiple of ZB_NVRAM_FLASH_PAGE_SIZE*/
#define ZB_NVRAM_PAGE_SIZE                0x4000
#define ZB_NVRAM_PAGE_COUNT               2
#define ZB_NVRAM_CONFIG_PAGE_COUNT        1
#define ZB_NVRAM_FULL_SIZE                (ZB_NVRAM_PAGE_SIZE * (ZB_NVRAM_PAGE_COUNT + ZB_NVRAM_CONFIG_PAGE_COUNT))

#ifndef ZB_NVRAM_FLASH_AUTO_ADDRESS
#define ZB_NVRAM_FLASH_START_OFFSET       0xC0000

#if (ZB_NVRAM_FLASH_START_OFFSET + ZB_NVRAM_FULL_SIZE) > ZB_NVRAM_FLASH_FULL_SIZE
#error Invalid NVRAM memory configuration!
#endif

/* ZB NVRAM start address*/
#define ZB_NVRAM_PAGE_BASE_ADDRESS(num)   (ZB_NVRAM_FLASH_START_OFFSET + (ZB_NVRAM_PAGE_SIZE * (num)))
#endif

#ifdef ZB_NVRAM_FLASH_AUTO_ADDRESS
#define ZB_NVRAM_FLASH_START_OFFSET       0x0000 // Invalid address - will be updated in zb_osif_nvram_init() function.

#if FDS_ENABLED
#if ZB_NVRAM_FULL_SIZE > (FDS_VIRTUAL_PAGES_RESERVED * FDS_VIRTUAL_PAGE_SIZE)
#error Invalid NVRAM memory configuration!
#endif
#endif

static uint32_t flash_end_addr(void)
{
    uint32_t const bootloader_addr = NRF_UICR->NRFFW[0];
    uint32_t const page_sz         = NRF_FICR->CODEPAGESIZE;
#ifndef NRF52810_XXAA
    uint32_t const code_sz         = NRF_FICR->CODESIZE;
#else
    // Number of flash pages, necessary to emulate the NRF52810 on NRF52832.
    uint32_t const code_sz         = 48;
#endif

    return (bootloader_addr != 0xFFFFFFFF) ? bootloader_addr : (code_sz * page_sz);
}

static uint32_t flash_start_addr(void)
{
    return flash_end_addr() - ZB_NVRAM_FULL_SIZE;
}

/* ZB NVRAM start address*/
#define ZB_NVRAM_PAGE_BASE_ADDRESS(num)   (flash_start_addr() + (ZB_NVRAM_PAGE_SIZE * (num)))
#endif

void fstorage_callback(nrf_fstorage_evt_t * p_evt)
{
  ZB_ASSERT(p_evt->result == NRF_SUCCESS);
  {
    switch (p_evt->id)
    {
      case NRF_FSTORAGE_EVT_WRITE_RESULT:
      case NRF_FSTORAGE_EVT_READ_RESULT:
      case NRF_FSTORAGE_EVT_ERASE_RESULT:
        zb_osif_clean_flash_op_progress();
        break;
      default:
        ZB_ASSERT(0);
        break;
    }
  }
}

NRF_FSTORAGE_DEF(nrf_fstorage_t nvram_fstorage) =
{
    .evt_handler    = fstorage_callback,
    .start_addr     = ZB_NVRAM_FLASH_START_OFFSET,
    .end_addr       = ZB_NVRAM_FLASH_START_OFFSET + ZB_NVRAM_FULL_SIZE,
};

#if defined ZB_USE_NVRAM || defined DOXYGEN

void zb_osif_nvram_init(zb_char_t* name)
{
  ZVUNUSED(name);

#ifdef ZB_NVRAM_FLASH_AUTO_ADDRESS
  nvram_fstorage.end_addr   = flash_end_addr();
  nvram_fstorage.start_addr = flash_start_addr();
#endif

  nrf_fstorage_init(
    &nvram_fstorage,    /* fstorage instance. */
#if defined SOFTDEVICE_PRESENT
    &nrf_fstorage_sd,   /* Name of the backend. */
#else
    &nrf_fstorage_nvmc, /* Name of the backend. */
#endif
    NULL                /* Optional parameter, backend-dependant. */
    );
}

zb_uint32_t zb_get_nvram_page_length()
{
  return ZB_NVRAM_PAGE_SIZE;
}

zb_uint8_t zb_get_nvram_page_count()
{
  return ZB_NVRAM_PAGE_COUNT;
}

zb_ret_t zb_osif_addr_read(zb_uint32_t address, zb_uint16_t len, zb_uint8_t* buf)
{
  if (!len)
  {
    return RET_INVALID_PARAMETER_2;
  }

  if (!buf)
  {
    return RET_INVALID_PARAMETER_3;
  }

  TRACE_MSG(TRACE_COMMON1, "zb_osif_addr_read address %ld len %d",
            (FMT__L_D, address, len));

  /*copy data from flash memory*/
  ZB_MEMCPY(buf, ((void *)address), len);

  return RET_OK;
}

zb_ret_t zb_osif_nvram_read(zb_uint8_t page, zb_uint32_t pos, zb_uint8_t* buf, zb_uint16_t len)
{
  zb_uint32_t flash_addr = 0;

  if (page >= ZB_NVRAM_PAGE_COUNT)
  {
    return RET_PAGE_NOT_FOUND;
  }

  if (pos + len >= ZB_NVRAM_PAGE_SIZE)
  {
    return RET_INVALID_PARAMETER;
  }

  if (!buf)
  {
    return RET_INVALID_PARAMETER_3;
  }

  if (!len)
  {
    return RET_INVALID_PARAMETER_4;
  }

  TRACE_MSG(TRACE_COMMON1, "zb_osif_nvram_read %hd pos %d len %d",
            (FMT__H_D_D, page, pos, len));

  flash_addr = ZB_NVRAM_PAGE_BASE_ADDRESS(page) + pos;

  /*copy data from flash memory*/
  ZB_MEMCPY(buf, ((void *)flash_addr), len);

  return RET_OK;
}

/* 01/22/2018 EE CR:MINOR Kill that function here and everywhere: not used. */
zb_ret_t zb_osif_nvram_read_test(zb_uint8_t page, zb_uint32_t pos, zb_uint8_t* buf, zb_uint16_t len)
{
  zb_ret_t    ret = RET_ERROR;
  zb_uint32_t i;

  if (page >= ZB_NVRAM_PAGE_COUNT)
  {
    return RET_PAGE_NOT_FOUND;
  }

  if (pos + len >= ZB_NVRAM_PAGE_SIZE)
  {
    return RET_INVALID_PARAMETER;
  }

  if (!buf)
  {
    return RET_INVALID_PARAMETER_3;
  }

  if (!len)
  {
    return RET_INVALID_PARAMETER_4;
  }

  /*read data from flash*/
  ret = zb_osif_nvram_read(page, pos, buf, len);
  if (ret != RET_OK)
  {
    return ret;
  }

  /*check content*/
  for (i = 0; i < len; ++i)
  {
    if (buf[i] != 0xFF)
    {
      return RET_OK;
    }
  }

  /*if all bytes are 0xFF return Error*/
  return RET_ERROR;
}

zb_ret_t zb_osif_addr_write(zb_uint32_t address, void* buf, zb_uint16_t len)
{
  zb_uint32_t flash_addr = 0;
  zb_int_t ret = 0;

  ZVUNUSED(ret);

  if (address + len > ZB_NVRAM_FULL_SIZE)
  {
    return RET_INVALID_PARAMETER;
  }

  if (!buf)
  {
    return RET_INVALID_PARAMETER_2;
  }

  if (!len)
  {
    return RET_INVALID_PARAMETER_3;
  }

  TRACE_MSG(TRACE_COMMON1, "zb_osif_addr_write address %ld len %d",
            (FMT__L_D, address, len));

  flash_addr = ZB_NVRAM_PAGE_BASE_ADDRESS(0) + address;

  /* 04/02/2018 EE CR:MINOR Make it a function? */
  while (zb_osif_flash_op_progress())
  {
  }

  /*copy data to flash*/
  zb_osif_set_flash_op_progress();
  ret = nrf_fstorage_write(
              &nvram_fstorage,   /* The instance to use. */
              flash_addr,        /* The address in flash where to store the data. */
              (uint32_t *)buf,   /* A pointer to the data. */
              len,               /* Lenght of the data, in bytes. */
              NULL               /* Optional parameter, backend-dependent. */
    );

  ZB_ASSERT(ret == NRF_SUCCESS);
  /* Wait SOC event handler with NRF_EVT_FLASH_OPERATION_SUCCESS or NRF_FSTORAGE_WRITE_RESULT */
  while (zb_osif_flash_op_progress())
  {
  }

  return RET_OK;
}

zb_ret_t zb_osif_nvram_write(zb_uint8_t page, zb_uint32_t pos, void* buf, zb_uint16_t len)
{
  zb_uint32_t flash_addr = 0;
  zb_int_t ret = 0;
  ZVUNUSED(ret);

  if (page >= ZB_NVRAM_PAGE_COUNT)
  {
    return RET_PAGE_NOT_FOUND;
  }

  if (pos + len >= ZB_NVRAM_PAGE_SIZE)
  {
    return RET_INVALID_PARAMETER;
  }

  if (!buf)
  {
    return RET_INVALID_PARAMETER_3;
  }

  if (!(len>>2))
  {
    return RET_INVALID_PARAMETER_4;
  }

  TRACE_MSG(TRACE_COMMON1, "zb_osif_nvram_write %hd pos %d len %d",
            (FMT__H_D_D, page, pos, len));

  flash_addr = ZB_NVRAM_PAGE_BASE_ADDRESS(page) + pos;

  ZB_ASSERT((len >= 4) && (len % 4 == 0));

  while (zb_osif_flash_op_progress())
  {
  }

  /*copy data to flash*/
  zb_osif_set_flash_op_progress();
  ret = nrf_fstorage_write(
              &nvram_fstorage,   /* The instance to use. */
              flash_addr,        /* The address in flash where to store the data. */
              (uint32_t *)buf,   /* A pointer to the data. */
              len,               /* Lenght of the data, in bytes. */
              NULL               /* Optional parameter, backend-dependent. */
    );
  ZB_ASSERT(ret == NRF_SUCCESS);

  /* Wait SOC event handler with NRF_EVT_FLASH_OPERATION_SUCCESS or NRF_FSTORAGE_WRITE_RESULT */
  while (zb_osif_flash_op_progress())
  {
  }

  return RET_OK;
}

zb_ret_t zb_osif_nvram_erase_async(zb_uint8_t page)
{
  zb_uint32_t flash_addr = 0, i;
  zb_int_t ret = 0;

  ZVUNUSED(ret);

  if (page >= ZB_NVRAM_PAGE_COUNT)
  {
    return RET_PAGE_NOT_FOUND;
  }
  TRACE_MSG(TRACE_COMMON3, "zb_osif_nvram_erase_async page %hd", (FMT__H, page));

  flash_addr = ZB_NVRAM_PAGE_BASE_ADDRESS(page);

  for (i = 0; i < ZB_NVRAM_PAGE_SIZE / ZB_NVRAM_FLASH_PAGE_SIZE; ++i)
  {
    while (zb_osif_flash_op_progress())
    {
    }

    zb_osif_set_flash_op_progress();
    ret = nrf_fstorage_erase(
                &nvram_fstorage,   /* The instance to use. */
                flash_addr,        /* The address of the flash pages to erase. */
                1,                 /* The number of pages to erase. */
                NULL               /* Optional parameter, backend-dependent. */
      );
    ZB_ASSERT(ret == NRF_SUCCESS);
    /* This call is designed to be asynchronous, but currently it is synchronous. */

    /* Wait SOC event handler with NRF_EVT_FLASH_OPERATION_SUCCESS or NRF_FSTORAGE_ERASE_RESULT */
    while (zb_osif_flash_op_progress())
    {
    }

    flash_addr += ZB_NVRAM_FLASH_PAGE_SIZE;
  }
  zb_osif_flash_erase_finished(page);
  return RET_OK;
}

void zb_osif_nvram_wait_for_last_op()
{
  /* empty for syncronous erase and write */
}

void zb_osif_nvram_flush()
{
  /* empty */
}

#ifdef ZB_PRODUCTION_CONFIG

/* Production config is in the next Flash page after ZB NVRAM */
#define ZB_OSIF_PRODUCTION_CONFIG_BLOCK_ADDRESS ZB_NVRAM_PAGE_BASE_ADDRESS(ZB_NVRAM_PAGE_COUNT)

#define ZB_OSIF_PRODUCTION_CONFIG_HEADER             {0xE7, 0x37, 0xDD, 0xF6}
#define ZB_OSIF_PRODUCTION_CONFIG_HEADER_SIZE        4

zb_bool_t zb_osif_production_configuration_check_header()
{
  zb_uint8_t hdr[] = ZB_OSIF_PRODUCTION_CONFIG_HEADER;
  zb_uint8_t buffer[ZB_OSIF_PRODUCTION_CONFIG_HEADER_SIZE];
  zb_osif_addr_read(ZB_OSIF_PRODUCTION_CONFIG_BLOCK_ADDRESS, ZB_OSIF_PRODUCTION_CONFIG_HEADER_SIZE, buffer);

  return ((zb_bool_t) !ZB_MEMCMP(buffer, hdr, sizeof(buffer)));
}

zb_void_t zb_osif_production_configuration_read(zb_uint8_t *buffer, zb_uint16_t len, zb_uint16_t offset)
{
  zb_osif_addr_read(ZB_OSIF_PRODUCTION_CONFIG_BLOCK_ADDRESS + ZB_OSIF_PRODUCTION_CONFIG_HEADER_SIZE + offset, len, buffer);
}

#endif /* ZB_PRODUCTION_CONFIG */

#endif  /* defined ZB_USE_NVRAM || defined DOXYGEN */
