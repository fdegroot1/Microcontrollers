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
	
	DDRD = 0b10000000;			// All pins PORTD are set to output 
	
	while (1)
	{
		if(PINC & 0x01){		// Check if the button is pressed.
			PORTD = 0b10000000; // Sets the value to the 8th LED, the value is 1 only on the 8th LED which means that the LED is on.
			wait(500);			// Waits half a second.
			PORTD = 0x00;		// Sets the value to all of the LED, the value of 0 which means that all of the LEDs are off.
			wait(500);			// Waits half a second
		}
		else{					// if the button is not pressed or hold, all of the lamps get the value of 0 which means that all of the LEDs are off.
			PORTD = 0x00;
		}

	}

	return 1;
}

