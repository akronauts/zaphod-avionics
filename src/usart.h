#define _FOSC 16000000
#define _BAUD 9600
#define _UBRR0 (_FOSC / 16 / _BAUD) - 1;  

#define LINE_END 0x0A
#define TRUE 1
#define FALSE 0

/*
 * USART variables
 * Note that, by design, both buffers are of length 256 and the indices (unsigned chars) will 
 * automatically wrap. 
 */ 
unsigned char rx_buffer[256]; 
unsigned char tx_buffer[256]; 
extern unsigned char rxw_index; 
extern unsigned char rxr_index; 
extern unsigned char txw_index; 
extern unsigned char txs_index; 

/*
 * initializes the uart communication
 */
void usart_init(void); 

/*
 * transmits an entire string and flushes immediately
 */ 
void usart_send_flush(unsigned char*);

/*
 * transmits a character and flushes immediately
 */ 
void usart_send_char_flush(unsigned char); 

/*
 * transmits a string without flushing immediately
 */
void usart_send(unsigned char*); 

/*
 * transmits a character without flushing immediately 
 */ 
void usart_send_char(unsigned char); 

/*
 * perform necessary usart maintnence (goes in loop)
 */ 
void usart_process(); 
