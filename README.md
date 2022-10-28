# Wemos D1 Mini Hardware Watchdog
An ATtiny85 based hardware watchdog in the Wemos D1 Mini/Pro form factor, able to be used as a shield for the Wemos boards. 

## Overview
This watchdog timer listens for a heartbeat signal from the Wemos D1 Mini/Pro and if no signal has been recieved during the 
pre-defined period the Wemos will be rebooted (along with the watchdog timer).
The ATtiny firmware allows for separate start-up and heartbeat delays to be defined and, in conjunction with the custom PCB, allows
these to be adjusted via solder bridges to cater for different applications without the need to re-flash the ATtiny firmware.

The custom circuit board also allows the pin that is used to listen for the heartbeat signal to be selected via a solder bridge.

The board is designed to allow either the 8-pin DIL through-hole version or the SOP-8 surface mount version of the ATtiny85 chip to be used.  

This repository contains the Gerber files to allow you to order your own PCBs from suppliers such as
JLCPCB and PCBWay

## Images
![PCB Front](https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/images/Watchdog_PCB_v1.5_Front.jpg)
![Completed PCB](https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/images/Watchdog_PCB_v1.5_Back.jpg)

## Bill of Materials

### Bill of Materials for ATtiny Programmer V1.0

Description | Quantity
:--- | :---:
ATtiny85 in either 8-pin DIL or SOP-8 package | 1
8-pin DIL socket if the 8-pin DIL package is used (Optional) | 1
LED in SMT 0805 package | 1
1k resistor in SMT 0805 package | 1
10k resistor in SMT 0805 package | 1
1N5819 Schottky Diode in SOD-323 package (D1) | 1
8-pin 0.1" header sockets/pins as required (see notes) | 2


### Notes
D1 mini boards are shipped with a variety of different 0.1" pins and sockets that can be used to suit the application.
These same connectors can be used with the hardware watchdog board, to allow it to be stacked above or below the Wemos,
or plugged directly into one of the sockets on a Wemos double or triple base

### Reducing the component count
The LED and associated 1k resistor are primarily there to indicate to the user that a heartbeat signal has been received from the Wemos.
I generally tend to flash the Wemos's onboard LED when a heartbeat signal has been sent, so both LEDs should flash in unison.
The LED on the watchdog board is also used to indicate when the watchdog is in its start-up delay phase, by lighting the LED constantly during this period.
I'd recommend having this LED, and its associated 1k resistor, in place when you first start experimenting with the hardware watchdog.
However, once you've become familiar with using the hardware watchdog the LED and 1k resistor can safely be omitted if required.

## Mounting the ATtiny85 processor

These photos show the correct orientation, and correct position of Pin 1 for the two different ATtiny85 packages:

![DIL Package](https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/images/8-pin_DIL.jpg)
![SOP-8 Package](https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/images/SOP-8.jpg)

Note that **if the processor is fitted upside down** (the dot on the package doesn't orientate with the corresponding white dot on the board)
then **the voltage regulator on the Wemos board will be fried**.
It's especially easy to do this when using a socketed 8-pin DIL package and the ATtiny is re-inserted in the wrong orientation after re-flashing 
(trust me, I've done this more than once!).

## Component orientation
The LED (if fitted) and Schottky Diode need to be orientaed correctly.
The location for the LED has a "+" on the board to indicate the anode. Most LEDs are marked with a green line on the cathode (negative) end of the component.
The Schottky Diode position on the board is marked D1 and has a vertical line on the PCB, indicating the cathode side of the diode, The diode will have a 

![Schottky Diode](https://github.com/Peterkn2001/ATtiny-Programmer/blob/main/images/Schottky_Diode.jpg)

# Links to associated projects

ATtiny Programmer, to allow you to flash your code to the ATtiny85:

https://github.com/Peterkn2001/ATtiny-Programmer


Hardware watchdog in ESP32 DevKitC V4 form-factor:

(Awaiting URL)
