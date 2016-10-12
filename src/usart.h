#define _FOSC 16000000
#define _BAUD 9600
#define _UBRR0 (_FOSC / 16 / _BAUD) - 1;

#define LINE_END 0x0A
#define TRUE 1
#define FALSE 0

#define BUFFER_SIZE 64

/*
 * used to implement a circular buffer
 */
typedef struct Buffer
{
	char buf[BUFFER_SIZE];
	unsigned char head;
	unsigned char tail;
} Buffer;

extern Buffer rx;
extern Buffer tx;

/*
 * initializes the uart communication
 */
void usart_init(void);

/*
 * adds a string to the tx buffer so it will be transmitted 
 */
void usart_send(unsigned char*);

/*
 * adds a character to the tx buffer so it will be transmitted 
 */
void usart_send_char(unsigned char);

/*
 * perform necessary usart maintnence (goes in loop)
 */
void usart_process();

/**
 * private function for adding a character to a buffer
 */
void _buffer_add(Buffer*, unsigned char);

/**
 * private function for getting a character from a buffer
 */
unsigned char _buffer_get_next(Buffer*);
