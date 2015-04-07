/*
 * Copyright 2015 Alexander Ransmann
 *
 * This is the main file for my GardenController project
 */

#include <avr/io.h>

#define F_CPU	8000000UL

#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#include "serial.h"
#include "dht11.h"
#include "adc.h"
#include "esp8266.h"

#define A0_ID		"1"
#define TEMP0_ID	"2"
#define	HUM0_ID		"3"

#define WAKEUP_TIME	225		/* WAKEUP_TIME * 8s = delay between measurements */

#define TRUE	1
#define FALSE	0

volatile int start_measurement;
volatile uint16_t watchdog_counter;

void gather_sensor_data(uint16_t *analog_data, uint8_t *temperature_data, uint8_t *humidity_data);
void send_sensor_data(uint16_t analog_data, uint8_t temperature_data, uint8_t humidity_data);
void wdt_start();

ISR(WDT_vect);

int main(void)
{	
	uint16_t analog_value_0;
	uint8_t temperature_value_0;
	uint8_t humidity_value_0;
	
	/* initialize variables */
	start_measurement = TRUE;
	watchdog_counter = 0;
	
	/* initialize system */
	serial_init(103);
	adc_init();
	dht11_init();
	esp_init();
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
	while(1)
	{
		/* always run on first startup; then after each timed wakeup */
		if(start_measurement)
		{
			/* gather sensor data */
			gather_sensor_data(&analog_value_0, &temperature_value_0, &humidity_value_0);
		
			/* send sensor data to database over TCP */
			esp_power_on();		
			send_sensor_data(analog_value_0, temperature_value_0, humidity_value_0);
			esp_power_off();
		
			/* clear flag */
			start_measurement = FALSE;
		}
		
		/* sleep until watchdog interrupt occurs */
		sleep_enable();
		wdt_start();
		sei();
		sleep_cpu();
		sleep_disable();
		cli();
	}
}

/* this function reads the actual analog value and from the DHT11 temperature and humidity */
void gather_sensor_data(uint16_t *analog_data, uint8_t *temperature_data, uint8_t *humidity_data)
{
	uint8_t dht_buffer[4];
	
	*analog_data = adc_read_analog(0);
	dht11_read_data(dht_buffer);
	*humidity_data = dht_buffer[0];
	*temperature_data = dht_buffer[2];
}

void send_sensor_data(uint16_t analog_data, uint8_t temperature_data, uint8_t humidity_data)
{
	char value_buffer[10];
	char length_buffer[10];
	int string_length;
	
	/* give module time to power up */
	_delay_ms(2000);
	
	/* connect to wlan */
	serial_puts("AT+RST\r\n");
	_delay_ms(2000);
	serial_puts("AT+CWMODE=3\r\n");
	_delay_ms(2000);
	serial_puts("AT+CWJAP=\"SSID\",\"yourpassword\"\r\n");
	_delay_ms(10000);
	
	/* send analog value over tcp */
	serial_puts("AT+CIPSTART=\"TCP\",\"www.cronj.de\",80\r\n");
	_delay_ms(500);
	utoa(analog_data, value_buffer, 10);
	string_length = strlen(value_buffer) + 66;
	itoa(string_length, length_buffer, 10);
	serial_puts("AT+CIPSEND=");
	serial_puts(length_buffer);
	serial_puts("\r\n");
	_delay_ms(100);
	serial_puts("GET /your_page.php?id=1&value=");
	serial_puts(value_buffer);
	serial_puts(" HTTP/1.1\r\nHost: www.cronj.de\r\n\r\n");
	_delay_ms(500);
	serial_puts("AT+CIPCLOSE\r\n");
	_delay_ms(500);
	
	/* send humidity value over tcp */
	serial_puts("AT+CIPSTART=\"TCP\",\"www.cronj.de\",80\r\n");
	_delay_ms(500);
	utoa(humidity_data, value_buffer, 10);
	string_length = strlen(value_buffer) + 66;
	itoa(string_length, length_buffer, 10);
	serial_puts("AT+CIPSEND=");
	serial_puts(length_buffer);
	serial_puts("\r\n");
	_delay_ms(100);
	serial_puts("GET /your_page.php?id=3&value=");
	serial_puts(value_buffer);
	serial_puts(" HTTP/1.1\r\nHost: www.cronj.de\r\n\r\n");
	_delay_ms(500);
	serial_puts("AT+CIPCLOSE\r\n");
	_delay_ms(500);
	
	/* send temperature value over tcp */
	serial_puts("AT+CIPSTART=\"TCP\",\"www.cronj.de\",80\r\n");
	_delay_ms(500);
	utoa(temperature_data, value_buffer, 10);
	string_length = strlen(value_buffer) + 66;
	itoa(string_length, length_buffer, 10);
	serial_puts("AT+CIPSEND=");
	serial_puts(length_buffer);
	serial_puts("\r\n");
	_delay_ms(100);
	serial_puts("GET /your_page.php?id=2&value=");
	serial_puts(value_buffer);
	serial_puts(" HTTP/1.1\r\nHost: www.cronj.de\r\n\r\n");
	_delay_ms(500);
	serial_puts("AT+CIPCLOSE\r\n");
	_delay_ms(500);
}

void wdt_start()
{
	wdt_reset();
	wdt_enable(WDTO_8S);

	_delay_ms(1);

	WDTCSR |= (1 << WDIE);
}

ISR(WDT_vect)
{
	wdt_disable();
	
	watchdog_counter++;
	
	if(watchdog_counter >= WAKEUP_TIME)
	{
		watchdog_counter = 0;
		start_measurement = TRUE;
	}
}
