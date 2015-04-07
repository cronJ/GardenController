/*
 * Copyright 2015 Alexander Ransmann
 *
 * This is the header file for the ADC code
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>

#define ADC_DDR		DDRC
#define ADC_PORT	PORTC
#define ADC_PIN		PC0

void adc_init();
uint16_t adc_read_analog(uint8_t channel);

#endif /* ADC_H_ */