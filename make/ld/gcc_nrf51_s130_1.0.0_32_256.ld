/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x0 + 108K + 4K, LENGTH = 256K - 112K /* 112 kB is taken by S130, 144 kB for app. */
  RAM (rwx) : ORIGIN = 0x20000000 + 10K, LENGTH = 32K - 10K /* 10 kB is taken by S130, 22 kB for app. */
}
INCLUDE "gcc_nrf51_common.ld"
