@ECHO OFF

set FROM[0]=lib\mbedcrypto\cc310\arm5_no_packs\_build\libmbedcrypto.lib
set FROM[1]=lib\mbedcrypto\software\arm5_no_packs\_build\libmbedcrypto-software.lib
set FROM[2]=lib\openthread_cli\ftd\arm5_no_packs\_build\libopenthread-cli-ftd.lib
set FROM[3]=lib\openthread_cli\mtd\arm5_no_packs\_build\libopenthread-cli-mtd.lib
set FROM[4]=lib\openthread_diag\arm5_no_packs\_build\libopenthread-diag.lib
set FROM[5]=lib\openthread\ftd\arm5_no_packs\_build\libopenthread-ftd.lib
set FROM[6]=lib\openthread\mtd\arm5_no_packs\_build\libopenthread-mtd.lib
set FROM[7]=lib\openthread_ncp\ftd\uart\arm5_no_packs\_build\libopenthread-ncp-ftd.lib
set FROM[8]=lib\openthread_ncp\ftd\spi\arm5_no_packs\_build\libopenthread-ncp-ftd-spi.lib
set FROM[9]=lib\openthread_ncp\mtd\uart\arm5_no_packs\_build\libopenthread-ncp-mtd.lib
set FROM[10]=lib\openthread_ncp\mtd\spi\arm5_no_packs\_build\libopenthread-ncp-mtd-spi.lib
set FROM[11]=lib\openthread_nrf52840\sdk\uart\arm5_no_packs\_build\libopenthread-nrf52840-sdk.lib
set FROM[12]=lib\openthread_nrf52840\sdk\usb\arm5_no_packs\_build\libopenthread-nrf52840-sdk-usb.lib
set FROM[13]=lib\openthread_nrf52840\softdevice\uart\arm5_no_packs\_build\libopenthread-nrf52840-softdevice-sdk.lib
set FROM[14]=lib\openthread_nrf52840\softdevice\usb\arm5_no_packs\_build\libopenthread-nrf52840-softdevice-sdk-usb.lib
set FROM[15]=lib\openthread_platform_utils\arm5_no_packs\_build\libopenthread-platform-utils.lib

set TO[0]=..\lib\keil\libmbedcrypto.lib
set TO[1]=..\lib\keil\libmbedcrypto-software.lib
set TO[2]=..\lib\keil\libopenthread-cli-ftd.lib
set TO[3]=..\lib\keil\libopenthread-cli-mtd.lib
set TO[4]=..\lib\keil\libopenthread-diag.lib
set TO[5]=..\lib\keil\libopenthread-ftd.lib
set TO[6]=..\lib\keil\libopenthread-mtd.lib
set TO[7]=..\lib\keil\libopenthread-ncp-ftd.lib
set TO[8]=..\lib\keil\libopenthread-ncp-ftd-spi.lib
set TO[9]=..\lib\keil\libopenthread-ncp-mtd.lib
set TO[10]=..\lib\keil\libopenthread-ncp-mtd-spi.lib
set TO[11]=..\lib\keil\libopenthread-nrf52840-sdk.lib
set TO[12]=..\lib\keil\libopenthread-nrf52840-sdk-usb.lib
set TO[13]=..\lib\keil\libopenthread-nrf52840-softdevice-sdk.lib
set TO[14]=..\lib\keil\libopenthread-nrf52840-softdevice-sdk-usb.lib
set TO[15]=..\lib\keil\libopenthread-platform-utils.lib

FOR /L %%l IN (0,1,15) DO (
    CALL ECHO Copying %%FROM[%%l]%%...
    CALL COPY /B %%FROM[%%l]%% /B %%TO[%%l]%% || exit 1
)
