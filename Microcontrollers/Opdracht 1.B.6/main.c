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
#include <stdio.h>
#include <stdlib.h>

void hz1(void);
void hz4(void);


typedef enum { C0 } ENUM_EVENTS;
typedef enum { STATE_1, STATE_2 } ENUM_STATES;


typedef struct {
	void (*finit)(void);
	void (*fbody)(void);
	void (*fexit)(void);
	ENUM_STATES nextState;
} STATE_TRANSITION_STRUCT;

STATE_TRANSITION_STRUCT fsm[2][1] = {
	{{hz1, NULL, NULL, STATE_2}},
	{{hz4, NULL, NULL, STATE_1}}
};



ENUM_STATES state = STATE_1;


void handleEvent(ENUM_EVENTS event)
/* 
short:			Change state in fsm. State change depens on event
inputs:			event, this determines the next state in the fsm
outputs:	
notes:			Needs extensive testing. !! Please review execution 
                order !!
Version :    	DMK, Initial code
*******************************************************************/
{
	// Call EXIT function old state
	if( fsm[state][event].fexit != NULL) {
		fsm[state][event].fexit() ;
	}
	
	// Set new state, -1 means 
	if(PINC & 0x01){
		state = fsm[state][event].nextState;
	}

	// Call INIT function
	if( fsm[state][event].finit != NULL) {
		fsm[state][event].finit() ;
	}

	// Call BODY function
	if( fsm[state][event].fbody != NULL) {
		fsm[state][event].fbody() ;
	}
}

void hz1(void){
	PORTD = 0b10000000;
	wait(1000);
	PORTD = 0b00000000;
	wait(1000);
}

void hz4(void){
	PORTD = 0b10000000;
	wait(250);
	PORTD = 0b00000000;
	wait(250);
}




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
		handleEvent(C0);
	}

	return 1;
}

