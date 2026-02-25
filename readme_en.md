# RhythKey Pro

![](doc/img/photo_2025-09-07_21-05-48.jpg)

# [中文文档](readme.md)

## Introduction

This is a DIY mini portable rhythm game controller.  
It controls rhythm games by simulating an Xbox 360 controller based on an `ATmega32U4` Arduino module.

Supported rhythm game list:

*   ONGEKI
*   SOUND VOLTEX
*   DJMAX RESPECT V
*   osu!mania
*   In Falsus Demo

There are also some untested games that should theoretically work, such as other modes of osu!, Hatsune Miku: Project DIVA, etc.

## Roadmap

Currently, the project is complete enough for normal gameplay, but there is still room for improvement:

- [x] Adjust the thickness of the switch clip part of the top case model
- [x] Write a software wrapper to get real-time lighting information from ONGEKI's lighting effect pipe, and encode it into vibration data streams to push to the controller's RGB LEDs for real-time changes
- [ ] Fine-tune the precision of the knob and eliminate the need for `Smooth Axis` settings
- [ ] Redraw the EDA project files to split the PCB into two parts, bypassing JLCPCB's free prototyping size limit and reducing the board production cost

No ETA is provided; I don't know how long these improvements will take. Of course, PRs are warmly welcomed.

## Assembly Guide

Please refer to the [Manual](doc/readme_en.md) for detailed steps.

## Why Not ESP32?

The core function of this project is to simulate an Xbox 360 controller (an XInput device), which requires the main control chip to have **native USB Device** capability and the ability to customize device descriptors.

`ATmega32U4` has a hardware-level USB controller. With the [ArduinoXInput](https://github.com/dmadison/ArduinoXInput) library, it can directly enumerate the device as an XInput controller, plug-and-play and driver-free on Windows. This is the decisive reason for choosing it for this project.

As for the ESP32 series:

* **ESP32-C3**: Only has a USB Serial/JTAG interface and **does not support USB Device mode**. It can only simulate a controller via Bluetooth, and honestly, playing rhythm games with a Bluetooth controller is just having too much free time on your hands.
* **ESP32-S2 / S3**: Supports USB OTG and theoretically can implement XInput via TinyUSB, but requires writing custom USB descriptors and protocol stack adaptation. The development complexity is far higher than the mature solution of ATmega32U4.

In terms of performance, the workload of this project (button matrix scanning, encoder reading, ADC sampling, WS2812 driving, XInput reporting) is more than enough for a 16MHz AVR, with no performance bottlenecks.

Considering compatibility, development costs, and ecosystem maturity comprehensively, `ATmega32U4` is the optimal choice for USB game controller projects.

If you just can't stand Arduino, you are welcome to fork the project and change it yourself.

## Acknowledgements / Project Origins

This project is a complete refactor and upgrade of [Rhythkey_V1](https://github.com/TheSiriusS233/Rhythkey_V1).  
We express our deep respect and gratitude to the original author [TheSiriusS233](https://github.com/TheSiriusS233) for their excellent work.  
If you are interested in the original version, please visit its official repository.

## License

This project uses a hybrid licensing model:

*   **Case Part** (including all 3D models and files in the `/case` directory)  
    Licensed under [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).
*   **PCB Part** (including all EDA files, schematics, PCB layouts, etc., in the `/pcb` directory)  
    Licensed under [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).
*   **Software Part** (including all Arduino source code in the `/firmware` directory)  
    Licensed under [GNU General Public License v3.0 (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.html).

In short, you are welcome to learn from, modify, and share this project for non-commercial purposes. However, using the hardware design of this project directly for any commercial production or sales is strictly prohibited.  
Also, please do not use this project to manufacture units for others for profit (commission-based), as this also violates our terms.
