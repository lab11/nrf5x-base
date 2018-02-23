/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x0, LENGTH = 192K
  RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 24K
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

INCLUDE "gcc_nrf52_common.ld"
