Nordic nRF5x Support Files
==========================

This repository is a starting point and shared code for Nordic nRF5x BLE platforms. This repo is
a collection of libraries, SDKs, Softdevices, and Makefiles to be included
within other projects using the Nordic platfroms. Pull requests welcome.

The currently supported SDK versions are: 9.0.0, 10.0.0, 11.0.0, 12.2.0, 14.1.0.

The currently supported Softdevice versions are:
s130_2.0.1, s140_6.0.0


Usage
-----

First, add this project as a submodule inside of your repo with your
nRF5x code.

    git submodule add https://github.com/lab11/nrf5x-base

Then write an application for the nRF5x SoC you are using and include
a Makefile that looks like this:

```make
PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configurations
NRF_IC = nrf52832
SDK_VERSION = 15
SOFTDEVICE_MODEL = s132

# Source and header files
APP_HEADER_PATHS += .
APP_SOURCE_PATHS += .
APP_SOURCES = $(notdir $(wildcard ./*.c))

# Include board Makefile (if any)
#include $(NRF_BASE_DIR)/boards/<BOARD_NAME_HERE>

# Include main Makefile
NRF_BASE_DIR ?= ../../
include $(NRF_BASE_DIR)/make/AppMakefile.mk
```

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

Flash an Application
--------------------

To flash an application, there is some setup
you must do.

1. Install the [`arm-none-eabi-gcc`](https://launchpad.net/gcc-arm-embedded) compiler.

    On Ubuntu:

        sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
        sudo apt-get update
        sudo apt-get install gcc-arm-embedded

2. Install Nordic's [command line
   tools](https://www.nordicsemi.com/DocLib/Content/User_Guides/nrf5x_cltools/latest/UG/cltools/nrf5x_installation)
   `mergehex` and `nrfjprog`. Ensure these tools are extracted and added to your path.

3. Install the JLink [software](https://www.segger.com/jlink-software.html)
for your platform. You want the "Software and documentation pack".

4. Acquire a [JLink JTAG programmer](https://www.segger.com/jlink-general-info.html).
The "EDU" edition works fine.

5. Program an app! With the JLink box attached to the target board:

        make flash

    will write the app and softdevice to the device. You can erase
    a chip with:

        make erase-all

    See the [make](https://github.com/lab11/nrf5x-base/tree/master/make) folder
    for a complete list of commands.

    Most of our boards use a [TagConnect header](http://www.tag-connect.com/TC2030-IDC-NL)
    instead of the way-too-large ARM JTAG header. We use [our own](https://github.com/lab11/jtag-tagconnect)
    adapter, but Segger also makes [one](https://www.segger.com/jlink-6-pin-needle-adapter.html).

5. Upon inital programming of an NRF51822, the nRF will enter debug mode, which will prevent the nRF from sleeping and
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

License
-------

The files in this repository are licensed under the [MIT License](LICENSE)
unless otherwise noted by the local directory's README and license files.

