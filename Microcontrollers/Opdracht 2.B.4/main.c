/*
 * GccApplication1.c
 *
 * Created: 10-2-2021 11:39:16
 * Author : Gebruiker
 */ 


#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


const unsigned char Numbers [12] = {
	//	  dPgfe dcba
	0b00001000,
	0b00011000,
	0b00111000,
	0b00111001,
	0b00111011,
	0b00111111,
	0b00110111,
	0b00100111,
	0b00000111,
	0b00000110,
	0b00000100,
	0b00000000,	
};


int main(void)
{
    /* Replace with your application code */
	
	int number = 0;
	DDRD = 0xFF;
	DDRC = 0x00;
	
	display(0);
	
    while (1) 
    {
		display(number);
		wait(500);
		number++;
		if(number > 11){
			number=0;
		}
    }
}

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


void display(int digit){
	//if(digit < 0 || digit > 15){
		
	//}
	//else if(digit >= 0 && digit <= 15){
		PORTD = Numbers[digit];
	//}
}

