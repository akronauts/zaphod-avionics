#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include <avr/interrupt.h>
#include "interrupt.h"

ISR(TIMER0_COMPA_vect)
{
	//usart_transmit_flush("BLINK\n");

	// toggle the LED
	// PORTB = PORTB ^ 0x01; 
}


int main(void)
{
	// initialize the usart communication
	usart_init();		

	// set port B as output
	DDRB = 0xFF; 
	
	// init our timer interrupt
	interrupt_init(); 

	// enable global interrupts 
	sei();
	
	while(1 == 1)
	{
		usart_process(); 
	}
}
