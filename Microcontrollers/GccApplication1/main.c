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


const unsigned char Numbers [16] = {
	//	  dPgfe dcba
	0b00111111,		// 0
	0b00000110,		// 1
	0b01011011,		// 2
	0b01001111,		// 3
	0b01100110,		// 4
	0b01101101,		// 5
	0b01111101,		// 6
	0b00000111,		// 7
	0b01111111,		// 8
	0b01101111,		// 9
	0b01110111,		// A
	0b01111100,		// b
	0b00111001,		// C
	0b01011110,		// d
	0b01111001,		// E
	0b01110001,		// F
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
		wait(200);
		
		if(PINC & 0x01){
			number++;
			if(number >= 16){
				number = 0;
			}
			display(number);
		}
		
		if(PINC & 0x02){
			number--;
			if(number < 0){
				number = 0;
			}
			display(number);
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

