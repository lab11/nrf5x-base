#!/bin/bash

declare -a list=(         
         "lib/mbedcrypto/cc310/armgcc"
         "lib/mbedcrypto/software/armgcc"
         "lib/openthread/mtd/armgcc"
         "lib/openthread/ftd/armgcc"
         "lib/openthread/ftd/armgcc"
         "lib/openthread_cli/mtd/armgcc"
         "lib/openthread_cli/ftd/armgcc"
         "lib/openthread_diag/armgcc"
         "lib/openthread_ncp/ftd/uart/armgcc"
         "lib/openthread_ncp/ftd/spi/armgcc"
         "lib/openthread_ncp/mtd/uart/armgcc"
         "lib/openthread_ncp/mtd/spi/armgcc"
         "lib/openthread_nrf52840/sdk/uart/armgcc"
         "lib/openthread_nrf52840/sdk/usb/armgcc"
         "lib/openthread_nrf52840/softdevice/uart/armgcc"
         "lib/openthread_nrf52840/softdevice/usb/armgcc"
         "lib/openthread_platform_utils/armgcc"
         )

for p in "${list[@]}"
do
    echo =====================================================================
    echo Building "$p"
    echo =====================================================================
    pushd $p
    make -j
    popd
done
