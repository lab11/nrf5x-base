/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x0 + 140K, LENGTH = 512K - 140K /* 136 kB is taken by S132, 148 kB for app. */
  RAM (rwx) :  ORIGIN = 0x20000000 + 12K, LENGTH = 64K - 12K /* softdevice wants max ~10K bytes. */
  /* NOTE: RAM must be updated based on parameters to softdevice_enable() */
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
