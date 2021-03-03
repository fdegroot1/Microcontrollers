/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ioisr.c
**
** Beschrijving:	BigAVR LCD module
** Target:			AVR mcu
** Build:			avr-gcc -std=c99 -Wall -O3 -mmcu=atmega128 -D F_CPU=8000000UL -c lcd.c
**					avr-gcc -g -mmcu=atmega128 -o lcd.elf lcd.o
**					avr-objcopy -O ihex lcd.elf lcd.hex
**					or type 'make'
** Author: 			dkroeske@gmail.com
** -------------------------------------------------------------------------*/

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ioisr.c
**
** Beschrijving:	BigAVR LCD module
** Target:			AVR mcu
** Build:			avr-gcc -std=c99 -Wall -O3 -mmcu=atmega128 -D F_CPU=8000000UL -c lcd.c
**					avr-gcc -g -mmcu=atmega128 -o lcd.elf lcd.o
**					avr-objcopy -O ihex lcd.elf lcd.hex
**					or type 'make'
** Author: 			dkroeske@gmail.com
** -------------------------------------------------------------------------*/

#include "lcd.h"

volatile int tenthvalue = 0;
volatile int preset = -10;
volatile int update = 0;
char str[12];


/******************************************************************
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:
notes:			Busy wait, not very accurate. Make sure (external)
clock value is set. This is used by _delay_ms inside
util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}


/******************************************************************
short:			main() loop, entry point of executable
inputs:
outputs:
notes:			Slow background task after init ISR
Version :    	DMK, Initial code
*******************************************************************/

ISR( TIMER2_OVF_vect ) {
	TCNT2 = preset; // Preset value
	tenthvalue++; // Increment counter
	update++;
}

ISR( TIMER2_COMP_vect ) {
	TCNT2 = preset; // Preset value
	tenthvalue++; // Increment counter
	update++;
}


// Initialize timer2
void timer2Init( void ) {
	TCNT2 = preset;
	TIMSK |= 1<<6; //Overflow interrupt
	//TIMSK |= 1<<7; //Compare interrupt
	
	sei();
	TCCR2 = 0b00000111;
}

//char * toString(int number) {
	//char str[17];
	//int length = snprintf(NULL, 0, "%d", number + 1);
	////char *str = malloc(length + 1);
	//snprintf(str, length + 1, "%d", number + 1);
	//return str;
//}


int main( void ) {
	
	
	
	// Init LCD
	init_4bits_mode();
	
	// Init I/O
	DDRB = 0xFF;			
	//DDRE = 0xFF;
	//DDRD &= ~1<<7; // set PORTD.7 for input
	
	wait(100);
	
	
	lcd_clear();
	
	wait(10);

	// Write sample string
	
	
	lcd_write_string("Hello world!");
	

	
	
	wait(10);
	
	//set_cursor(1);
	timer2Init();

	// Loop forever
	
	while (1) {
		PORTB = TCNT2;
		PORTE = tenthvalue;
		
		lcd_clear();
		char str[17];
		snprintf(str,17,"%d",TCNT2);
		
		lcd_write_string(str);
		wait(500);
	}

	return 1;
}
