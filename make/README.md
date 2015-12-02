nRF5x Make System
=================

This folder contains the GCC build system for the nRF5x platforms. It
originally comes from
[hlnd/nrf51-pure-gcc-setup](https://github.com/hlnd/nrf51-pure-gcc-setup).

We develop on Linux. Original files still exist for Windows, but have not been
tested in some time. Feel free to submit a pull request if you find something
that doesn't work.

The currently supported SDK versions are: 9, 10

The currently supported Softdevice versions are: s110_8.0.0, s120_2.1.0, and s130_1.0.0

Things to Install
-----------------
1. gcc-arm-none-eabi
2. gdb-arm-none-eabi
3. The jlink tools for linux
4. The jlink debuger for linux

Usage
-----

See the `apps/` folder for an example of how to use this build system. An
example Makefile is in the root of this repository.

Additionally, you can customize things inside of your application makefile.
The following variables can all be set:

These are required:
- `APPLICATION_SRCS`   : List of all .c files to be compiled.
- `LIBRARY_PATHS`      : List of directories with .h files
- `SOURCE_PATHS`       : List of directories with .c files
- `SOFTDEVICE_MODEL`   : s110 | s120 | s130 | s210 | s310 | or do not set for no softdevice

These are optional
- `SOFTDEVICE_VERSION` : Full version number of the softdevice.
- `SOFTDEVICE`         : Path to the softdevice to use
- `START_CODE`         : .s file to execute first
- `SYSTEM_FILE`        : Base nRF .c file.
- `NRF_MODEL`          : nrf51 | nrf52  : Set by the softdevice used
- `BOARD`              : Hardware board being used. Will get set as #define.
- `RAM_KB`             : Size of RAM on chip    : Defaults to 16
- `FLASH_KB`           : Size of flash on chip  : Defaults to 256
- `SDK_VERSION`        : Major version number of the SDK to use. Defaults to 10
- `GDB_PORT_NUMBER`    : Defaults to 2331

If you want to use the GDB functionality with multiple J-Links, you should
make sure that all projects have a unique GDB port number defined in their
project Makefile.


Targets
-------
Most of the targets provided should be self explanatory, but some may use some
extra explanation:

- `flash`

    Build project and flash onto a chip. Also checks that the correct softdevice is
    already on the chip, and automatically runs flash-softdevice if not.

- `flash ID=XX:XX:XX:XX:XX:XX`

    Sets the Bluetooth ID for the chip to whatever replaces XX:XX:XX:XX:XX:XX (must
    be valid hex digits). Bluetooth ID is written to the top of flash and persists
    across future flashes (but not erase-alls). Application code needs to read the
    value from flash and actually set it as the Bluetooth ID.

- `erase-all`

    Does an erase all of a chip.

- `flash-softdevice`

    Used to flash a softdevice to a chip. (Note, this is done automatically by
    make flash). Flashes softdevice as specified in the project Makefile.

- `flash-softdevice SOFTDEVICE=<PATH_TO_SOFTDEVICE_WITHOUT_SPACES>`

    Flashes a specific version of the softdevice. The path to the softdevice hex
    needs to be without spaces, due to Make limitations.

- `debug`

    Makes with debug symbols. Use before startdebug.

- `startdebug`

    Starts a J-Link GDB Server in a separate terminal window, and then GDB
    also in a separate window. If you change the code, you can then make directly
    from gdb, and do load to run the new code.

    To use this feature you must enable the working path as safe! you can also
    enable all paths by adding 'set auto-load safe-path /' to ~/.gdbinit.

- `recover`

    Provides equal functionality to that of nrfjprog / nRFgo Studio on Windows.


If you have multiple J-Links connected to your system, you should
set the `SEGGER_SERIAL` variable to the serial number of your J-Link, so that
the programming Just Works (tm). It seems Segger on Linux isn't capable of
giving you a selection of J-Links, as on Windows.


Verbose Output
--------------

To see the full commands that are being run by make, do:

    make V=1

