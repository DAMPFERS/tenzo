#include <avr/interrupt.h>
#include <avr/io.h>

#define ASYNCHRONOUS_NORMAL_MODE 16
#define ASYNCHRONOUS_DOUBLE_SPEED_MODE 8
#define SYNCHRONOUS_MASTER_MODE 2

#define HEX 16
#define DEC 10
#define OCT 8
#define BIN 2

// Буфер для приема данных
#define RX_BUFFER_SIZE 4
extern volatile char rx_buffer[RX_BUFFER_SIZE];
extern volatile uint8_t rx_head;
extern volatile uint8_t rx_tail;


void UART_Init(unsigned long baud = 9600, unsigned char OPERATING_MODE = ASYNCHRONOUS_NORMAL_MODE);
void UART_SendByte(unsigned char data);
void UART_SendString(const char* str, char end = '\0');
void UART_SendUInt(unsigned long value, char base = DEC);
void UART_SendSInt(unsigned long value, char base = DEC);
void UART_SendFloat(unsigned long value, char base = DEC);

unsigned char UART_ReceiveByte(void);
void UART_ReceiveString(char* str, unsigned char max_lenght);
void UART_ReceiveInt(char* str, unsigned char max_lenght);
void UART_ReceiveFloat(char* str, unsigned char max_lenght);


void intInStrTranslate(unsigned long value, char* str, char base = DEC);
void StrInIntTranslate(unsigned long value, char* str, char base = DEC);

unsigned char USART_Read(unsigned char *data);

