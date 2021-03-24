/*
 * Opdracht 3.B.3.c
 *
 * Created: 03/03/2021 10:36:46
 * Author : fabia
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int counter = 0;
int light_state = 0; 

ISR(TIMER2_OVF_vect){
	
	TCNT2 = -8;
	if(counter == 15 && light_state == 0){
		light_state = 1;
		counter = 0;
		PORTD = 0;
	}
	else if(counter == 25 && light_state == 1){
		light_state	= 0;
		counter = 0;
		PORTD = 1<<7;
	}
	counter++;
	
}

ISR(TIMER2_COMP_vect){
	TCNT2 = -8;
	if(counter == 15 && light_state == 0){
		light_state = 1;
		counter = 0;
		PORTD = 0;
	}
	else if(counter == 25 && light_state == 1){
		light_state	= 0;
		counter = 0;
		PORTD = 1<<7;
	}
	counter++;
	
}

void initTimer2(void){
	TCNT2 = -8;
	TIMSK |= 1<<6; //ISR overflow
	sei();
	TCCR2 = 0x05;
	
}
int main(void)
{
    /* Replace with your application code */
	DDRD = 1<<7; // set PORTD.7 for output
	PORTD = 1<<7;
	
	initTimer2();
	
	
	
		
    while (1) 
    {
		_delay_ms(1);
    }
}

