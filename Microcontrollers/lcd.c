#include "lcd.h"

void init(){
	// PORTC output mode and all low (also E and RS pin)
	DDRD = 0xFF;
	DDRA = 0xFF;
	PORTC = 0x00;
	PORTA = 0x00;
	//PORTA = 0xFF;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	
	PORTA |= (1<<6);	// E high
	_delay_ms(1);			// nodig
	PORTA &= ~(1<<6);  	// E low
	_delay_ms(1);			// nodig?

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	
	PORTA |= (1<<6);	// E high
	_delay_ms(1);			// nodig
	PORTA &= ~(1<<6);  	// E low
	_delay_ms(1);			// nodig?
	
	PORTC = 0x80;
	
	PORTA |= (1<<6);	// E high
	_delay_ms(1);			// nodig
	PORTA &= ~(1<<6);  	// E low
	_delay_ms(1);			// nodig?
	

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	
	PORTA |= (1<< 6);	// E high
	_delay_ms(1);			// nodig
	PORTA &= ~(1<<6);  	// E low
	_delay_ms(1);			// nodig?
	
	PORTC = 0xF0;
	
	PORTA |= (1<<6);	// E high
	_delay_ms(1);			// nodig
	PORTA &= ~(1<<6);  	// E low
	_delay_ms(1);			// nodig?

	// Step 4 (table 12)
	PORTC = 0x00;   // Entry mode set
	
	PORTA |= (1<<6);	// E high
	_delay_ms(1);			// nodig
	PORTA &= ~(1<<6);  	// E low
	_delay_ms(1);			// nodig?
	
	PORTC = 0x60;
	
	PORTA |= (1<<6);	// E high
	_delay_ms(1);			// nodig
	PORTA &= ~(1<<6);  	// E low
	_delay_ms(1);			// nodig?
	
}

void display_text(char *str){
	for(;*str; str++){
		// First nibble.
		PORTC = *str;
		PORTA |= (1<<4);
		
		PORTA |= (1<<6);	// E high
		_delay_ms(1);			// nodig
		PORTA &= ~(1<<6);  	// E low
		_delay_ms(1);			// nodig?

		// Second nibble
		PORTC = (*str<<4);
		PORTA |= (1<<4);
		
		PORTA |= (1<<6);	// E high
		_delay_ms(1);			// nodig
		PORTA &= ~(1<<6);  	// E low
		_delay_ms(1);			// nodig?
		
	}
}

void set_cursor(int position){
	
}