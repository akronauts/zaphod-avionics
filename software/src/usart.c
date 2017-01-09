#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// initialize global variables
Buffer rx = {.tail = 0, .head = 0};
Buffer tx = {.tail = 0, .head = 0};


/**
 * this interrupt service routine fires every time a new character (byte) is received by 
 * by rx0 
 */ 
ISR(USART0_RX_vect)
{
	// add the new character to the buffer
	unsigned char c = UDR0;
	_buffer_add(&rx, c);
}

void usart_init(void)
{
	// load the baud rate register value
	unsigned int ubrr0 = _UBRR0;

	// set the baud rate register for port 0
	UBRR0L = (unsigned char) ubrr0;
	UBRR0H = (unsigned char) (ubrr0 >> 8);

	// enable transmit, receive, and RX complete interrupt
	UCSR0B = 0x98;

	// set async USART, no parity, 1 stop bit, 8 data bits
	UCSR0C = 0x06;
}

void usart_send(unsigned char* msg)
{
	unsigned char c;
	while((c = *(msg++)) != 0x00)
	{
		// add each character to the tx buffer
		_buffer_add(&tx, c);
	}
}

void usart_send_char(unsigned char c)
{
	// add the character to the tx buffer
	_buffer_add(&tx, c);
}

void usart_process()
{
	unsigned char c;
	// transmit the next byte in the buffer, if any
	if ((c = _buffer_get_next(&tx)) != 0x00 )
	{
		// wait for the transmit buffer to be empty
        while(!(UCSR0A & 0x20));
        // send the character
        UDR0 = c;
	}

	// check for any new messages
	// save a copy of the tail so we can replace it later
	unsigned char tail = rx.tail;
	while((c = _buffer_get_next(&rx)) != 0x00)
	{
		if (c == LINE_END)
		{
			// there is a new message
			// revert the rx tail so we can grab it 
			rx.tail = tail;
			while ((c = _buffer_get_next(&rx)) != LINE_END)
			{
				// do something with the message here!
				usart_send_char(c);
			}
			usart_send_char('\n');
			return;
		}
	}
	rx.tail = tail;
}

void _buffer_add(Buffer* b, unsigned char c)
{
	if (b->head < BUFFER_SIZE)
	{
		// normal operation; add byte to buffer
		b->buf[b->head++] = c;
	}
	else
	{
		// reached buffer limit; wrap around
		b->head = 0;
		b->buf[b->head++] = c;
	}
}

unsigned char _buffer_get_next(Buffer* b)
{
	unsigned char c;
	if (b->tail < b->head)
	{
		// normal operation; get the next byte in the buffer
		c =  b->buf[b->tail++];
	}
	else if (b->tail > b->head)
	{
		// it appears the buffer has wrapped; still get the next byte
		if (b->tail < BUFFER_SIZE)
		{
			c = b->buf[b->tail++];
		}
		else
		{
			b->tail = 0;
			c = b->buf[b->tail++];
		}
	}
	else
	{
		// no bytes to get
		c = 0x00;
	}
	return c;
}
