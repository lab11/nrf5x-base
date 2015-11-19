/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x0 + 96K, LENGTH = 256K - 96K /* 96 kB is taken by S110, rest for app. */
  RAM (rwx) : ORIGIN = 0x20000000 + 8K, LENGTH = 32K - 8K /* 8 kB is taken by S110, 24 kB for app. */
}
INCLUDE "gcc_nrf51_common.ld"
