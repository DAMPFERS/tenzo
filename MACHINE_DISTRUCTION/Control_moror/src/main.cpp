#define __AVR_ATmega168__
#define F_CPU 16000000UL  // Частота тактирования микроконтроллера
#define TWI_FREQ 100000UL  // Желаемая частота I2C


#include <avr/io.h>
#include <avr/interrupt.h>

#include <oled_RVV.h>
#include <encoder_RVV.h>
#include <motor_control_RVV.h>


#define POSITION_NOW 50
#define POSITION_NEW 50
#define PAGE_NOW     1
#define PAGE_NEW     3

#define SETTINGS  0
#define ANGLE     2
#define SPEED     3
#define SET       4
#define START     5
#define STOP      6

#define BUTTON_PIN           PD2
#define LEFT_ENCODER_PIN     PD3
#define RIGHT_ENCODER_PIN    PD4

#define STATE_SELECT 0
#define STATE_SET    1

#define WORK_ANGLE    1
#define WORK_SPEED    2

void WRITE_MENU();
void write_SETTINGS();
void write_ANGLE();
void write_SPEED();
void write_SET();
void write_START();
void write_STOP();

unsigned char write_NEW(unsigned char x, unsigned char page);
unsigned char write_NOW(unsigned char x, unsigned char page);

void write_line();

void clear_selection(unsigned char y);
void write_selection(unsigned char y);

void write_number(float number, unsigned char x, unsigned char page);

void startMotor(unsigned char start, unsigned char work, float angle, float speed);


int main(void) { 
  // DDRB |= 1 << PB0;
  // PORTB ^= 1 << PB0;
  // I2C_Init(FREQ_DATA_I2C);
  // I2C_Start();
  // SSD1306_Init();

  // WRITE_MENU();

  // unsigned char end_positinon_NOW = write_NOW(POSITION_NOW, PAGE_NOW);
  // unsigned char end_positinon_NEW = write_NEW(POSITION_NEW, PAGE_NEW);

  // write_number(0.0, end_positinon_NOW, PAGE_NOW);
  // write_number(0.0, end_positinon_NEW, PAGE_NEW);
  
  // unsigned char position_select = ANGLE;
  // write_selection(position_select);

  // float angle = 0.0;
  float speed = 0.0;

  double delta_angle = STEP_ANGLE * 10;
  float delta_speed = 0.01;

  unsigned char state_flag = STATE_SELECT;
  // unsigned char start_flag = STOP;
  unsigned char work_flag = 0;
  
  motorInit(PB0,PB1,PB2,PB3);
  // encoderInit(LEFT_ENCODER_PIN, RIGHT_ENCODER_PIN);
  setAngle(100 * STEP_ANGLE);

  while (1) {
    // _delay_ms(1000);
  // PORTB ^= 1 << PB0;
    
    /*
    // startMotor(start_flag, work_flag, angle, speed);

    if (encoder_state == LEFT){
      encoder_state = 0;
      if (state_flag == STATE_SELECT){
        clear_selection(position_select);
        position_select = position_select > ANGLE ? position_select - 1 : STOP;
        write_selection(position_select);
        
      }
        
      // else if (state_flag == STATE_SET){
      //   if (position_select == ANGLE){
      //     write_number(angle, end_positinon_NOW, PAGE_NOW);
      //     angle = angle < delta_angle ? 0.0 : angle - delta_angle;
      //     write_number(angle, end_positinon_NEW, PAGE_NEW);
      //   }
      //   else if (position_select == SPEED) {
      //     write_number(speed, end_positinon_NOW, PAGE_NOW);
      //     speed = speed < delta_speed ? 0.0 : speed - delta_speed;
      //     write_number(speed, end_positinon_NEW, PAGE_NEW);
      //   }
      // }

    }




    else if(encoder_state == RIGHT){
      encoder_state = 0;
      if (state_flag == STATE_SELECT){
        clear_selection(position_select);
        position_select = position_select < STOP ? position_select + 1 : ANGLE;
        write_selection(position_select);
        
      }
        
      // else if (state_flag == STATE_SET){
      //   if (position_select == ANGLE){
      //     write_number(angle, end_positinon_NOW, 3);
      //     angle += delta_angle;
      //     write_number(angle, end_positinon_NEW, 3);
      //   }
      //   else if (position_select == SPEED) {
      //     write_number(speed, end_positinon_NOW, 3);
      //     speed += delta_speed;
      //     write_number(speed, end_positinon_NEW, 3);
      //   }
      // }
    }
    
    
    /*
    // обработка нажатия кнопки
    if (buttonReading(BUTTON_PIN)){
      if (state_flag == STATE_SELECT){
        switch (position_select)
        {
        case ANGLE:
          work_flag = WORK_ANGLE;
          break;
        case SPEED:
          work_flag = WORK_SPEED;
          break;
        case START:
          start_flag = START;
          break;
        case STOP:
          start_flag = STOP;
          break;
        }
        state_flag = STATE_SET;
      }
        
      else if (state_flag == STATE_SET)   state_flag = STATE_SELECT;
    }*/


    
  }
}

