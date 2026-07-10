# Rocketry-Template

[![Build](https://github.com/saddleback-college-rocketry-team/Rocketry-Template/actions/workflows/native.yml/badge.svg?branch=main)](https://github.com/saddleback-college-rocketry-team/Rocketry-Template/actions/workflows/native.yml)
[![Docs](https://img.shields.io/badge/docs-doxygen-blue)](https://saddleback-college-rocketry-team.github.io/Rocketry-Template/)

## Install:
1. [VSCode](https://code.visualstudio.com/Download)
2. [PlatformIO extension for VSCode](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

Optional not needed
1. [Docker comment generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen)
--- 

## About:

Template to help speed up the coding process. The template is defaulted to work for a Teensy 4.1 but can be used to work for any microcontroller, it will just need modifications. 

The layout which does the following:

1. Read data from sensor
2. Save data to an SD card in the form of a CSV file
3. Transmit

---

To alter the template to work with a different microcontroller do the following:

1. Go to the file: ```platformio.ini```
2. Modify the following snippet to be the name of the controller you're using:

```
# DEFAULT: WHICH TO RUN - RUNS TEENSY ENV
[platformio]
default_envs = teensy41

# TEENSY - mimic teensy environment as if its in arduino ide
# replace teensy with whatever micrcontroller you're using
[env:teensy41]
platform = teensy
board = teensy41
```
---

## Helpful Libraries / Info:
  ### Millis:
https://docs.arduino.cc/language-reference/en/functions/time/millis/
 
  ### Delay:
https://docs.arduino.cc/language-reference/en/functions/time/delay/
 
  ### Serials and baud rates:
BAUD: https://docs.arduino.cc/language-reference/en/functions/communication/serial/begin/

UART: https://docs.arduino.cc/language-reference/en/functions/communication/serial/

I2C:  https://docs.arduino.cc/language-reference/en/functions/communication/wire/
  
  ### Teensy specific serials / pins:
UART: https://www.pjrc.com/teensy/td_uart.html

I2C:  https://www.pjrc.com/teensy/td_libs_Wire.html
 
  ### Transmitting / Recieving:
https://docs.arduino.cc/learn/communication/uart/
 
  ### GPS:
https://www.pjrc.com/teensy/td_libs_TinyGPS.html