/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x0 + 108K, LENGTH = 256K - 108K /* 108 kB is taken by S130, 148 kB for app. */
  RAM (rwx) :  ORIGIN = 0x20000000 + 10K, LENGTH = 16K-10K /* softdevice wants min 4.9K */
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
