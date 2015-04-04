7 segment display clock project
===============================

Source code of my [7 segment display clock project](http://ahtoxa.net/micros/stm32/7-seg-clock/) project (RUS).
To clone project with submodules use
`git clone --recursive https://github.com/antongus/clock-7-seg.git`

####Prerequisites:
- GCC cross-compiler for ARM Cortex-M3 controllers (I use [gcc-arm-embedded](https://launchpad.net/gcc-arm-embedded));
- st-flash tool from [texane/stlink](https://github.com/texane/stlink) on linux, or
- ST-LINK_CLI utility from ST on Windows.

####Build and flash instructions:
- type `make` on shell prompt to build;
- `make program` to load program into microcontroller.

Instead of typing commands on shell prompt one can use Eclipse CDT IDE to build project and flash microcontroller.
Just load Eclipse project and select `build` and `program` from "make targets" window.
