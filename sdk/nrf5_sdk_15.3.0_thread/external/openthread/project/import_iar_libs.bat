@ECHO OFF

set FROM[0]=lib\mbedcrypto\cc310\iar\_build\openthread_lib_libmbedcrypto.a
set FROM[1]=lib\mbedcrypto\software\iar\_build\openthread_lib_libmbedcrypto_software.a
set FROM[2]=lib\openthread_cli\ftd\iar\_build\openthread_lib_libopenthread_cli_ftd.a
set FROM[3]=lib\openthread_cli\mtd\iar\_build\openthread_lib_libopenthread_cli_mtd.a
set FROM[4]=lib\openthread_diag\iar\_build\openthread_lib_libopenthread_diag.a
set FROM[5]=lib\openthread\ftd\iar\_build\openthread_lib_libopenthread_ftd.a
set FROM[6]=lib\openthread\mtd\iar\_build\openthread_lib_libopenthread_mtd.a
set FROM[7]=lib\openthread_ncp\ftd\uart\iar\_build\openthread_lib_libopenthread_ncp_ftd.a
set FROM[8]=lib\openthread_ncp\ftd\spi\iar\_build\openthread_lib_libopenthread_ncp_ftd_spi.a
set FROM[9]=lib\openthread_ncp\mtd\uart\iar\_build\openthread_lib_libopenthread_ncp_mtd.a
set FROM[10]=lib\openthread_ncp\mtd\spi\iar\_build\openthread_lib_libopenthread_ncp_mtd_spi.a
set FROM[11]=lib\openthread_nrf52840\sdk\uart\iar\_build\openthread_lib_libopenthread_nrf52840_sdk.a
set FROM[12]=lib\openthread_nrf52840\sdk\usb\iar\_build\openthread_lib_libopenthread_nrf52840_sdk_usb.a
set FROM[13]=lib\openthread_nrf52840\softdevice\uart\iar\_build\openthread_lib_libopenthread_nrf52840_softdevice_sdk.a
set FROM[14]=lib\openthread_nrf52840\softdevice\usb\iar\_build\openthread_lib_libopenthread_nrf52840_softdevice_sdk_usb.a
set FROM[15]=lib\openthread_platform_utils\iar\_build\openthread_lib_libopenthread_platform_utils.a

set TO[0]=..\lib\iar\libmbedcrypto.a
set TO[1]=..\lib\iar\libmbedcrypto-software.a
set TO[2]=..\lib\iar\libopenthread-cli-ftd.a
set TO[3]=..\lib\iar\libopenthread-cli-mtd.a
set TO[4]=..\lib\iar\libopenthread-diag.a
set TO[5]=..\lib\iar\libopenthread-ftd.a
set TO[6]=..\lib\iar\libopenthread-mtd.a
set TO[7]=..\lib\iar\libopenthread-ncp-ftd.a
set TO[8]=..\lib\iar\libopenthread-ncp-ftd-spi.a
set TO[9]=..\lib\iar\libopenthread-ncp-mtd.a
set TO[10]=..\lib\iar\libopenthread-ncp-mtd-spi.a
set TO[11]=..\lib\iar\libopenthread-nrf52840-sdk.a
set TO[12]=..\lib\iar\libopenthread-nrf52840-sdk-usb.a
set TO[13]=..\lib\iar\libopenthread-nrf52840-softdevice-sdk.a
set TO[14]=..\lib\iar\libopenthread-nrf52840-softdevice-sdk-usb.a
set TO[15]=..\lib\iar\libopenthread-platform-utils.a

FOR /L %%l IN (0,1,15) DO (
    CALL ECHO Copying %%FROM[%%l]%%...
    CALL COPY /B %%FROM[%%l]%% /B %%TO[%%l]%% || exit 1
)
