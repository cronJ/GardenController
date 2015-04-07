# GardenController
This repository includes all files for my GardenController project.

## Hardware
I'm using an ATmega48A @8MHz connected to an ESP8266 on an ESP-01 module.
Humidity and temperature data are provided by an DHT11 sensor.
Soil moisture is measured over analog voltage.

## Software
System sleeps in power_down mode until watchdog counter allows a new measurement.
Sleeptime ist calculated by following formular: WAKEUP_TIME * 8s
Current WAKEUP_TIME is 225 which results in an sleeptime of 30 minutes.

## Possible optimizations
Usage of the power.h file to reduce power consumption in sleep mode.
Finetuning of delay times in ESP8266 transmission routine.