# RhythKey Pro User Manual

## 3D Printing the Controller Case

Please go to [Releases](https://github.com/pStrikeZ/RhythKeyPro/releases) to download the `.3mf` model files for the case,  
or use your favorite 3D modeling software to open and modify the `.step` files in the `/case` directory.  
You can contact a friend with a 3D printer or use a paid 3D printing service on the internet to print them.

The 3mf file with the `with Text` suffix is ​​a version with logos and memes. If your chosen 3D printing service provider supports multi-color material mixing printing, it is recommended to use this model file and fully inform the provider of the color configuration used when submitting the printing request.

![](img/0DF8D29E-5A36-44F4-87DD-D1579D7350B3.png)

## PCB Printing

You can use [JLCPCB EDA Pro](https://pro.lceda.cn/) to log in and import the `.epro2` format files located in the `/pcb` directory, export the Gerber files, and then go to JLCPCB's online PCB ordering service to print the PCB.

Alternatively, if you do not need to review or modify the PCB, you can go directly to [Releases](https://github.com/pStrikeZ/RhythKeyPro/releases) to download the Gerber files.

It is recommended to choose a white PCB to make the silkscreen on the front of the circuit board look more beautiful.

Note: Since I used some footprints without electrical information when drawing the PCB, the electrical schematic function in the EDA has no reference value.

## BOM Component Sourcing

First, you need to purchase the electronic components for the BOM. The list of these components is as follows, roughly divided into two categories: `Mandatory Accessories` and `Optional Accessories`. However, it is recommended to purchase and assemble all accessories.

### Mandatory Accessories

* 1x [Sparkfun Arduino Pro Micro](https://www.sparkfun.com/pro-micro-3-3v-8mhz.html)  
It can also be a compatible version produced by other manufacturers, as long as it is a module equipped with the ATMega32U4 main control chip.  
Do not buy the version using the Mini-USB interface, as its PCB size does not match our PCB, ~~and I believe you should find it hard to find such a data cable in 2026~~.  
It is recommended to buy a version with pre-soldered pins for convenient and quick use.
* 12x Your favorite 5-pin mechanical keyboard switches  
When I was improving the design of this rhythm game controller, a friend asked me if I could use optical switches or electrostatic capacitive switches to improve the absolute gaming experience in rhythm game scenarios.  
Regrettably, the answer is no. The actuation force / travel distance adjustment functions of optical or electrostatic capacitive switches cannot be implemented using an Arduino-based main control scheme by a student without the help of any professional tuning equipment.
* 12x Kailh mechanical keyboard switch sockets
* 8x 1U size mechanical keyboard keycaps and 4x 2.75U size mechanical keyboard keycaps
* 4x 2U size mechanical keyboard stabilizers  
Most of the time, you will be bundled by the seller with a 6.5U or 7U length stabilizer suitable for the space bar of a mechanical keyboard, which is normal.
* 2x ALPS EC11 series rotary potentiometers and 2 potentiometer knobs  
The EC11 potentiometers imported from the original ALPS brand have two versions: `EC11E09244BS` with detents and `EC11E1834403` without detents.  
I have purchased both versions of the potentiometers, and I strongly recommend buying the `EC11E1834403` model without detents. Because the potentiometer with detents has a strong tactile bump every time it is rotated, it will feel very strange when playing rhythm games that require knob turning (SOUND VOLTEX).  
The EC11 series potentiometers imported from Japan are very expensive. You can buy low-cost domestic alternative EC11 potentiometers, but I cannot guarantee the gaming experience when using such third-party potentiometers.  
When purchasing potentiometer knobs, please ensure that the internal shape of the knob is consistent.
* 1x ALPS B10K 75mm slide potentiometer and 1 slide potentiometer fader cap  
You can also buy a domestic alternative version of the B10K 75mm slide potentiometer, but I cannot guarantee the gaming experience when using such third-party potentiometers.
* 7x 6mm*6mm SMD tactile push button switches, button height 7.5mm
* 4x R0805 10KΩ SMD resistors
* 16x R0805 100nF SMD capacitors
* 1x R0805 330Ω SMD resistor
* 12x R0805 1Ω SMD resistors
* 12x WS2812 reverse-mount RGB LED beads  
Be sure to buy the reverse-mount version of the beads, otherwise they cannot be soldered to the PCB.
* 18x R1206 1N4148 switching diodes
* 3x 1N4007 SMA diodes
* 6x M2*8 self-tapping screws
* 5x M2*16 self-tapping screws
* Soldering iron with adjustable temperature, solder wire, soldering flux, rosin, **and a pair of curved tweezers**  
*Or if you have a hot air gun, maybe try reflow soldering?*

### Optional Accessories

* 2x 2.54mm single-row female headers  
If you do not consider the hot-swappable feature of the module, you can solder the module directly to the motherboard, although I do not recommend doing so.
* 6x 6mm thick rubber feet  
When using header sockets to connect the Arduino module, the height of the module will exceed the thickness of the 3D printed parts, so several feet are needed to raise it. Raising the knob side by 12mm and the user side by 6mm is sufficient.
* 12x Mechanical switch pads  
Mainly because I really don't think a few pieces of foam can change the sound of the keys of a rhythm game controller with a 3D printed shell to any extent, so it's better not to stick anything. But since you can choose to stick them, I'll mention it here.

## Soldering

Except for the 6 SMD tactile switches located on the front of the motherboard, all SMD electronic components are designed on the back of the motherboard. The silkscreen directly indicates the component model, just put them in the right place.  
After completing the soldering of the SMD components on the back, solder the potentiometers, front SMD switches, female headers, and other parts.  
The completed circuit board should look like the picture below, **although the PCB in the picture is a former Prototype version and is inconsistent with the PCB in the repository**:

![](img/photo_2025-09-07_23-01-35.jpg)

![](img/photo_2025-09-07_23-01-37.jpg)

## Assembly

* Embed the stabilizers into the top case
* Place the PCB in the bottom case and screw in the M2*8 self-tapping screws
* Place the top case on the bottom case and screw in the M2*16 self-tapping screws
* Install mechanical switches, knob caps, potentiometer caps, and keycaps  
If you feel that the potentiometer cap is very loose on the potentiometer, you can stuff a small piece of paper inside the potentiometer cap, which will increase the friction between the cap and the potentiometer.

The assembled product is shown in the figure:

![](img/photo_2025-09-07_21-05-48.jpg)

## Firmware Flashing

### First Time Flashing

First, download and install [Arduino IDE](https://www.arduino.cc/en/software), open the Preferences tab.

Enter `https://raw.githubusercontent.com/dmadison/ArduinoXInput_Boards/master/package_dmadison_xinput_index.json` in Additional Boards Manager URLs, as shown in the figure. If there is already other content in this line, append it to the end, separated by a semicolon.

![](img/4AF872F5-66ED-403F-9668-8D7AD8C9A4F6.png)

Then open Boards Manager, search for and install XInput SparkFun Boards.

![](img/C669478A-9825-449B-BC53-AC3749E21E64.png)

Next, open Library Manager, search for and install the XInput library and FastLED library.

![](img/5DD11E6F-D4BB-4D52-8B6E-4DE5271D5FEE.png)

![](img/446A5C16-07A0-4C32-A101-41BE26E88EA4.png)

Then, connect the USB-C interface of the assembled RhythKey Pro to the computer.  
When flashing the firmware for the first time, the exposed COM port can be recognized without the RESET button. Click the pen icon next to the recognized serial port, search for and select `SparkFun Pro Micro w/ XInput`, select the corresponding port on the right, and then select OK.

![](img/A767EAC8-6E66-4B97-9027-A1FE7ACB7AE0.png)

![](img/CAFC25C6-4EF5-4317-B0F2-D91EAE2669FF.png)

Next (very important!), click the Tools tab. There is a `Processor: ATMega32U4` option. Click it and be sure to select the `ATMega32U4 (5V, 16 MHz)` item in the pop-up menu, otherwise the module will not be recognized after flashing the firmware.

![](img/4F5AFB17-A21A-4B92-8C3F-19AF3A24C8E1.png)

Then open the `rhythkeyoro_fw.ino` file in the repository, click the `Upload` button in the upper left corner, and wait for the firmware burning to complete.

### Firmware Updating

Since the Pro Micro module will become an XInput device after flashing the XInput firmware, the Arduino IDE will not be able to recognize it. At this time, you need to press the RESET button of the Pro Micro module to make it re-enter Bootloader mode before firmware updating.

This Bootloader mode will only last for 8 seconds, so it will test the user's proficiency in computer operations.

First open the Arduino IDE, press the RESET key once to make the module enter Bootloader mode.  
You need to complete the binding settings of the module port and board type within these 8 seconds (click the pen icon, select the port, select the board type).  
Then, with the module NOT in Bootloader mode, click the Upload button to burn the firmware.  
After the Arduino IDE completes firmware compilation (as shown in the figure below) and displays Uploading, immediately press the RESET button to make it re-enter Bootloader mode, so that the new firmware can be successfully flashed.

![](img/26B513CE-9E7E-43BD-9B95-F8A1E58A603F.png)

## Game Settings

**Disclaimer**: This project does not provide installation files for any games or tutorials on how to use the games themselves.

Press the SMD button in the lower right corner of the controller panel to switch between ONGEKI mode and VARIOUS mode.

In ONGEKI mode, the menu key needs to be pressed and held for 3 seconds to be triggered;  
In VARIOUS mode, the side keys and menu keys will be directly disabled to prevent accidental touches during the game.

### ONGEKI

Open `segatools.ini`, adjust other settings, and scroll to the end of the configuration file.

```ini
[io4]
enable=1
; Test button virtual-key code. Default is the F1 key.
test=0x70
; Service button virtual-key code. Default is the F2 key.
service=0x71
; Keyboard button to increment coin counter. Default is the F3 key.
coin=0x72

; Set "1" to enable mouse lever emulation, "0" to use XInput
mouse=0
```

Change the `mouse` item to `0`, then start the game, press `F1` to enter test mode, select `レバー設定` (Lever Settings) and enter.

![](img/A0AC07B9-7E17-4014-87DB-FF74D8B1B509.png)

![](img/F37B6166-CD39-4C0E-A306-2C2299E3DFFE.png)

First select `初期設定に戻す` (Reset to Default), then slowly move the potentiometer to the far left, and then to the far right.  
Next select `終了` (Exit), then `保存する` (Save). After completing other necessary settings, exit test mode and start enjoying the game.

### SOUND VOLTEX

First switch to VARIOUS mode.

Run `spicecfg.exe` or press the `F4` button after entering the game to open the spicetools settings interface.

![](img/69BDBD09-95C8-41A5-897E-45F3F30AEDA7.png)

Click the `Bind` buttons one by one as shown in the figure, and then press the corresponding button on the controller to bind.  
After binding the buttons, switch to the `Analogs` tab.

![](img/5417F45D-8848-4352-8696-1087133996B3.png)

Bind the two knobs to the `X` axis and `Y` axis of the controller stick respectively, and enable `Smooth Axis`, then exit the settings and start enjoying the game.

### DJMAX RESPECT V

Switch to VARIOUS mode, enter the game, go to Settings -> Control -> Input Settings -> Edit.

![](img/9A561E22-5F77-403A-9E1D-07CB06490577.png)

After setting up the key mapping, start enjoying the game directly.

### osu!

Use a similar method to DJMAX to set up key mapping and then start enjoying the game.

### In Falsus Demo

Switch to VARIOUS mode, and then make sure to import this layout in Steam.

steam://controllerconfig/4355100/3667527437

Then in Steam Settings -> Controller -> Calibration & Advanced Settings, turn off the deadzone for the right stick, and then enter the game.

Note: Depending on the size of different monitors, you may need to fine-tune the vertical position and size of the mapped stick area yourself.
