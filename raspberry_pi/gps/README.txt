GPS Receiver Code

I. INTRO
=========
This project is the GPS component of the system.

II. INFORMATION
===============
This code is written to interface with the Adafruit Ultimate GPS breakout with
the RaspberryPi.

The code uses the Raspberry Pi's UART. 
Connections on the Pi are:
GPS Pin "TX" to Pi Pin 10 "Rx"
GPS Pin "GND" to Pi Pin 6 "Ground"
GPS Pin "Vin" to 3.3 VDC PS.

It appears that the Raspberry Pi doesn't source enough current to power
the GPS receiver, so the receiver has been powered from seperate power supply.

III. BUILDING
=============
To build, use the make command.
To build in debug mode, uncomment the DEBUG line in the Makefile.

