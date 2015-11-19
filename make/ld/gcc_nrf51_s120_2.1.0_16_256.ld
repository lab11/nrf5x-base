/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x0 + 116K, LENGTH = 256K - 116K /* 116 kB is taken by S120, 140 kB for app. */
  RAM (rwx) : ORIGIN = 0x20000000 + 10K, LENGTH = 16K - 10K /* 10 kB is taken by S120, 6 or 22 kB for app. */
}
INCLUDE "gcc_nrf51_common.ld"
