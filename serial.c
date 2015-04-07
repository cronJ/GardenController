/*
 * Copyright 2015 Alexander Ransmann
 *
 * This file implements simple UART functionality for Atmel ATMega
 */ 

#include "serial.h"

void serial_init(uint8_t baud)
{
	/* set baud rate */
	UBRR0 = baud;
	/* double uart speed */
	UCSR0A = _BV(U2X0);
	/* enable receiver and transmitter */
	UCSR0B = _BV(RXEN0)|_BV(TXEN0);
	/* set frame format: 8data, 1stop bit */
	UCSR0C = _BV(UCSZ01)|_BV(UCSZ00);
}

void serial_putc(char c)
{
	/* wait for empty transmit buffer */
	while(bit_is_clear(UCSR0A, UDRE0));
	
	/* put data into buffer, sends the data */
	UDR0 = c;
}

char serial_getc(void)
{
	/* wait for data to be received */
	while(bit_is_clear(UCSR0A, RXC0));
	
	/* get and return received data from buffer */
	return UDR0;
}

void serial_puts(char *s)
{
	for (uint8_t i = 0; i < strlen(s); i++)
	{
		serial_putc(s[i]);
	}
}
