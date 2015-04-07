/*
 * Copyright 2015 Alexander Ransmann
 *
 * This is the header file for the DHT11 sensor code
 */

#ifndef DHT11_H_
#define DHT11_H_

#include <avr/io.h>
#include <stdint.h>

#define F_CPU	8000000UL

#include <util/delay.h>

#define DHT11_PWR	PB0
#define DHT11_DAT	PB1
#define DHT11_DDR	DDRB
#define DHT11_PORT	PORTB
#define DHT11_PIN	PINB

#define ERROR		0
#define PAR_ERROR	1
#define OK			2

void dht11_init();
uint8_t dht11_read_data(uint8_t *data);

#endif /* DHT11_H_ */