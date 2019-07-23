#!/bin/bash

declare -a source=(
    "lib/mbedcrypto/cc310/armgcc/libmbedcrypto.a"
    "lib/mbedcrypto/software/armgcc/libmbedcrypto-software.a"
    "lib/openthread_cli/ftd/armgcc/libopenthread-cli-ftd.a"
    "lib/openthread_cli/mtd/armgcc/libopenthread-cli-mtd.a"
    "lib/openthread_diag/armgcc/libopenthread-diag.a"
    "lib/openthread/ftd/armgcc/libopenthread-ftd.a"
    "lib/openthread/mtd/armgcc/libopenthread-mtd.a"
    "lib/openthread_ncp/ftd/uart/armgcc/libopenthread-ncp-ftd.a"
    "lib/openthread_ncp/ftd/spi/armgcc/libopenthread-ncp-ftd-spi.a"
    "lib/openthread_ncp/mtd/uart/armgcc/libopenthread-ncp-mtd.a"
    "lib/openthread_ncp/mtd/spi/armgcc/libopenthread-ncp-mtd-spi.a"
    "lib/openthread_nrf52840/sdk/uart/armgcc/libopenthread-nrf52840-sdk.a"
    "lib/openthread_nrf52840/sdk/usb/armgcc/libopenthread-nrf52840-sdk-usb.a"
    "lib/openthread_nrf52840/softdevice/uart/armgcc/libopenthread-nrf52840-softdevice-sdk.a"
    "lib/openthread_nrf52840/softdevice/usb/armgcc/libopenthread-nrf52840-softdevice-sdk-usb.a"
    "lib/openthread_platform_utils/armgcc/libopenthread-platform-utils.a"
    )

declare destination=../lib/gcc/

for f in "${source[@]}"
do
    echo =====================================================================
    echo Copying "$f" to "$destination"
    echo =====================================================================
    cp $f $destination
done

