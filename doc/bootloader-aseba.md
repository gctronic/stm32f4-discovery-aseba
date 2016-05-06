---
title: Using the Aseba bootloader on the STM32 Discovery
author: Antoine Albertelli, WISE Robotics
documentclass: article
urlcolor: blue
linkcolor: black
abstract: |
    The Aseba programming environment includes a bootloader protocol that can be used to upgrade nodes over different transport layers.
    Currently, this includes TCP, HTTP, CAN and serial ports.
    A port of the Aseba bootloader for STM32 was implemented, supporting upgrade via CAN.
    This document presents how to use it.
---


# Building the Aseba bootloader

From the bootloader source project directory (`aseba-bootloader`), run the following commands to import and build the dependencies.
This process is only required once.

```bash
git submodule update --init
pushd libopencm3
make
popd ..
```

Then to compile the bootloader itself run `make`, which must be redone after each changes.

# Flashing the Aseba bootloader

In this example we will assume the bootloader is flashed using JTAG.
It is also possible to use the DFU programming protocol, please see the corresponding documentation for details.

Once the device is attached, simply entering `make flash` will program the device.
*Note:* This requires OpenOCD to work.

# Building the application with bootloader support

![Flash layout comparison when building with or without bootloader. We can see that the data sections containing Aseba bytecode and configuration data are at the same location in both cases. This allows the user to add or remove the bootloader without losing their settings or programmed bytecode.](flash_layout.png)


The application should be recompiled with bootloader support because its vector table and the bootloader's will overlap causing issues.
First, switch back to the discovery project directory (`cd ..`)

To do so, start by cleaning the project with `make clean`, then compile it with bootloader support `make USE_ASEBA_BOOTLOADER=yes`.
It should create `build/aseba-discovery.hex` that we will flash in the next step.

*Note:* Hex files produced by this project requires Aseba version 1.5.4 or above to work.

# Starting asebaswitch

`asebaswitch` is a tool used to create a bridge between several Aseba interfaces.
We will use it to connect the Aseba translator to the network, allowing `asebacmd` to connect to it.

In a new terminal run `asebaswitch -d "ser:device=/dev/ttyUSB0"`, replacing `/dev/ttyUSB0` with the path to your adapter.
The `-d` flag dumps messages to the console, which can be useful for debugging.

*Note: *On OSX (and maybe on other platforms as well), the OS will ask if you want to authorise `asebaswitch` to listen for incoming network connections.
This is normal and you can allow it.

# Using asebacmd to write the firmware

So now we are ready to flash the node using `asebacmd`.
We assume that your node has the ID 42, which is the default.
In case you changed it replace it with the node ID of your choice.

```bash
asebacmd whex 42 build/aseba-discovery.hex
```

Now put the board in bootloader mode.
To do this you can either reset the board or use `asebacmd sb 42` if an Aseba-compatible firmware is flashed on the device.

# Exiting the bootloader

To leave bootloader mode you can simply reset the board and wait.
The bootloader will automatically timeout and execute the application after a few seconds.
You can also use `asebacmd eb 42` to ask node 42 to leave bootloader mode.
