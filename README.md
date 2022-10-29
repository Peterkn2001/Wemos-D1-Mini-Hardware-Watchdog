# Wemos D1 Mini Hardware Watchdog
An ATtiny85 based hardware watchdog in the Wemos D1 Mini/Pro form factor, able to be used as a shield for the Wemos boards. 

## Overview
This watchdog timer listens for a heartbeat signal from the Wemos D1 Mini/Pro and if no signal has been recieved during the 
pre-defined period the Wemos will be rebooted (along with the watchdog timer).
The ATtiny firmware allows for separate start-up and heartbeat delays to be defined and, in conjunction with the custom PCB, allows
these to be adjusted via solder bridges to cater for different applications without the need to re-flash the ATtiny firmware.

The custom circuit board also allows the pin that is used to listen for the heartbeat signal to be selected via a solder bridge.

All solder bridges are located on the top of the board, to allow it to be soldered in place on another PCB and still allow the options to be changed.

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

## Orientation of other components
The LED (if fitted) and Schottky Diode need to be orientaed correctly.
The location for the LED has a "+" on the board to indicate the anode. Most LEDs are marked with a green line on the cathode (negative) end of the component.

The Schottky Diode position on the board is marked D1 and has a vertical line on the PCB, indicating the cathode side of the diode, The diode will also have a vertical line on one side which also indicates the cathode, like in the image below (the code printed on your diode may be differnet)...

![Schottky Diode](https://github.com/Peterkn2001/ATtiny-Programmer/blob/main/images/Schottky_Diode.jpg)

# The ATtiny Firmware
The firmware for the ATtiny85 is here:

https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/ATtiny-Firmware/Hardware_Watchdog_v1_5.ino

The firmware allows two values to be defined for both the initial (startup) delay and the hearbeat timeout.

When the device first boots, the onboard LED of the hardware watchdog (if fitted) will be lit, and will go off when the intial delay has ended.
The hardware watchdog then listens for a regular hearbeat signal from the Wemos to confirm that the Wemos is still operating correctly.
If no heartbeat signal is recieved before the heartbeat timeout delay period ends then the device will reboot the Wemos (and the hardware watchdog).

The initial startup delay is to allow the Wemos to perform all of the void startup actions (initialising sensors, connecting to WiFi etc)
before the void loop executes. The void loop needs to trigger a heartbeat signal to the selected GPIO pin (preferably using a non-blocking timer such as SimpleTimer)
to prevent the hardware watchdog from rebooting the Wemos and staring the process over again.

The first reboot would occur once both the inital startup delay and the heartbeat delay have expired.

To avoid having to re-flash the ATtiny86 processor with differnet firmware for different situations, the firmware allows two differnet starup and heratbeat
intervals to be defined, which can be selected by applying a solder bridge to the relevant pads on the PCB. This is all explained in the commenst of the ATtiny code.

You will need additional hardware to be able to flash the AYtiny85 processor.
Some of the hardware sold for this purpose on the internet isn't actually suitable for this task, and the best solutions use an Arduino Uno or Nano.
As wiring-up and programming an Arduino for thios task each time you want to re-flash an ATtiny is messy and time-consuming I designed and built my own solution.
See the "Links to associated projects" section below for further details.

**Note that it is important to selct the correct processor type and clock frequency (8 MHz), and to burn the bootloader before uploading the firmware.
If the initial startup and heratbeat timing is much longer or shorter than expected then the wrong clock frequency has probably been used.**

# Selecting the Heartbeat Pin

Only one of the five pairs of heartbeat solder pads should be bridged
(but if no pads are bridged then the the watchdog will constantly reboot once the startub and heartbead delay periods have passed)

![Hearbeat_Solder_Pads](https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog/blob/main/images/Heartbeat_solder_pads.jpg)

No changes to the ATTiny firmware are needed when selecting the heartbeat pin, it is done solely via the solder pads.

# Test sketch for the Wemos D1 Mini/Pro

The sketch located here:


illustrates how to use simpleTimer to call a function which sends a heartbeat to the hardware watchdog.

The following sketch - which should only be used for testing purposes - increases the delay between heatbeats so that the hardware watchdog will
eventually force a reset:



# Using Arduino OTA with the hardware watchdog
If Arduino OTA is used to perform Over The Air updates to a Wemos D1 Mini/Pro that uses the hardware watchdog then there is the
potential for the hardware watchdog to reboot the Wemos during an OTA update, because the Wemos has stopped sending hearbeat pulses to the watchdog
while it is performing the OTA process. This would most likley result in the Wemos requiring the code to be re-flashed via the physical USB port.

There is a simple solution to this, which is to force the OTA process to send a heartbeat pulse to the hardware watchdog whne the OTA process begins,
the to continue to feed the SimpleTimer object during the OTA process, by executing 'timer.run();` throughout the process.

This is achieved by adding these lines of code to `void ssetup()` :

```cpp
  ArduinoOTA.onStart([]() {
  send_heartbeat(); 
  }); //Send a heartneat pulse when the OTA update process starts, to avoid a timeout
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r", (progress / (total / 100))); timer.run();
  }); // Run timer during the OTA process to avoid heartbeat timeout
```  




# Links to associated projects

ATtiny Programmer, to allow you to flash your code to the ATtiny85:

https://github.com/Peterkn2001/ATtiny-Programmer


Hardware watchdog in ESP32 DevKitC V4 form-factor:

(Awaiting URL)
