/*
 * Copyright 2015 Alexander Ransmann
 *
 * This file reads data from the ADC
 */

#include "adc.h"

void adc_init()
{
	/* adc returns false data on first read out */
	adc_read_analog(0);
}

uint16_t adc_read_analog(uint8_t channel)
{
	/* configure ADC to use AVCC as reference */
	ADMUX = _BV(REFS0);
	
	/* set selected ADC channel */
	switch(channel)
	{
		case 1: ADMUX |= _BV(MUX0);
		break;
		case 2: ADMUX |= _BV(MUX1);
		break;
		case 3: ADMUX |= _BV(MUX1)|_BV(MUX0);
		break;
		case 4: ADMUX |= _BV(MUX2);
		break;
		case 5: ADMUX |= _BV(MUX2)|_BV(MUX0);
		break;
		case 6: ADMUX |= _BV(MUX2)|_BV(MUX1);
		break;
		case 7: ADMUX |= _BV(MUX2)|_BV(MUX1)|_BV(MUX0);
		break;
		default:
		break;
	} 
	
	/* enable ADC and start conversion */
	ADCSRA |= _BV(ADEN)|_BV(ADSC);
	
	/* wait until conversion finishes and return ADC value */
	while(bit_is_set(ADCSRA, ADSC));
	return ADC;
}
