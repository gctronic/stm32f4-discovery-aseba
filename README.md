Epuck Firmware
==============
Firmware for the Epuck 2

## Requirements

This project requires the following tools:

* A recent version of GCC/G++ for ARM (Tested with 4.9+)
* A C / C++ compiler for the host (only required for unit tests)
* [Packager](packager), which is a tool to generate makefiles with dependencies.
    Once you installed Python 3, it can be installed by running `pip3 install cvra-packager`.

### Quickstart
Make sure you have an ARM GCC toolchain and OpenOCD installed.

```bash
git submodule init
git submodule update

packager

make
make flash
```

To start the shell, open a terminal emulator and run

```bash
sudo python -m serial.tools.miniterm /dev/ttyACM0
```
 assuming `/dev/ttyACM0` is where the discovery is connected

### Running unit tests

When developping for this project you might want to run the unit tests to check that your work is still OK.
To do this you will need the following:

* A working C/C++ compiler
* CMake
* [Cpputest][cpputest] A C++ unit testing library

Once everything is installed you can run the following:

```
mkdir build
cd build
cmake ..
make check
```

[cpputest]: http://cpputest.github.io
[packager]: http://github.com/cvra/packager
