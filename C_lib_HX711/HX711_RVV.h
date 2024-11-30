#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio>
#include <stdint.h>


typedef struct {
    uint32_t sens_1;
    uint32_t sens_2;
    uint32_t sens_3;
} SensorsData;


unsigned char GPIO_Init(unsigned char SCK, unsigned char DT1, unsigned char DT2, unsigned char DT3);
uint32_t GPIO_ReadSensor(unsigned char SCK, unsigned char DT, unsigned char GAIN);
SensorsData GPIO_ReadSensorS(unsigned char SCK, unsigned char DT1, unsigned char DT2, unsigned char DT3, unsigned char GAIN);