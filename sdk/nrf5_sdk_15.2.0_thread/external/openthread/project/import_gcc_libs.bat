@ECHO OFF

set FROM[0]=lib\mbedcrypto\cc310\armgcc\libmbedcrypto.a
set FROM[1]=lib\mbedcrypto\software\armgcc\libmbedcrypto-software.a
set FROM[2]=lib\openthread_cli\ftd\armgcc\libopenthread-cli-ftd.a
set FROM[3]=lib\openthread_cli\mtd\armgcc\libopenthread-cli-mtd.a
set FROM[4]=lib\openthread_diag\armgcc\libopenthread-diag.a
set FROM[5]=lib\openthread\ftd\armgcc\libopenthread-ftd.a
set FROM[6]=lib\openthread\mtd\armgcc\libopenthread-mtd.a
set FROM[7]=lib\openthread_ncp\ftd\uart\armgcc\libopenthread-ncp-ftd.a
set FROM[8]=lib\openthread_ncp\ftd\spi\armgcc\libopenthread-ncp-ftd-spi.a
set FROM[9]=lib\openthread_ncp\mtd\uart\armgcc\libopenthread-ncp-mtd.a
set FROM[10]=lib\openthread_ncp\mtd\spi\armgcc\libopenthread-ncp-mtd-spi.a
set FROM[11]=lib\openthread_nrf52840\sdk\uart\armgcc\libopenthread-nrf52840-sdk.a
set FROM[12]=lib\openthread_nrf52840\sdk\usb\armgcc\libopenthread-nrf52840-sdk-usb.a
set FROM[13]=lib\openthread_nrf52840\softdevice\uart\armgcc\libopenthread-nrf52840-softdevice-sdk.a
set FROM[14]=lib\openthread_nrf52840\softdevice\usb\armgcc\libopenthread-nrf52840-softdevice-sdk-usb.a
set FROM[15]=lib\openthread_platform_utils\armgcc\libopenthread-platform-utils.a

set TO[0]=..\lib\gcc\libmbedcrypto.a
set TO[1]=..\lib\gcc\libmbedcrypto-software.a
set TO[2]=..\lib\gcc\libopenthread-cli-ftd.a
set TO[3]=..\lib\gcc\libopenthread-cli-mtd.a
set TO[4]=..\lib\gcc\libopenthread-diag.a
set TO[5]=..\lib\gcc\libopenthread-ftd.a
set TO[6]=..\lib\gcc\libopenthread-mtd.a
set TO[7]=..\lib\gcc\libopenthread-ncp-ftd.a
set TO[8]=..\lib\gcc\libopenthread-ncp-ftd-spi.a
set TO[9]=..\lib\gcc\libopenthread-ncp-mtd.a
set TO[10]=..\lib\gcc\libopenthread-ncp-mtd-spi.a
set TO[11]=..\lib\gcc\libopenthread-nrf52840-sdk.a
set TO[12]=..\lib\gcc\libopenthread-nrf52840-sdk-usb.a
set TO[13]=..\lib\gcc\libopenthread-nrf52840-softdevice-sdk.a
set TO[14]=..\lib\gcc\libopenthread-nrf52840-softdevice-sdk-usb.a
set TO[15]=..\lib\gcc\libopenthread-platform-utils.a

FOR /L %%l IN (0,1,15) DO (
    CALL ECHO Copying %%FROM[%%l]%%...
    CALL COPY /B %%FROM[%%l]%% /B %%TO[%%l]%% || exit 1
)
