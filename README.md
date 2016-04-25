Epuck Firmware
==============
Firmware for the Epuck 2

### Quickstart
Make sure you have an ARM GCC toolchain and OpenOCD installed.

```bash
git submodule init
git submodule update

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
