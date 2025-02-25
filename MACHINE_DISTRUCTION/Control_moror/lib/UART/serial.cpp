#include <serial.h>
#include <motor_control_RVV.h>



volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;


void UART_Init(unsigned long baud, unsigned char OPERATING_MODE){
    uint16_t ubrl_value = (F_CPU/(baud * OPERATING_MODE))-1;
    UBRR0L = (unsigned char) ubrl_value;
    UBRR0H = (unsigned char)(ubrl_value >> 8);
    
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // Enable reciever and transmitter | 1 << RXEN0;
    //UCSR0C = 1 << USBS0 | 3 << UCSZ00; // Set frame format: 8data, 2stop bit
    UCSR0C |= 1 << UCSZ00 | 1 << UCSZ01; // Set frame format: 8data
    UCSR0C &= ~(1 << USBS0);             // 2stop bit
    if(OPERATING_MODE == SYNCHRONOUS_MASTER_MODE)
      UCSR0C |= 1 << UMSEL00;
    return;
  }
  
  
void UART_SendByte(unsigned char data){
  while(!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = data;
  return;
}
  
  
void UART_SendString(const char* str, char end){
  while(*str)
    UART_SendByte(*str++);
  if (end != '\0') UART_SendByte(end);
  return;
}
  
  
  void UART_SendUInt(unsigned long value, char base){
    char buf[33];
    intInStrTranslate(value, buf, base);
    UART_SendString(buf);
    UART_SendByte('\r');
    UART_SendByte('\n');
  }
  
  
  void intInStrTranslate(unsigned long value, char* str, char base){
    char buf[33];
    char* ptr = &buf[sizeof(buf) - 1];
    *ptr = '\0';
    do{
      unsigned long m = value;
      value /= base;
      char c = m - base * value;
      *--ptr = c < 10 ? c + '0' : c - 10 + 'A' ;
    } while (value);
  
    while(*ptr)
      *str++ = *ptr++; 
    *str = '\0';
    return;
  }
  
  
  unsigned char UART_ReceiveByte(void){
    while(!(UCSR0A & (1 << RXC0)))
      ;
    return UDR0;
  }
  

  void UART_ReceiveString(char* str, unsigned char max_lenght){
    for(unsigned char i = 0; i < max_lenght - 1; i++){
      unsigned char receivedChar = UART_ReceiveByte();
      if (receivedChar == '\r' || receivedChar == '\n'){
        *str = '\0';
        break;
      }
      *str++ = receivedChar;
    }
    *str = '\0';
    return;
  }



// Функция для чтения байта из буфера
unsigned char USART_Read(unsigned char *data) {
  if (rx_head == rx_tail) {
      return 0; // Буфер пуст
  }
  
  *data = rx_buffer[rx_tail];
  rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
  return 1;
}


ISR(USART_RX_vect) {
    // Чтение полученного байта
    char data = UDR0;
    
    // Расчет следующей позиции в буфере
    uint8_t next_head = (rx_head + 1) % RX_BUFFER_SIZE;
    
    if (next_head != rx_tail) {
        // Сохраняем данные если есть место в буфере
        rx_buffer[rx_head] = data;
        rx_head = next_head;
    }
    // Иначе данные теряются при переполнении буфера
}