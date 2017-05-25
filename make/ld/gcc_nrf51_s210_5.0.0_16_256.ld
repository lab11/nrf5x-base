MEMORY
{
  FLASH (rx) : ORIGIN = 0xd000, LENGTH = 0x33000     /* 52k for softdevice, 76k for app */
  RAM (rwx) :  ORIGIN = 0x20000900, LENGTH = 0x3700  /* 2.25k for softdevice, 13.75k for app */
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
