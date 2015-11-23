Nordic nRF5x Support Files
==========================

This repository is a starting point and shared code for Nordic nRF5x BLE platforms. This repo is
a collection of libraries, SDKs, Softdevices, and Makefiles to be included
within other projects using the Nordic platfroms. Pull requests welcome.

The currently supported SDK versions are: 9.0.0

The currently supported Softdevice versions are: s110_7.3.0, s110_8.0.0, s120_2.1.0, and s130_1.0.0


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
        /<my app name>
            Various .c and .h files for your application
            Makefile (copy from Makefile.example and modify as needed)
        /<my other app name>
        ...
    /src
        various platform-level code (e.g. functions shared between applications)
    /include
        various platform-level headers (e.g. platform pin mappings)
    /nrf5x-base (submodule)
```

An example of this project in use can be found
[here](https://github.com/helena-project/squall/tree/master/software/apps/beacon).


If you're using submodules in your project, you may want to use this to make
git automatically update them:
https://gist.github.com/brghena/fc4483a2df83c47660a5


Development Tools
-----------------

When developing a BLE application, several tools exist to make your life easier.
The easiest option, if you have access to an android phone, is [nRF Master Control Panel](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en).
On iOS, [LightBlue Explorer](https://itunes.apple.com/us/app/lightblue-explorer-bluetooth/id557428110?mt=8)
has similar or better functionality. Alternatively,
[noble](https://github.com/sandeepmistry/noble) is a NodeJS library for interacting with BLE that can run from
a Linux or Mac computer.




