/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x18000, LENGTH = 0x28000
  RAM (rwx) : ORIGIN = 0x20002000, LENGTH = 0x6000
}
INCLUDE "gcc_nrf51_common.ld"
