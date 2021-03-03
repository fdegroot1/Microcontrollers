/*
 * Opdracht 3.B.2.c
 *
 * Created: 24/02/2021 09:22:57
 * Author : fabia
 */ 

#define F_CPU 16e6

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

volatile int tenthvalue = 0;
volatile int preset = -10;
volatile int update = 0;
char str[12];



//Interrupt routine timer compare match
ISR( TIMER2_COMP_vect ) {
	TCNT2 = preset; // Preset value
	tenthvalue++; // Increment counter
	update++;
}
// Initialize timer2
void timer2Init( void ) {
	TCNT2 = preset;
	TIMSK |= 1<<6;
	sei();
	TCCR2 = 0b00000111;
}

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

// Main program: Counting on T2
int main( void ) {
	init_4bits_mode();
	DDRD &= ~1<<7; // set PORTD.7 for input
	DDRB = 0xFF; // set PORTC for output (shows tenth value)
	
	timer2Init();
	
	lcd_clear();
	set_cursor(1);
	lcd_write_string("2");
	while (1) {
		PORTA = TCNT2; // show value counter 2
		
		if(update){
			lcd_clear();
			wait(10);
			set_cursor(1);
			wait(10);
			sprintf(str,"%d",tenthvalue);
			lcd_write_string("31");
			update--;
		}
		wait(10); // every 10 ms
	}
}

