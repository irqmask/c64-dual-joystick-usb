# Overview

When the c64-dual-joystick-usb adapter is connected, two joystick devices will show up in the operating system.



# Compile

## Prerequisites

The following packages are needed to cross-compile under Ubuntu-like operating systems:
    avr-libc avrdude binutils-avr gcc-avr make

## Compile

    cd c64-dual-joystick-usb
    make
    make program

Type 'make' to compile the firmware and 'make program' to program the firmware into the device.
When using 'make program' the Makefile needs to be adjusted matching the programmer.

