#include "interrupt.h"
#include <avr/interrupt.h>

void interrupt_init(void)
{
	// enable compare interrupts for timer0
	TIMSK0 = 0x02;
	// set timer0 to a value of 0
	TCNT0 = 0x00;
	// specify normal counter operation (no PWM or external interrupt)
	TCCR0A = 0x00;
	// prescaler of 64
	TCCR0B = 0x03; 
	// set compare register
	OCR0A = 0xFF;   
}
