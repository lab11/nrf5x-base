/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx): ORIGIN = 256K - 16K - 1K - 1K, LENGTH = 16K /* 1K is taken by flash storage of BLE Address, 16K for Bootloader */
  BOOTLOADER_SETTINGS (rw) : ORIGIN = 256K - 1K - 1K, LENGTH = 1K /* Right below Bootloader */
  RAM (rwx) : ORIGIN = 0x20000000 + 10K, LENGTH = 32K - 10K /* 10 kB is taken by S110, 24 kB for app. */
  UICR_BOOTLOADER (r)  : ORIGIN = 0x10001014, LENGTH =  0x04
}


SECTIONS
{
    .bootloaderSettings (NOLOAD):
    {
    
    } > BOOTLOADER_SETTINGS

    .uicrBootStartAddress :
    {
        KEEP(*(.uicrBootStartAddress))
    } > UICR_BOOTLOADER
}

INCLUDE "gcc_nrf51_common.ld"
