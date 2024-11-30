#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define BCM2835_GPIO_BASE 0x3F200000
#define GPIO_FSEL0        (BCM2835_GPIO_BASE + 0x00)
#define GPIO_SET0         (BCM2835_GPIO_BASE + 0x1C)
#define GPIO_CLR0         (BCM2835_GPIO_BASE + 0x28)
#define GPIO_PIN_LEVEL0   (BCM2835_GPIO_BASE + 0x34)  // Состояние пинов (входные данные)


#define GPIO_PIN_17       (1 << 17)  // Пин 17
#define GPIO_PIN_18       (1 << 18)  // Пин 18 (выходной)

void gpio_set_output(uint32_t pin); // Настройка пина как выход
void gpio_set_high(uint32_t pin);   // Установка высокого уровня
void gpio_set_low(uint32_t pin);    // Установка низкого уровня
void gpio_set_input(uint32_t pin);
int gpio_read(uint32_t pin);