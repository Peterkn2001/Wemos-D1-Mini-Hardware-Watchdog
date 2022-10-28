# Wemos D1 Mini Hardware Watchdog
An ATtiny85 based hardware watchdog in the Wemos D1 Mini/Pro form factor, able to be used as a shield for the Wemos boards. 

## Overview
This watchdog timer listens for a heartbeat signal from the Wemos D1 Mini/Pro and if no signal has been recieved during the 
pre-defined period the Wemos will be rebooted (along with the watchdig timer).
The ATtiny firmware allows for separate startup and heartbeet delays to be defined and, in conjunction with the custom PCB, allows
these to be adjusted via solder bridges to cater for differnet applications without the need to re-flash the ATtiny firmware.

The custom circuit board also allows the pin that is used to listen for the hearbeat signal to be selected via a solder bridge.

The board is designed to alow either the 8-pin DIL through-hole version or the SOP-8 surface mount version of the ATtiny85 chip to be used.  

This repository contains the Gerber files to allow you to order your own PCBs from suppliers such as
JLCPCB and PCBWay

## Images
![PCB Front](https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/images/Watchdog_PCB_v1.5_Front.jpg)


## Bill of Materials

### Bill of Materials for ATTiny Programmer V1.0

Description | Quantity
:--- | :---:
ATtiny85 in either 8-pin DIL or SOP-8 package | 1
8-pin DIL socket if the 8-pin DIL package is used (Optional) | 1
LED in SMT 0805 package | 1
1k resistor in SMT 0805 package | 1
10k resistor in SMT 0805 package | 1
1N5819 Schotty Diode in SOD-323 package (D1) | 1
8-pin 0.1" header sockets/pins as required (see notes) | 2


### Notes
D1 mini boards are shipped with a varietry of differnet 0.1" pins and sockets that can be used to suit the application.
These same connectors can be used with the hardware watchdog board, to allow it to be stacked above or below the Wemos,
or plugged directly into one of the sockets on a Wemos double or triple base

### Reducing the component count
The LED and associated 1k resistor are primarily there to indicate to the user that a hearbeat signal has been recieved from the Wemos.
I generally tend to flash the Wemos's onboard LED when a heartbeat signal has been sent, so both LEDs shoid flash in unison.
The LED on the watchdog board is also used to indicate when the watchdog is in its startup delay phase, bu lighting the LED cionstantky during this period.
I'd reccomend having this LED, and its assciated 1k resistor, in place when you first start experimenting with the hardware watchdog.
However, once you've become familiar with using the hardware watchdog the LED and 1k resistor can safely be omitted if required.

## Mounting the ATtiny85 processor

These photos show the correct orientaion, and correct position of Pin 1 for the two differnet ATtiny85 packages:

![DIL](https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/images/8-pin_DIL.jpg)
![SOP-8 Front](https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/images/SOP-8.jpg)

Note that **if the processor is fitted upside down** (the dot on the package doesn't orientate with the corresponding white dot on the board)
then **the voltage regulator on the Wemos board will be fried**.
It's especially easy to do ths when uisng a socketed 8-pin DIL package and the ATtiny is re-inserted in the wrong orientation after re-flashing
(trust me, I've done this more than once!).



# Links to associated projects

ATtiny Programmer, to allow you to flash your code to the ATtiny85:

https://github.com/Peterkn2001/ATtiny-Programmer


Hardware watchdog in ESP32 DevKitC V4 form-factor:

(Awaiting URL)