void WRITE_MENU(){
  I2C_Init(FREQ_DATA_I2C);

  SSD1306_Init();
  SSD1306_Clear();

  write_line();

  write_SETTINGS();
  write_ANGLE();
  write_SPEED();
  write_SET();
  write_START();
  write_STOP();
  return;
}

void write_SETTINGS(){
  unsigned char text[7][6] ={
    {0x46, 0x89, 0x99, 0x91, 0x62, 0x0}, // S
    {0xff, 0x89, 0x89, 0x89, 0x89, 0x0}, // E
    {0x1, 0x1, 0xff, 0x1, 0x1, 0x0}, // T
    {0x1, 0x1, 0xff, 0x1, 0x1, 0x0}, // T
    {0x0, 0x81, 0xff, 0x81, 0x0, 0x0}, // I
    {0xff, 0x6, 0x18, 0x60, 0xff, 0x0}, // N
    {0x3c, 0x42, 0x91, 0x91, 0x73, 0x0}  // G
  };
  unsigned char x = 0;
  for(unsigned char i = 0; i < 7; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x++, SETTINGS, text[i][j]);
    }
  }
  return;
}


void write_SET(){
  unsigned char text[3][6] ={
    {0x46, 0x89, 0x99, 0x91, 0x62, 0x0}, // S
    {0xff, 0x89, 0x89, 0x89, 0x89, 0x0}, // E
    {0x1, 0x1, 0xff, 0x1, 0x1, 0x0}  // T
    
  };
  unsigned char x = 0;
  for(unsigned char i = 0; i < 3; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x++, SET, text[i][j]);
    }
  }
  return;
}

void write_SPEED(){
  unsigned char text[5][6] ={
    {0x46, 0x89, 0x99, 0x91, 0x62, 0x0}, // S
    {0xff, 0x9, 0x9, 0x9, 0xf, 0x0}, // P
    {0xff, 0x89, 0x89, 0x89, 0x89, 0x0}, // E
    {0xff, 0x89, 0x89, 0x89, 0x89, 0x0}, // E
    {0xff, 0x81, 0x81, 0x66, 0x18, 0x0}  // D
    
  };
  unsigned char x = 0;
  for(unsigned char i = 0; i < 5; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x++, SPEED, text[i][j]);
    }
  }
  return;
}


void write_ANGLE(){
  unsigned char text[5][6] ={
    {0xfc, 0x16, 0x11, 0x16, 0xfc, 0x0}, // A
    {0xff, 0x6, 0x18, 0x60, 0xff, 0x0}, // N
    {0x3c, 0x42, 0x91, 0x91, 0x73, 0x0}, // G
    {0xff, 0x80, 0x80, 0x80, 0x80, 0x0}, // L
    {0xff, 0x89, 0x89, 0x89, 0x89, 0x0}  // E
    
  };
  unsigned char x = 0;
  for(unsigned char i = 0; i < 5; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x++, ANGLE, text[i][j]);
    }
  }
  return;
}


void write_START(){
  unsigned char text[5][6] ={
    {0x46, 0x89, 0x99, 0x91, 0x62, 0x0}, // S
    {0x1, 0x1, 0xff, 0x1, 0x1, 0x0}, // T
    {0xfc, 0x16, 0x11, 0x16, 0xfc, 0x0}, // A
    {0xff, 0x9, 0x39, 0x69, 0xc6, 0x0}, // R
    {0x1, 0x1, 0xff, 0x1, 0x1, 0x0}  // T
    
  };
  unsigned char x = 0;
  for(unsigned char i = 0; i < 5; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x++, START, text[i][j]);
    }
  }
  return;
}

void write_STOP(){
  unsigned char text[4][6] ={
    {0x46, 0x89, 0x99, 0x91, 0x62, 0x0}, // S
    {0x1, 0x1, 0xff, 0x1, 0x1, 0x0}, // T
    {0x7e, 0x81, 0x81, 0x81, 0x7e, 0x0}, // O
    {0xff, 0x9, 0x9, 0x9, 0xf, 0x0}  // P
  };
  unsigned char x = 0;
  for(unsigned char i = 0; i < 4; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x++, STOP, text[i][j]);
    }
  }
  return;
}


