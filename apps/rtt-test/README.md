RTT (Real Time Transfer) Test App
=================================

This will do debug printf over the Segger JLink.

After flashing this app, run

    JLinkExe -device nrf51822 -if swd -speed 1000

in one terminal and

    JLinkRTTClient

in another.
