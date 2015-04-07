/*
 * Copyright 2015 Alexander Ransmann
 *
 * This file powers the ESP8266 module on or off.
 * It connects to a specific server and writes all data to it.
 */

#include "esp8266.h"
#include "serial.h"

void esp_init()
{
	/* configure power pin as output */
	ESP8266_DDR |= _BV(ESP8266_POWER);
}

void esp_power_on()
{
	/* turn power pin on */
	ESP8266_PORT |= _BV(ESP8266_POWER);
}

void esp_power_off()
{
	/* turn power pin off */
	ESP8266_PORT &= ~_BV(ESP8266_POWER);
}
