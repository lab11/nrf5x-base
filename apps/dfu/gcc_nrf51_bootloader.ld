/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx): ORIGIN = 0x3bc00, LENGTH = 0x4000 
  RAM (rwx): ORIGIN = 0x20002000, LENGTH = 0x6000 
  BOOTLOADER_SETTINGS (rw) : ORIGIN = 0x3fc00, LENGTH = 0x0400
  UICR_BOOTLOADER (r)  : ORIGIN = 0x10001014, LENGTH =  0x04
}


SECTIONS
{
    .bootloaderSettings :
    {
    
    } > BOOTLOADER_SETTINGS

    .uicrBootStartAddress :
    {
        KEEP(*(.uicrBootStartAddress))
    } > UICR_BOOTLOADER
   
}

INCLUDE "gcc_nrf51_common.ld"
