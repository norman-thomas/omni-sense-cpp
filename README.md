# omni-sense-cpp

_ESP8266 using many sensors_

This is a sensors firmware for the Wifi-capable ESP8266 controller.
Support for several environment sensors (BME280, BMP280, SI7021, TSL2561, TSL2591, PPD42NS, DHT22) is implemented, which enables the measurement of temperature, humidity, pressure, luminosity, dust and rain.
Values are periodically measured and published via MQTT.

## Prerequisites

* ESP8266 or WeMos D1 microcontroller
* micro-USB cable
* Arduino IDE
* Wifi
* MQTT broker

## Configuration

* inside `credentials.h` configure your WiFi and MQTT credentials
* inside `omni-sense.ino` (de)activate the sensors you want to support in your build by uncommenting the `INCLUDE_*` lines
* inside `omni-sense.ino` set `MQTT_PREFIX_LOCATION` and `MQTT_PREFIX_ROOM` to desirable values, they will be used for naming the published MQTT topics

## Wiring

* connect the sensor(s) to the I2C ports (SDA, SCL) of the controller
  * in case of the ESP8266 NodeMCU and Wemos D1 Mini, the sensor's SCL and SDA should be hooked up to the controller's D1 and D2 pins respectively

## Installation

With the ESP8266 connected to the computer, the firmware can be compiled and flashed onto the controller using the Arduino IDE.

Once running, the controller should periodically publish topics to the broker.
