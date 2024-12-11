#ifndef READ_HX711
#define READ_HX711

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdint.h>

#define BCM2835_GPIO_BASE 0x3F200000
#define GPIO_FSEL0        (BCM2835_GPIO_BASE + 0x00)
#define GPIO_SET0         (BCM2835_GPIO_BASE + 0x1C)
#define GPIO_CLR0         (BCM2835_GPIO_BASE + 0x28)
#define GPIO_PIN_LEVEL0   (BCM2835_GPIO_BASE + 0x34)  // Состояние пинов (входные данные)

#define GAIN                  128
#define GPIO_PIN15_DT1       (1 << 15)  // Пин 15
#define GPIO_PIN16_DT2       (1 << 16)  // Пин 16
#define GPIO_PIN17_DT3       (1 << 17)  // Пин 17
#define GPIO_PIN18_CLK       (1 << 18)  // Пин 18 (выходной)


typedef struct {
    uint32_t sens_1;
    uint32_t sens_2;
    uint32_t sens_3;
} SensorsData;



void gpioSetOutput(uint32_t pin); // Настройка пина как выход
void gpioSetHigh(uint32_t pin);   // Установка высокого уровня
void gpioSetLow(uint32_t pin);    // Установка низкого уровня
void gpioSetInput(uint32_t pin);
int gpioRead(uint32_t pin);
uint8_t gpioReads3Sensors(uint32_t dt1, uint32_t dt2, uint32_t dt3);

void initSensors(uint32_t clk, uint32_t dt1, uint32_t dt2, uint32_t dt3);
SensorsData readSensors(uint32_t clk, uint32_t dt1, uint32_t dt2, uint32_t dt3);

#endif