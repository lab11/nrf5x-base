Nordic nRF5x Support Files
==========================

This repository is a starting point and shared code for Nordic nRF5x BLE platforms. This repo is
a collection of libraries, SDKs, Softdevices, and Makefiles to be included
within other projects using the Nordic platfroms. Pull requests welcome.

The currently supported SDK versions are: 9.0.0, 10.0.0, 11.0.0, 12.2.0.

The currently supported Softdevice versions are:
s110_7.3.0, s110_8.0.0, s120_2.1.0, s130_1.0.0, s130_2.0.0, s130_2.0.1.


Usage
-----

First, add this project as a submodule inside of your repo with your
nRF5x code.

    git submodule add https://github.com/lab11/nrf5x-base

Then write an application for the nRF5x SoC you are using and include
a Makefile that looks like this:

```make
PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
# Various C libraries that need to be included
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += ble_srv_common.c
APPLICATION_SRCS += app_util_platform.c
APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += nrf_delay.c
APPLICATION_SRCS += led.c
APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += simple_adv.c
# Add other libraries here!

# platform-level headers and source files
LIBRARY_PATHS += ../../include
SOURCE_PATHS += ../../src

# Set the softdevice needed for the application
SOFTDEVICE_MODEL = s110

# Include the main Makefile
NRF_BASE_PATH ?= ../../nrf5x-base
include $(NRF_BASE_PATH)/make/Makefile
```
An example Makefile is included in this repo as Makefile.example. Copy to your
own application directory and modify as desired.

Generally, the expected directory structure for your project is:
```
    /apps
        /<application 1>
        /<application 2>
        ...
    /src
        various platform-level code (e.g. functions shared between applications)
    /include
        various platform-level headers (e.g. platform pin mappings)
    /nrf5x-base (submodule)
```

Example Applications
--------------------

This repo has several example and test applications. See the
[apps](https://github.com/lab11/nrf5x-base/tree/master/apps)
folder.

Supported Features
--------------

There are libraries for many common BLE functions in this repo:

- `simple_ble`: Quick interface to most common BLE functions
  - BLE Advertisements
    - Device name only
    - Manufacturer data
    - Eddystone
    - Rotating multiple advertisements
  - BLE Services
- SQL style database ([LittleD](https://github.com/graemedouglas/LittleD))
- [RTT Debugging](https://www.segger.com/pr-j-link-real-time.html)
- Nordic [BLE Serialization](http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk51.v10.0.0%2Fble_serialization_s110_events.html)
- Nordic DFU over-the-air reprogramming.



Program a nRF51822
------------------

To flash an application to a nRF51822 BLE chip, there is some setup
you must do.

First, install the [`arm-none-eabi-gcc`](https://launchpad.net/gcc-arm-embedded) compiler.

    On Ubuntu:

        sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
        sudo apt-get update
        sudo apt-get install gcc-arm-embedded

### With JLink

1. Install the JLink [software](https://www.segger.com/jlink-software.html)
for your platform. You want the "Software and documentation pack".

2. Acquire a [JLink JTAG programmer](https://www.segger.com/jlink-general-info.html).
The "EDU" edition works fine.

3. Program an app! With the JLink box attached to the target board:

        make flash

    will write the app and softdevice to the nRF51822. You can erase
    a chip with:

        make erase-all

### With STLink/v2

1. Install [openocd](http://openocd.org/), an On-Chip Debugging / In-System Programming tool.

    On Ubuntu:

        sudo apt-get install openocd

2. Get a [STLink/v2 programmer](http://www.st.com/en/development-tools/st-link-v2.html).
There are also popular unofficial versions described [here](http://wiki.sgmk-ssam.ch/wiki/STM32_dev#ST-Link_V2_Programmer) in more detail.

3. Add `PROGRAMMER = stlinkv2` to your Makefile.

4. Program an app! With the STLink attached to the target board:

        make flash-softdevice # only necessary the first time, after erase of if you change the softdevice
        make flash

    will write the app and softdevice to the nRF51822. You can erase
    a chip with:

        make erase-all


See the [make](https://github.com/lab11/nrf5x-base/tree/master/make) folder for a complete list of commands.

Most of our boards use a [TagConnect header](http://www.tag-connect.com/TC2030-IDC-NL) instead of the way-too-large ARM JTAG header. We use [our own](https://github.com/lab11/jtag-tagconnect) adapter, but Segger also makes [one](https://www.segger.com/jlink-6-pin-needle-adapter.html).

5. Upon inital programming, the nRF will enter debug mode, which will prevent the nRF from sleeping and 
   prevent the reset line from working. To fix this, either perform a powerdown/powerup or download nrfjprog from 
   (https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF51822) and run nrfjprog --pinreset

Git Submodules
--------------

If you're using submodules in your project, you may want to use this to make
git automatically update them:
https://gist.github.com/brghena/fc4483a2df83c47660a5


BLE Tools for Other Platforms
-----------------

When developing a BLE application, several tools exist to make your life easier.
The easiest option, if you have access to an android phone, is [nRF Master Control Panel](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en).
On iOS, [LightBlue Explorer](https://itunes.apple.com/us/app/lightblue-explorer-bluetooth/id557428110?mt=8)
has similar or better functionality. Alternatively,
[noble](https://github.com/sandeepmistry/noble) is a NodeJS library for interacting with BLE that can run from
a Linux or Mac computer.

Example Platforms Using nRF5x-base
----------------------------------

- [Squall](https://github.com/helena-project/squall)
- [BLEES](https://github.com/lab11/blees)
- [Nucleum](https://github.com/lab11/nucleum)
- [PolyPoint](https://github.com/lab11/polypoint)
- [PowerBlade](https://github.com/lab11/powerblade)


License
-------

The files in this repository are licensed under the [MIT License](LICENSE)
unless otherwise noted by the local directory's README and license files.

