# 2104-Group-P2J


ICT2104-Embedded Systems Programming
## About our project
>An autonomous robot that is capable of **mapping areas** around itself, **sending the map data** to a storage location to be built into a feasible 2D map capable of tracking where and when routes happened. This idea designated Mini Robotic 2D Mapper (MR2M) was further supported by the realization that there were many diverse potential ways the product could go, these ways included but were not limited to, autonomous space mapping, and the mapping and navigation of cave systems. 
- Since the project is currently in its startup phase, our team is attempting to build an MR2M robot with minimal materials and deploying it within a safe environment.

## What you need
- MSP432 Microcontroller
- ESP8266 Wifi module 
>-  It's a wireless  [SoC](https://en.wikipedia.org/wiki/System_on_a_chip)
>-   It has GPIO, I2C, ADC, SPI, PWM and some more
>-   It's running at 80MHz
>-   64KBytes of instruction RAM
>-   96KBytes of data RAM
>-   64KBytes boot ROM
>-   The  [ESP8266 chip](https://github.com/esp8266/esp8266-wiki/wiki/Hardware_ESP8266-Versions)  is made by  [Espressif](http://espressif.com/en/products/esp8266/)
>-   [Modules](https://github.com/esp8266/esp8266-wiki/wiki/Hardware_versions)  bearing this chip are made by various manufacturers
- HC-SR04 Ultrasonic Sensor
- Motor Driver
- 2 Motor
- 2 IR Encoder

## Project setup
To use our project, you have to install [Simplelink_msp432p4_sdk_3_30_00_13](http://www.ti.com/tool/SIMPLELINK-MSP432-SDK) library in your Code Composer Studio.

## How to use
> Download or pull our source codes into your Code Composer Studio and launch the project. 
Project2104_Integrated consist of the project for the integrated ultrasonic sensor and motor, while Project2104_Wifi_Independent consists of the project for the standalone wifi.

### Important: How to run ESP8266
- Ensure the wifi module and the computer that is running the codes are on the same network
- Check wifi module’s IP address by using the ATCIFSR command in the comm. If not connected to any network, follow steps from here to connect. https://www.electronicshub.org/connect-esp8266-to-wifi/
- Run the main.c codes
- Open any browser and key in the IP address of the wifi module
- Wait for 40 seconds for the data to be displayed

