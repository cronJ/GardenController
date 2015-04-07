/*
 * Copyright 2015 Alexander Ransmann
 *
 * This is the header file for the UART code
 */ 

#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>

#define F_CPU	8000000UL

#include <util/delay.h>
#include <stdint.h>
#include <string.h>

void serial_init(uint8_t baud);
void serial_putc(char c);
char serial_getc(void);
void serial_puts(char *s);

#endif /* SERIAL_H_ */