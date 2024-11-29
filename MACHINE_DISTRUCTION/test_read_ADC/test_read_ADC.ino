#define F_CPU 16000000L
#define BAUND 9600L
#define UBRL_value (F_CPU/(BAUND*16))-1
#define DT 3
#define SCK 2
#define GAIN 128





#include <avr/io.h>
#include "util/delay.h"


unsigned long readLoadCell(uint8_t dt, uint8_t sck);


void init_UART(void){
 UBRR0L= UBRL_value;
 UBRR0H = UBRL_value >> 8;
 
 /* Enable receiver and transmitter */
 UCSR0B = (1<<TXEN0);
 /* Set frame format: 8data, 2stop bit */
 UCSR0C = (3<<UCSZ00);
}

void send_UART(char data){
 while (!(UCSR0A & (1<<UDRE0)));
  UDR0 = data;
 
}

void init_pin(void){
 DDRD = 0x00;
 DDRD |= 1 << SCK;
 DDRD &= ~(1 << DT);
}

void send_UART_string(const char *str) {
 while (*str) {
  send_UART(*str++);
 }
 return;
}

 //PORTD |= 0x04;
 //_delay_ms(1000);




void int_in_str_translete(unsigned long value, char *str, int base) {
 char buf[33]; 
 char *ptr = &buf[sizeof(buf) - 1];
 *ptr = '\0';
 
 do {
  unsigned long m = value;
  value /= base;
  char c = m - base * value;
  *--ptr = c < 10 ? c + '0' : c + 'A' - 10;
 } while (value);
 
 while (*ptr) {
  *str++ = *ptr++;
 }
 *str = '\0';
 return;
}




void uart_println(unsigned long value, int base) {
 char buf[33];
 int_in_str_translete(value, buf, base);
 send_UART_string(buf);
 send_UART('\r');
 send_UART('\n');
 return;
}




int main(void)
{
    /* Replace with your application code */
  init_pin();
  init_UART();

  PORTD &= ~(1 << SCK);
  while (1) {
    uart_println(123, 10);
  }
}

unsigned long readLoadCell(uint8_t dt, uint8_t sck) { 
 while ((1 << dt) & PIND)
 ;
 unsigned long res = 0;
 for (char i = 0; i < 24; i++) {
  PORTD |= (1 << SCK);
  _delay_us(1);
  PORTD &= ~(1 << SCK);
  _delay_us(1);
  res = (res << 1) | (0x01 & (PIND >> dt));
 }
 PORTD |= (1 << SCK);
 _delay_us(1);
 PORTD &= ~(1 << SCK);
 _delay_us(1);
 
 if(GAIN == 64){
 PORTD |= (1 << SCK);
 _delay_us(1);
 PORTD &= ~(1 << SCK);
 _delay_us(1);
 PORTD |= (1 << SCK);
 _delay_us(1);
 PORTD &= ~(1 << SCK);
 _delay_us(1);
 }
 if(GAIN == 32){
  PORTD |= (1 << SCK);
  _delay_us(1);
  PORTD &= ~(1 << SCK);
  _delay_us(1);
 }
 /*PORTD |= (1 << SCK);
 _delay_us(1);
 PORTD &= ~(1 << SCK);
 _delay_us(1);*/
 res = res ^ 0x800000;
 return res;
}