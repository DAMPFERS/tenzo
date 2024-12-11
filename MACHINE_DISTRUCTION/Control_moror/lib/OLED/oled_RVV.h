#ifndef OLED_SSD1306_128x64
#define OLED_SSD1306_128x64
/*
OLED экран SSD1306 обычно имеет адрес 0x3C или 0x3D на шине I2C.
Для отправки команд и данных на экран используется байт Co (Control byte). Этот байт определяет, что будет отправлено далее: команда или данные.
Co = 0x00: Следующие байты будут интерпретироваться как команды.
Co = 0x40: Следующие байты будут интерпретироваться как данные.

Организация памяти дисплея SSD1306
OLED дисплей SSD1306 имеет разрешение 128x64 пикселя. 
Память дисплея организована в виде страниц, где каждая страница содержит 8 пикселей по высоте. 
Всего у дисплея 8 страниц, каждая из которых содержит 128 байт данных 
(по одному байту на каждую колонку).

Адресация памяти
Страницы:
Память дисплея разделена на 8 страниц, каждая из которых содержит 8 пикселей по высоте.
Страницы адресуются командами 0xB0 до 0xB7.
Колонки:
Каждая страница содержит 128 колонок, каждая из которых содержит 1 байт данных.
Колонки адресуются командами 0x00 до 0x0F для нижних 4 бит и 0x10 до 0x1F для верхних 4 бит.

Пример адресации
Для установки адреса памяти в позиции (x, y), где x — это колонка (0-127), а y — это строка (0-63):
Страница: page = y / 8
Бит в байте: bit = y % 8
Колонка: column = x

*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FREQ_DATA_I2C 100000UL
#define SSD1306_I2C_ADDR 0x3C  // Адрес OLED экрана на шине I2C

#define ON_PIX 1
#define OFF_PIX 0

void I2C_Init(unsigned long freq_data_I2C); 
void I2C_Start(void);                               // Функция для начала передачи (START condition)
void I2C_Stop(void);                                // Функция для окончания передачи (STOP condition)
void I2C_Write(unsigned char data);                 // Функция для записи данных
void SSD1306_SendCommand(unsigned char command);    // Функция для отправки команды на OLED экран
void SSD1306_SendData(unsigned char data);          // Функция для отправки данных на OLED экран
void SSD1306_Init(void);                            // Функция для инициализации OLED экрана
void SSD1306_Clear(void);                           // Функция для очистки экрана


void SSD1306_DisplayByte(unsigned char x, unsigned char y, unsigned char data); // Функция для отображения байта на экране
void SSD1306_SetPixel(unsigned char x, unsigned char y, unsigned char color);   // Функция для установки пикселя

unsigned char SSD1306_ReadData(unsigned char x, unsigned char page);            // Функция для чтения данных из памяти дисплея

#endif