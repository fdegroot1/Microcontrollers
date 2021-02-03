/*
 * week 1.c
 *
 * Created: 3-2-2021 09:20:35
 * Author : Gebruiker
 */ 

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** knipper.c
**
** Beschrijving:	Toggle even en oneven leds PORTD  
** Target:			AVR mcu
** Build:			avr-gcc -std=c99 -Wall -O3 -mmcu=atmega128 -D F_CPU=8000000UL -c knipper.c
**					avr-gcc -g -mmcu=atmega128 -o knipper.elf knipper.o
**					avr-objcopy -O ihex knipper.elf knipper.hex 
**					or type 'make'
** Author: 			dkroeske@gmail.com
** -------------------------------------------------------------------------*/
#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;


PATTERN_STRUCT pattern[] = {
	{0b10000001, 100}, {0b01000010, 100}, {0b00100100, 100}, {0b00011000, 100},
	{0b00011000, 100}, {0b00100100, 100}, {0b01000010, 100}, {0b10000001, 100},
	{0b00000011, 200}, {0b00001100, 200}, {0b00110000, 200}, {0b11000000, 200},
	{0b00000000, 0}
};


/******************************************************************/
void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

/******************************************************************/
int main( void )
/* 
short:			main() loop, entry point of executable
inputs:			
outputs:	
notes:			Looping forever, flipping bits on PORTD
Version :    	DMK, Initial code
*******************************************************************/
{
	
	DDRD = 0b11111111;			// All pins PORTD are set to output 
	
	while (1)
	{
		// Set index to begin of pattern array
		int index = 0;
		// as long as delay has meaningful content
		while( pattern[index].delay != 0 ) {
			// Write data to PORTD
			PORTD = pattern[index].data;
			// wait
			wait(pattern[index].delay);
			// increment for next round
			index++;
		}
	}

	return 1;
}

