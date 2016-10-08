#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// initialize global variables
unsigned char rxw_index = 0; // index of the last byte written to the rx buffer
unsigned char rxr_index = 0; // index of the last byte read from the rx buffer
unsigned char txw_index = 0; // index of the last byte written to the buffer
unsigned char txs_index = 0; // index of the byte waiting to be sent    

unsigned char is_rxw_wrapped = FALSE; 

ISR(USART_RX_vect)
{
	if(rxw_index == 0xFF)
	{
		is_rxw_wrapped = TRUE; 
	}
 	
	// add the new character to the buffer
	rx_buffer[rxw_index++] = UDR0; 
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


void usart_send_flush(unsigned char* msg)
{
	unsigned char c;
	while ((c = *(msg++)) != 0x00)
	{
		// wait for the transmit buffer to empty
		while (!(UCSR0A & 0x20));
		// send the current character
		UDR0 = c; 	
	}
}

void usart_send_char_flush(unsigned char c)
{
	// wait for the transmit buffer to be empty
	while(!(UCSR0A & 0x20));
	// send the character
	UDR0 = c;  
}

void usart_send(unsigned char* msg)
{
	unsigned char c; 
	while((c = *(msg++)) != 0x00)
	{
		tx_buffer[txw_index++] = c; 
	}
}

void usart_send_char(unsigned char c)
{
	tx_buffer[txw_index++] = c; 
}

void usart_process()
{
	// transmit the next char in the buffer, if any
	if (txs_index < txw_index)
	{
		usart_send_char_flush(tx_buffer[txs_index++]);
	}

	// check for any new messages
	unsigned char i = rxr_index;
	unsigned char c; 
	while(i++ < rxw_index || (is_rxw_wrapped == TRUE))
	{
		is_rxw_wrapped = FALSE; 
		if ((c = rx_buffer[i]) == LINE_END)
		{
			// there is a new message
			// send it back
			while (rxr_index < i)
			{
				usart_send_char(rx_buffer[rxr_index++]);
			}  
		}
	}
}
