---
title: Aseba CAN bootloader protocol
author: Antoine Albertelli, WISE Robotics
documentclass: article
urlcolor: blue
linkcolor: black
---


plan:

1. Intro
2. CAN frame description
3. How does Aseba put data in those
5. Commands

# General protocol construction
The Aseba protocol is built on top of the Controller Area Network (CAN) protocol.
It uses the following features:

* CAN2.0A type frames with a 11 bit identifier.
    This ID is used to indicate packet type and source ID.
* 8 bytes packet.
    Basic framentation support is implemented, but not used by the bootloader.

## Short packets

Short packets are used whenever the length of the message is smaller than 8 bytes.
It is also the only message type used by the bootloader.
The short packet type is `0x03`.

## Normal packets

Normal packets are used to transmit longer messages.
They consist of a start packet (type `0x01`), N data packets (type `0x00`) and 1 stop packet (type `0x02`).
They are not implemented in the STM32 bootloader.


# Aseba bootloader commands

Commands are made of at least one 16 bit number indicating the command type.
They can be followed by some arguments, which are all 16 bit numbers.

Since the bootloader protocol only uses short packets, the message must fit in a single frame (8 bytes).
This limits the argument count to a maximum of 3, plus the message ID.

All the bootloader-related command types start at 0x8000.

## PUSH_ACK (0x8006)
This message is used to send a reply from the node being programmed to the node doing the programming.
It contains a single argument, which is the error code.

## CMD_RESET (0x8000)
When receiving this command the node must reply then boot into user code.
This command does not take any argument.
It replies with a `PUSH_ACK` message with error code `REPLY_OK`.

## CMD_READ_PAGE (0x8001)
This command is used to read a page from the flash for verification purposes.
It takes one argument, which is the page number to read.

It must reply with a series of `PUSH_PAGE_DATA` messages containing the read data.

## CMD_WRITE_PAGE (0x8002)
This command puts the device in programming mode.
It takes a page number as parameter and replies with `REPLY_OK`.
The device will then wait for a serie of `CMD_PAGE_DATA` messages and writes them to the given flash page.

If the page number is invalid, it will reply with `REPLY_INVALID_VALUE` instead.

## CMD_PAGE_DATA (0x8003)
This command contains 4 byte of page data to write to flash.
It must have been preceded by a `CMD_WRITE_PAGE`.

Possible replies are:

* If the device is not in programming mode, it will reply with `REPLY_NOT_PROGRAMMING`
* If the frame contains the last bytes of the page the device will write it to its internal memory and reply with `REPLY_OK`.
* Otherwise it does not reply.

## PUSH_DESCRIPTION (0x8004)
This message type is sent from the slave to the programming node at boot.
It contains the following 16 bits fields:

1. The supported page size, in bytes.
2. The index of the first available page.
3. The number of available page.
