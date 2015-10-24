nrf5x-base Files
================

Starting point and shared code for Nordic nRF5x BLE platforms. This repo is
a collection of libraries, SDKs, Softdevices, and Makefiles to be included
within other projects using the Nordic platfroms. Pull requests welcome.

The currently supported SDK versions are: 9.0.0

The currently supported Softdevice versions are: s110_7.3.0, s110_8.0.0, s120_2.1.0, and s130_1.0.0


Usage
-----
Submodule this project within your own repo, and point to it in your Makefile.
The expected directory structure is:
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

An example Makefile is included in this repo as Makefile.example. Copy to your
own application directory and modify as desired.

An example of this project in use can be found
[here](https://github.com/helena-project/squall/tree/master/software/apps/beacon).


If you're using submodules in your project, you may want to use this to make
git automatically update them:
https://gist.github.com/brghena/fc4483a2df83c47660a5

