// #include <main.h>
#include <gui.h>


void WRITE_MENU(){
  I2C_Init(FREQ_DATA_I2C);

  SSD1306_Init();
  SSD1306_Clear();

  write_line();

  write_SETTINGS();
  write_ANGLE();
  write_ROTAT();
  write_OSCIL();
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


void write_ROTAT(){
  unsigned char text[5][6] ={
    {0xfe, 0x12, 0x32, 0x52, 0x46, 0x0}, // R
    {0x7c, 0x82, 0x82, 0x82, 0x7c, 0x0}, // O
    {0x02, 0x02, 0xfe, 0x02, 0x02, 0x0}, // T
    {0xf8, 0x24, 0x22, 0x24, 0xf8, 0x0}, // A
    {0x02, 0x02, 0xfe, 0x02, 0x02, 0x0}  // T
  };
  unsigned char x = 0;
  for(unsigned char i = 0; i < 5; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x++,ROTATION , text[i][j]);
    }
  }
  return;
}


void write_OSCIL(){
  unsigned char text[5][6] ={
    {0x7c, 0x82, 0x82, 0x82, 0x7c, 0x0}, // O
    {0x4c, 0x92, 0x92, 0x92, 0x64, 0x0}, // S
    {0x7c, 0x82, 0x82, 0x82, 0x82, 0x0}, // C
    {0x00, 0x82, 0xfe, 0x82, 0x00, 0x0}, // I
    {0xfe, 0x80, 0x80, 0x80, 0x80, 0x0}  // L
  };
  unsigned char x = 0;
  for(unsigned char i = 0; i < 5; i++){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x++, OSCILATION, text[i][j]);
    }
  }
  return;
}


void write_ANGLE(){
  unsigned char text[5][6] ={
    {0xf8, 0x24, 0x22, 0x24, 0xf8, 0x0}, // A
    {0xfe, 0x0c, 0x10, 0x60, 0xfe, 0x0}, // N
    {0x78, 0x84, 0xb2, 0x92, 0x76, 0x0}, // G
    {0xfe, 0x80, 0x80, 0x80, 0x80, 0x0}, // L
    {0xfe, 0x92, 0x92, 0x92, 0x92, 0x0}  // E
    
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
    {0x4c, 0x92, 0x92, 0x92, 0x64, 0x0}, // S
    {0x02, 0x02, 0xfe, 0x02, 0x02, 0x0}, // T
    {0xf8, 0x24, 0x22, 0x24, 0xf8, 0x0}, // A
    {0xfe, 0x12, 0x32, 0x52, 0x46, 0x0}, // R
    {0x02, 0x02, 0xfe, 0x02, 0x02, 0x0}  // T
    
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
    {0x4c, 0x92, 0x92, 0x92, 0x64, 0x0}, // S
    {0x02, 0x02, 0xfe, 0x02, 0x02, 0x0}, // T
    {0x7c, 0x82, 0x82, 0x82, 0x7c, 0x0}, // O
    {0xfe, 0x12, 0x12, 0x12, 0x0c, 0x0}  // P
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
  unsigned char text[6] ={0x38, 0x44, 0x54, 0x44, 0x38, 0x0};
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
  unsigned char position = 0;
  if (number < 0){
    number = -number;
    SSD1306_DisplayByte(x + position++, page, 0x18);
    SSD1306_DisplayByte(x + position++, page, 0x18);
    SSD1306_DisplayByte(x + position++, page, 0x18);
    // SSD1306_DisplayByte(x + position++, page, 0x18);
    SSD1306_DisplayByte(x + position++, page, 0x00);
  }
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


  
  unsigned char i; 
  for (i = 0; buffer[i] != '.'; ++i){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x + position, page, text[buffer[i] - '0'][j]);
      position++;
    }
  }

 
  SSD1306_DisplayByte(x + position++, page, 0xC0);
  // position++;
  SSD1306_DisplayByte(x + position++, page, 0xC0);
  // position++;
  SSD1306_DisplayByte(x + position++, page, 0x00);
  // position++;
 

  for (++i; buffer[i] != '\0'; ++i){
    for(unsigned char j = 0; j < 6; j++){
      SSD1306_DisplayByte(x + position, page, text[buffer[i] - '0'][j]);
      position++;
    }
  }
  
  return;
}



void write_numberClear(unsigned char x, unsigned char page){
  for(; x < 127; x++)
      SSD1306_DisplayByte(x, page, 0x00);
  return;
}



void write_FOOTNOTE_Clear(){
    for(unsigned char postion = 0; postion < 24; postion++)
      SSD1306_DisplayByte(50 + postion, 7, 0x00);
}

void write_FOOTNOTE(unsigned char state){
  unsigned char deg[6][4] = {
    {0x90, 0xb8, 0x90, 0x00},  // +-
    {0xe0, 0xa0, 0xf8, 0x00},  // d
    {0x78, 0xa8, 0xb8, 0x00},  // e
    {0xb8, 0xa8, 0xf8, 0x00},  // g
    {0xc0, 0x60, 0x30, 0x18},  // /
    {0xb0, 0xa8, 0x68, 0x00},  // s
  };

  unsigned char in_start;
  unsigned char in_end;

  switch (state)
  {
  case ANGLE:
    in_start = 1;
    in_end = 3;
    break;

  case ROTATION:
    in_start = 1;
    in_end = 5;
    break;
  
  case OSCILATION:
    in_start = 0;
    in_end = 3;
    break;
  }

   unsigned char postion = 0;
  for(unsigned char i = in_start; i <= in_end; i++){
    for(unsigned char j = 0; j < 4; j++){
      SSD1306_DisplayByte(50 + postion, 7, deg[i][j]);
      postion++;
    }
  }
  return;
}



