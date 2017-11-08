# spsensor

This application requires the following hardware:

1. [Arduino Uno R3 (Atmega328 - assembled)](https://www.adafruit.com/product/50) 
1. [Adafruit Bluefruit LE Shield - Bluetooth LE for Arduino](https://www.adafruit.com/product/2746).

Some assembly is required.

After flashing the device with the new firmware the device will use the Bluetooth Low Energy module to wait for a connection. Once a connection is established the device will send level information to the remote device. The level information has a range of 0 to 100 as a function of percentage full.
