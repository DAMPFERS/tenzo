#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define BCM2711_PERI_BASE 0xFE000000
#define GPIO_BASE (BCM2711_PERI_BASE + 0x200000)  // Базовый адрес GPIO

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

typedef struct {
    uint32_t sens_1;
    uint32_t sens_2;
    uint32_t sens_3;
} SensorsData;

void gpioSet(int pin);    // Функция для установки бита в регистре
void gpioClear(int pin);  // Функция для сброса бита в регистре
int gpioRead(int pin);    // Функция для чтения состояния пина
void setup_io();          // Функция для инициализации доступа к GPIO

SensorsData readSensors(uint32_t clk, uint32_t dt1, uint32_t dt2, uint32_t dt3);