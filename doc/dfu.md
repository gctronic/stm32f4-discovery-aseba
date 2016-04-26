---
title: Using the DFU bootloader on the STM32 Discovery
author: Antoine Albertelli, WISE Robotics
documentclass: article
linkcolor: blue
---

# Introduction
Most microcontrollers are programmed using JTAG for flashing.
However this is not always practical: JTAG adapters are not always available or JTAG connections might not be available for the end user.
To solve this problem a bootloader is employed, which is a small software used to self-update the microcontroller using a standard communication port such as CAN, Ethernet, UART, etc.

STM32 microcontrollers are burned with such a bootloader in factory in a read only memory.
This allows us to update the firmware using USB Device Firmware Upgrade (DFU) standard (other communications methods are possible).
DFU is an official USB device class which specifies a vendor and device independent way of updating the firmware of a USB device.
It can be used on a wide range of devices, from microcontrollers to smartphones.

# Installing the requirements
To use the bootloader you will need a host PC running an implementation of the DFU protocol.
The one we recommend using is called [dfu-util][dfu-util] and is available on many platforms, including Windows, Linux and OSX.

It can be installed on Ubuntu by running the following command:

```bash
sudo apt-get install dfu-util
```

Or on OSX, using [Homebrew][homebrew]:

```bash
brew install dfu-util
```

# Preparing the board
According to the STM32F407 Reference Manual, section 2.4 (Boot configuration), the bootloader resides in system memory.
To instruct the CPU to boot from system memory we must pull BOOT0 to 1 (high) and BOOT1 to 0 (low).

*Note:* Since pin BOOT1 is also a GPIO it is named PB2 on some schematics or boards, including the Discovery.

*Note:* To avoid the bootloader entering UART or CAN mode you should stop the traffic coming to those pins.

![Connections required for the DFU bootloader. The left USB is only required to power the board and can be ignored in case the board has an external power supply.](dfu-connections.jpg)

# Flashing the firmware

Now that our board is ready, we can start the flashing process itself.
We assume that your firmware is now stored in the file `firmware.bin`.
When building the Aseba demo for the Discovery the file can be obtained by running `make ch.bin` and will be located in `build/ch.bin`.
We will flash the code at the beginning of the flash (0x08000000).

```bash
dfu-util -d 0483:df11 \ # VendorID:ProductID for STM32F407.
        -c 1 -a 0 -i 0 \ # Config of the device interface, do not change
        -s 0x08000000 \ # Start address
        -D firmware.bin # Update operation
```

The vendor id and product id can be obtained from `System Information.app` on OSX and `lsusb` on Linux.

Once the flashing operation is complete, you can remove the bridge between BOOT0 and 5V.
Reset the microcontroller, and you are done.

[dfu-util]: http://dfu-util.sourceforge.net/
[homebrew]: http://brew.sh/