unsigned char write_NEW(unsigned char x, unsigned char page){
  unsigned char text[3][6] ={
    {0xff, 0x6, 0x18, 0x60, 0xff, 0x0}, // N
    {0xff, 0x89, 0x89, 0x89, 0x89, 0x0}, // E
    {0xff, 0x60, 0x30, 0x60, 0xff, 0x0}  // W
  };
  unsigned char postion = 0;
  for(unsigned char i = 0; i < 3; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x + postion, page, text[i][j]);
      postion++;
    }
  }
  SSD1306_DisplayByte(x + postion, page, 0x36);
  postion++;
  SSD1306_DisplayByte(x + postion, page, 0x36);
  postion++;
  postion++;
  
  return x + postion;
}


unsigned char write_NOW(unsigned char x, unsigned char page){
  unsigned char text[3][6] ={
    {0xff, 0x6, 0x18, 0x60, 0xff, 0x0}, // N
    {0x7e, 0x81, 0x81, 0x81, 0x7e, 0x0}, // O
    {0xff, 0x60, 0x30, 0x60, 0xff, 0x0}  // W
  };
  unsigned char postion = 0;
  for(unsigned char i = 0; i < 3; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x + postion, page, text[i][j]);
      postion++;
    }
  }
  SSD1306_DisplayByte(x + postion, page, 0x36);
  postion++;
  SSD1306_DisplayByte(x + postion, page, 0x36);
  postion++;
  postion++;

  return x + postion;
}


void write_selection(unsigned char y){
  unsigned char text[6] ={0x1c, 0x22, 0x2a, 0x22, 0x1c, 0x00};
  unsigned char x = 36;
  for(unsigned char j = 0; j < 5; j++){
    SSD1306_DisplayByte(x++, y, text[j]);
  }
  return;
}

void clear_selection(unsigned char y){
  unsigned char x = 36;
  for(unsigned char j = 0; j < 5; j++){
    SSD1306_DisplayByte(x++, y, 0x00);
  }
  return;
}


void write_line(){
  for(unsigned char i = 0; i < 8; i++)
    SSD1306_DisplayByte(44, i, 0xff);
}


void write_number(float number, unsigned char x, unsigned char page){
  unsigned char text[10][6] ={
    {0x7e, 0xa1, 0x99, 0x85, 0x7e, 0x0}, // 0
    {0x84, 0x86, 0xff, 0x80, 0x80, 0x0}, // 1
    {0xc2, 0xa1, 0x91, 0x89, 0x86, 0x0}, // 2
    {0x91, 0x91, 0x91, 0x91, 0x7e, 0x0}, // 3
    {0xf, 0x8, 0x8, 0x8, 0xff, 0x0}, // 4
    {0x8f, 0x91, 0x91, 0x91, 0x63, 0x0}, // 5
    {0x7e, 0x91, 0x91, 0x91, 0x63, 0x0}, // 6
    {0x3, 0x1, 0xf1, 0x9, 0x7, 0x0}, // 7
    {0x6e, 0x91, 0x91, 0x91, 0x6e, 0x0}, // 8
    {0x4e, 0x91, 0x91, 0x91, 0x7e, 0x0}  // 9
  };
  
  // unsigned char point[6] = {0x7e, 0x85, 0x99, 0xa1, 0x7e, 0x00};
  
  char buffer[8];
  unsigned char index = 0; 
  // Разделяем число на целую и дробную части
  int integerPart = (int)number;
  float fractionalPart = number - integerPart;

  if (integerPart == 0) {
    buffer[index++] = '0';
  }
  else {
    int temp = integerPart;
    int digits = 0;
    while (temp > 0) {
      digits++;
      temp /= 10;
    }
    for (int i = digits - 1; i >= 0; i--) {
      buffer[index + i] = (integerPart % 10) + '0';
      integerPart /= 10;
    }
    index += digits;
  }

  
  // Добавляем точку
  buffer[index++] = '.';
    
  for (unsigned char i = 0; i < 3; i++) {
        fractionalPart *= 10;
        buffer[index++] = (int)fractionalPart + '0';
        fractionalPart -= (int)fractionalPart;
  }

  // Завершаем строку нулевым символом
  buffer[index] = '\0';


  unsigned char position = 0;
  unsigned char i; 
  for (i = 0; buffer[i] != '.'; ++i){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x + position, page, text[buffer[i] - '0'][j]);
      position++;
    }
  }

 
  SSD1306_DisplayByte(x + position, page, 0xC0);
  position++;
  SSD1306_DisplayByte(x + position, page, 0xC0);
  position++;
  SSD1306_DisplayByte(x + position, page, 0x00);
  position++;
 

  for (++i; buffer[i] != '\0'; ++i){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x + position, page, text[buffer[i] - '0'][j]);
      position++;
    }
  }
  
  return;
}




void startMotor(unsigned char start, unsigned char work, float angle, float speed){
  if (start == START){
    if (work == WORK_ANGLE){

    }
    else if (work == WORK_SPEED){

    }
  }
  else if (start == STOP){

  }
    return;
}