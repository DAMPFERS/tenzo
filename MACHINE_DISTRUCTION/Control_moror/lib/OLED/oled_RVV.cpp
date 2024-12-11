#include <oled_RVV.h>


void I2C_Init(unsigned long freq_data_I2C ){
    // Настройка частоты I2C
    TWBR = ((F_CPU / freq_data_I2C) - 16) / 2;
    TWSR = 0;

    // Включение I2C
    TWCR = (1 << TWEN);
    _delay_ms(100);
    return;
}


// Функция для начала передачи (START condition)
void I2C_Start(void){
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
    ;
    return;
}


// Функция для окончания передачи (STOP condition)
void I2C_Stop(void){
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    return;
}


// Функция для записи данных
void I2C_Write(unsigned char data){
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
    ;
    return;
}



// Функция для отправки команды на OLED экран
void SSD1306_SendCommand(unsigned char command) {
    I2C_Start();
    // I2C_Write(SSD1306_I2C_ADDR);  // Адрес устройства
    I2C_Write(SSD1306_I2C_ADDR << 1);  // Адрес устройства сдвинут
    I2C_Write(0x00);  // Co = 0x00 (команда)
    I2C_Write(command);  // Отправка команды
    I2C_Stop();
    return;
}


// Функция для отправки данных на OLED экран
void SSD1306_SendData(unsigned char data) {
    I2C_Start();
    // I2C_Write(SSD1306_I2C_ADDR);  // Адрес устройства
    I2C_Write(SSD1306_I2C_ADDR << 1);  // Адрес устройства сдвинут
    I2C_Write(0x40);  // Co = 0x40 (данные)
    I2C_Write(data);  // Отправка данных
    I2C_Stop();
    return;
}


// Функция для инициализации OLED экрана
void SSD1306_Init(void) {
    SSD1306_SendCommand(0xAE);  // Выключить дисплей
    SSD1306_SendCommand(0xD5);  // Установить частоту осциллятора
    SSD1306_SendCommand(0x80);  // Частота осциллятора
    SSD1306_SendCommand(0xA8);  // Установить мультиплексирование
    SSD1306_SendCommand(0x3F);  // Мультиплексирование 1/64
    SSD1306_SendCommand(0xD3);  // Установить смещение дисплея
    SSD1306_SendCommand(0x00);  // Смещение 0
    SSD1306_SendCommand(0x40 | 0x00);  // Начальная строка
    SSD1306_SendCommand(0x8D);  // Включить зарядку насоса
    SSD1306_SendCommand(0x14);  // Включить зарядку насоса
    SSD1306_SendCommand(0x20);  // Установить режим памяти
    SSD1306_SendCommand(0x00);  // Горизонтальный адресный режим
    SSD1306_SendCommand(0xA1);  // Установить режим сканирования
    SSD1306_SendCommand(0xC8);  // Нормальный режим сканирования
    SSD1306_SendCommand(0xDA);  // Установить COM Pins
    SSD1306_SendCommand(0x12);  // COM Pins
    SSD1306_SendCommand(0x81);  // Установить контраст
    SSD1306_SendCommand(0xCF);  // Контраст
    SSD1306_SendCommand(0xD9);  // Установить предупреждение
    SSD1306_SendCommand(0xF1);  // Предупреждение
    SSD1306_SendCommand(0xDB);  // Установить VCOMH
    SSD1306_SendCommand(0x40);  // VCOMH
    SSD1306_SendCommand(0xA4);  // Нормальный дисплей
    SSD1306_SendCommand(0xA6);  // Нормальный дисплей
    SSD1306_SendCommand(0xAF);  // Включить дисплей
    return;
}


// Функция для очистки экрана
void SSD1306_Clear(void) {
    for (unsigned char i = 0; i < 8; i++) {
        SSD1306_SendCommand(0xB0 + i);  // Установить страницу
        SSD1306_SendCommand(0x00);  // Установить нижнюю колонку
        SSD1306_SendCommand(0x10);  // Установить верхнюю колонку
        for (unsigned char j = 0; j < 128; j++) {
            SSD1306_SendData(0x00);  // Очистить данные
        }
    }
}


// Функция для отображения байта на экране
void SSD1306_DisplayByte(unsigned char x, unsigned char y, unsigned char data) {
    // unsigned char page = y / 8;
    SSD1306_SendCommand(0xB0 + y);  // Установить страницу
    SSD1306_SendCommand(0x00 + (x & 0x0F));  // Установить нижнюю колонку
    SSD1306_SendCommand(0x10 + (x >> 4));  // Установить верхнюю колонку
    SSD1306_SendData(data);  // Отправить данные
}


// Функция для установки пикселя
void SSD1306_SetPixel(unsigned char x, unsigned char y, unsigned char color) {
    unsigned char page = y / 8;
    unsigned char bit = y % 8;
    SSD1306_SendCommand(0xB0 + page);  // Установить страницу
    SSD1306_SendCommand(0x00 + (x & 0x0F));  // Установить нижнюю колонку
    SSD1306_SendCommand(0x10 + (x >> 4));  // Установить верхнюю колонку
    unsigned char data = 0;
    if (color) {
        data |= (1 << bit);  // Установить бит
    } else {
        data &= ~(1 << bit);  // Сбросить бит
    }
    SSD1306_SendData(data);  // Отправить данные
}
