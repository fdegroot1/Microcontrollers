/*
 * Project name:
     Demo4_2 : D conversion on channel 1, PORTF.1, single mode
 * Author: Avans-TI, JW
 * Revision History: 
     20101229: - initial release;
 * Description:
     This program gives an interrupt on each ms
 * Test configuration:
     MCU:             ATmega128
     Dev.Board:       BIGAVR6
     Oscillator:      External Clock 08.0000 MHz
     Ext. Modules:    -
     SW:              AVR-GCC
 * NOTES:
     - Turn ON the PORT LEDs at SW12.1 - SW12.8
*/

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

#define BIT(x)	(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}


// Initialize ADC: 
void adcInit( void )
{
	ADMUX = 0b11100001;			// AREF=2,56 V, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// 
}


char * toString(int number) {
	int length = snprintf(NULL, 0, "%d", number + 1);
	char *str = malloc(length + 1);
	snprintf(str, length + 1, "%d", number + 1);
	return str;
}

void lcdInit(){
	init_4bits_mode();
	wait(10);
	lcd_clear();
	set_cursor(1);
}

// Main program: Counting on T1
int main( void )
{
	DDRF = 0x00;					// set PORTF for input (ADC)
	DDRB = 0xFF;					// set PORTA for output 
	adcInit();
							// initialize ADC
	init_4bits_mode();
	_delay_ms(10);
	lcd_clear();
	_delay_ms(10);	

	
	while (1)
	{
		//ADCSRA |= BIT(6);				// Start ADC
		//while ( ADCSRA & BIT(6) ) ;		// Wait for completion
		//PORTC = ADCL;	
		PORTB = ADCH;					// Show MSB (bit 9:2) of ADC
		lcd_clear();
		_delay_ms(10);
		lcd_write_string(toString((ADCH<<1)));
		wait(500);						// every 50 ms (busy waiting)
	}
}
