#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include <avr/interrupt.h>

int main(void)
{
	// initialize the usart communication
	usart_init();

	// enable global interrupts
	sei();

	while(1 == 1)
	{
		usart_process();
	}
}
