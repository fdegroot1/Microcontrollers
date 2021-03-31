#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"

#define BIT(x) ( 1<<x )
#define DDR_SPI DDRB // spi Data direction register
#define PORT_SPI PORTB // spi Output register
#define SPI_SCK 1 // PB1: spi Pin System Clock
#define SPI_MOSI 2 // PB2: spi Pin MOSI
#define SPI_MISO 3 // PB3: spi Pin MISO
#define SPI_SS 0 // PB0: spi Pin Slave Select

enum interrupt_status {INTERRUPT_FALLING, INTERRUPT_RISING};
	
static enum interrupt_status int_stat = INTERRUPT_RISING;

uint16_t timer_dist = 125; // time measured by timer;


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


void spi_masterInit(void)
{
	DDR_SPI = 0xff; // All pins output: MOSI, SCK, SS, SS_display
	DDR_SPI &= ~BIT(SPI_MISO); // except: MISO input
	PORT_SPI |= BIT(SPI_SS); // SS_ADC == 1: deselect slave
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); // or: SPCR = 0b11010010;
	// Enable spi, MasterMode, Clock rate fck/64
	// bitrate=125kHz, Mode = 0: CPOL=0, CPPH=0
}
// Write a byte from master to slave
void spi_write( unsigned char data )
{
	SPDR = data; // Load byte --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
}
// Write a byte from master to slave and read a byte from slave - not used here
char spi_writeRead( unsigned char data )
{
	SPDR = data; // Load byte --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
	data = SPDR; // New received data (eventually, MISO) in SPDR
	return data; // Return received byte
}
// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}
// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{
	PORTB |= BIT(chipNumber);
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






int main(void)
{
	DDRG = 0xFF; // port g all output. pin 0 is trig, the rest is for debug
	DDRD = 0x00; // port D pin 0 on input. 0 is echo and also interrupt
	
	EICRA = 0x03; // interrupt PORTD on pin 0, rising edge
	
	EIMSK |= 0x01; // enable interrupt on pin 0 (INT0)
	
	TCCR1A = 0b00000000; // initialize timer1, prescaler=256
	TCCR1B = 0b00001100; // CTC compare A, RUN
	
	sei(); // turn on interrupt system
	
	
	spi_masterInit();
	_delay_ms(50);
	
	init_4bits_mode();
	_delay_ms(10);
	lcd_clear();


    while (1) 
    {
		
    }
}



