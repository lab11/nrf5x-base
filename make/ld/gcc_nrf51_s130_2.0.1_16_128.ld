/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x0 + 108K, LENGTH = 128K - 108K /* 108 kB is taken by S130, 20 kB for app. */
  RAM (rwx) :  ORIGIN = 0x20002100, LENGTH = 0x1F00 /* 0x2100 is taken by s130, 0x1F00 for app. */
  /* per app_ram_base.h, the softdevice requires 0x1fe8 worth of flash to enable a peripheral with a
   *    single connection. Since we have so little RAM to work with, we are going to go with that.
   *    Note that if used for a central, this limits to a single peripheral connection as well.
   * However, this has been determined to be a lie. In SDK12, the softdevice now demands 0x2078 for
   *    the same settings.
   */
}

SECTIONS
{
  .fs_data_out ALIGN(4):
  {
    PROVIDE( __start_fs_data = .);
    KEEP(*(fs_data))
    PROVIDE( __stop_fs_data = .);
  } = 0
}

INCLUDE "gcc_nrf51_common.ld"
