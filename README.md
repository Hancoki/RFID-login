# RFID-login

![RFID login render](images/rfid-login-render.png)

## Introduction

This is an RFID login device based on the microcontroller Sparkfun Pro Micro. It allows easy to type in your login credentials in same way as your keyboard does. But it seems a little bit smarter.

## Description

![RFID login schematic](images/rfid-login-schematic.png)

In first step I developed this device as a prototype on a breadboard. For the device I used the following components:

### Bill of material

- SparkFun Pro Micro 3.3V/8MHz microcontroller
- RFID MFRC522 reader
- circuit board size 70mm x 50mm
- USB mini cable (USB-A male/USB-B male) ~1m
- resistors 3x 200Ohm, 1x 100Ohm
- RGB led
- switch
- piezo buzzer

### Wiring

As you can see in schematic above I wired the pins of microcontroller as follows:

Pin | Function
--- | --------
4   |
5   |
6   |
7   |
8   |
9   |
10  |
14  |
15  |
16  |
GND |
VCC | 3.3V (RFID reader)

### Programming

For programming the Pro Micro I used [PlatformIO](https://github.com/platformio/platformio-core) inside of [VS CODE](https://github.com/microsoft/vscode).

Included libraries:
- [Keyboard library for Arduino](https://www.arduino.cc/reference/en/language/functions/usb/keyboard/)
- [Arduino library for MFRC522 by miguelbalboa](https://github.com/miguelbalboa/rfid)

## Feedback

If you have any questions or ideas for improvement, please let me know and write an issue. Thanks for your participation!