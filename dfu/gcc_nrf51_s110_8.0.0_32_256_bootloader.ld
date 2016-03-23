/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx): ORIGIN = 0x3b800, LENGTH = 0x4000 
  RAM (rwx): ORIGIN = 0x20002000, LENGTH = 0x5f80
  NOINIT (rw): ORIGIN = 0x20007f80, LENGTH = 0x80
  BOOTLOADER_SETTINGS (rw) : ORIGIN = 0x3f800, LENGTH = 0x0400
  UICR_BOOTLOADER (r)  : ORIGIN = 0x10001014, LENGTH =  0x04
}


SECTIONS
{
    .noInit (NOLOAD):
    {
       
    } > NOINIT

    .bootloaderSettings (NOLOAD):
    {
    
    } > BOOTLOADER_SETTINGS

    .uicrBootStartAddress :
    {
        KEEP(*(.uicrBootStartAddress))
    } > UICR_BOOTLOADER
   
}

INCLUDE "gcc_nrf51_common.ld"
