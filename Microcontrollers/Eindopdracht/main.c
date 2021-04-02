#define F_CPU 8e6

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"

#define BIT(x) ( 1<<x )
#define LEFT 2 //pin 2
#define RIGHT 1 // pin 1
#define SONAR 0 //pin 0

int step_number = 0;
enum direction_stepper_moter {CLOCKWISE, COUNTER_CLOCKWISE};
enum interrupt_status {INTERRUPT_FALLING, INTERRUPT_RISING};
	
static enum direction_stepper_moter dir_stepper_motor = CLOCKWISE;	
static enum interrupt_status int_stat = INTERRUPT_RISING;

uint16_t timer_dist = 125; // time measured by timer;

void OneStep(){
	switch(step_number){
		case 0:
		PORTE = 0b00000001;
		break;
		
		case 1:
		PORTE = 0b00000011;
		break;
		
		case 2:
		PORTE = 0b00000010;
		break;
		
		case 3:
		PORTE = 0b00000110;
		break;
		
		case 4:
		PORTE = 0b00000100;
		break;
		
		case 5:
		PORTE = 0b00001100;
		break;
		
		case 6:
		PORTE = 0b00001000;
		break;
		
		case 7:
		PORTE = 0b00001001;
		break;
		
		default:
		PORTE = 0b00000000;
		break;
	}
	
	if(dir_stepper_motor == CLOCKWISE){
		step_number++;
		} else{
		step_number--;
	}

	if(step_number > 7){
		step_number = 0;
	}
	if(step_number < 0){
		step_number = 7;
	}
	_delay_ms(1);
}



ISR(INT0_vect)
{
	
	// if the interrupt was generated on a rising edge (start sending echo)
	if (int_stat == INTERRUPT_RISING)
	{
		// set interrupt pin 0 on PORTD to falling edge
		EICRA = 0x02;
		
		// reset the time in timer1
		TCNT1 = 0x00;
		
		// set interrupt status
		int_stat = INTERRUPT_FALLING;
	} else 
	// else if it was generated on a falling edge (end sending echo)
	{
		// set interrupt pin 0 on PORTD to rising edge
		EICRA = 0x03;
		
		// read timer1 into time_dist
		timer_dist = TCNT1;
		
		// set interrupt status
		int_stat = INTERRUPT_RISING;
	}
	
}


void wait_us(unsigned int us)
{
	for(int i = 0; i < us; i++)
	{
		_delay_us(1);
	}
}

void wait_ms(unsigned int ms)
{
	
	for(int i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}
}

void ultrasonic_send_pulse()
{
	PORTG = 0x00; // 10 us low pulse
	wait_us(10);
	PORTG = 0x01;
}



void ultrasonic_measurement(){
	char snum[5];
	
	ultrasonic_send_pulse();
	int distance = timer_dist * (340 / 2);
	lcd_clear();
	itoa(distance, snum, 10);;
	lcd_write_string(snum);
	
	wait_ms(200);
}

void write_int_lcd(int number){
	int length = snprintf(NULL, 0, "%d", number);
	char str[length + 1];
	snprintf(str, length + 1, "%d", number);
	lcd_write_string(str);
}

int main(void)
{
	DDRG = 0xFF; // port g all output. pin 0 is trig, the rest is for debug
	DDRD = 0x00; // port D pin 0 on input. 0 is echo and also interrupt
	
	EICRA = 0x03; // interrupt PORTD on pin 0, rising edge
	
	EIMSK |= 0x01; // enable interrupt on pin 0 (INT0)
	
	TCCR1A = 0b00000000; // initialize timer1, prescaler=256
	TCCR1B = 0b00001100; // CTC compare A, RUN
	
	sei(); // turn on interrupt system
	
	init_4bits_mode();
	_delay_ms(10);
	lcd_clear();

	for(int i = 0; i < 4096; i++){
		OneStep();
		_delay_ms(1);
	}

	int isSonarActive = 0;
    while (1) 
    {
		if(PINF & BIT(SONAR)){
			if(isSonarActive){
				//TODO deactivate sonar
				isSonarActive = 0;
			}
			else{
				//TODO activate sonar
				isSonarActive = 1;
			}
		}
		
		if(PINF & BIT(LEFT)){
			//TODO rotate to left
			OneStep();
			_delay_ms(1);
		}
		
		if(PINF & BIT(RIGHT)){
			//TODO rotate to right
			OneStep();
			_delay_ms(1);
		}
		
		if(isSonarActive){
			ultrasonic_measurement();
		}
		else{
			lcd_clear();
			lcd_write_string("Sonar is off");
		}
		_delay_ms(100);
		
    }
}


