/*
 * Copyright 2015 Alexander Ransmann
 *
 * This file reads data from an DHT11 humidity and temperature sensor.
 * Timer data is saved before assigning to levels. This leads to no measurement errors.
 */

#include "dht11.h"
#include "serial.h"

void dht11_init()
{
	/* configure DHT11 power pin to output and data pin to input */
	DHT11_DDR |= _BV(DHT11_PWR);
	DHT11_DDR &= ~_BV(DHT11_DAT);
}

uint8_t dht11_read_data(uint8_t *data)
{	
	uint8_t temp [40];
	uint8_t calc[5];
	uint8_t counter, parity;
	
	/* configure Timer0 to 1us period (8MHz / 8 = 1MHz -> 1us) */
	TCCR0B = _BV(CS01);
	
	/*
	turn on sensor power and wait for 1s.
	according datasheet the sensor needs this time for reaching a stable state
	*/
	DHT11_PORT |= _BV(DHT11_PWR);
	_delay_ms(1000);
	
	/* set data pin to output and set level to low for minimum 18ms */
	DHT11_DDR |= _BV(DHT11_DAT);
	DHT11_PORT &= ~_BV(DHT11_DAT);
	_delay_ms(20);
	
	/* set data pin high and switch to input mode */
	DHT11_PORT |= _BV(DHT11_DAT);
	DHT11_DDR &= ~_BV(DHT11_DAT);
	
	/* wait for sensor response (20-40us) or return ERROR */
	TCNT0 = 0;
	while(bit_is_set(DHT11_PIN, DHT11_DAT))
	{
		if(TCNT0 >= 50) return ERROR;
	}
	
	/* sensor pulls data pin 80us low */
	TCNT0 = 0;
	while(bit_is_clear(DHT11_PIN, DHT11_DAT))
	{
		if(TCNT0 >= 100) return ERROR;
	}
	
	/* sensor pulls data pin 80us high */
	TCNT0 = 0;
	while(bit_is_set(DHT11_PIN, DHT11_DAT))
	{
		if(TCNT0 >= 100) return ERROR;
	}
	
	/* now read 40 bit of data */
	for(uint8_t i = 0; i < 40; i++)
	{
		/* each bit is represented by a 50us low time... */
		TCNT0 = 0;
		while(bit_is_clear(DHT11_PIN, DHT11_DAT))
		{
			if(TCNT0 >= 70) return ERROR;
		}
		
		/* ...and 26-28us high time for '0' or 70us high time for '1' */
		TCNT0 = 0;
		while(bit_is_set(DHT11_PIN, DHT11_DAT))
		{
			if(TCNT0 >= 90) return ERROR;
		}
			
		temp[i] = TCNT0;
	}
	
	/* now calculate bit level from measured time */
	counter = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 7; j >= 0; j--)
		{
			/* if timer value equals low level add '0' */
			if(temp[counter] >= 20 && temp[counter] <= 35)
			{
				calc[i] &= ~_BV(j);
			}
			/* if timer value equals high level add '1' */
			else if(temp[counter] >= 60 && temp[counter] <= 80)
			{
				calc[i] |= _BV(j);
			}
			else return ERROR;
			
			counter++;
		}
	}
	
	/* check parity by adding data bytes */
	parity = calc[0] + calc[1] + calc[2] + calc[3];
	
	if(parity != calc[4])
	{
		return PAR_ERROR;
	}
	
	/* write sensor data to given array */ 
	for(int i = 0; i < 4; i++)
	{
		data[i] = calc[i];
	}
	
	return OK;
}