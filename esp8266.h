/*
 * Copyright 2015 Alexander Ransmann
 *
 * This is the header file for the ESP8266 code
 */

#ifndef ESP8266_H_
#define ESP8266_H_

#include <avr/io.h>

#define F_CPU 8000000UL

#include <util/delay.h>
#include <stdint.h>
#include <string.h>

#define ESP8266_DDR		DDRB
#define ESP8266_PORT	PORTB
#define ESP8266_POWER	PB2

#define ESP8266_GOT_IP			2
#define ESP8266_IS_CONNECTED	3
#define ESP8266_IS_DISCONNECTED	4

#define CWMODE_STA	"CWMODE:1"
#define CWMODE_AP	"CWMODE:2"
#define CWMODE_BOTH	"CWMODE:3"

#define ESP_READY       "ready"
#define ESP_OK          "OK"
#define ESP_ERROR       "ERROR"
#define ESP_LINK        "Linked"
#define ESP_UNLINK      "Unlink"
#define ESP_BUSY        "busy"
#define ESP_NO_CHANGE   "no change"
#define ESP_UNKNOWN	"no this fun"

void esp_init();
void esp_power_on();
void esp_power_off();

#endif /* ESP8266_H_ */