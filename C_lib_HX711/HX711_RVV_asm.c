#include <HX711_RVV_asm.h>

void gpioSetOutput(uint32_t pin) {
    *((volatile uint32_t*)GPIO_FSEL0) |= (1 << (pin * 3));  // Настройка пина как выход
}


void gpioSetHigh(uint32_t pin) {
    *((volatile uint32_t*)GPIO_SET0) = pin;  // Установка высокого уровня
}


void gpioSetLow(uint32_t pin) {
    *((volatile uint32_t*)GPIO_CLR0) = pin;  // Установка низкого уровня
}


void gpioSetInput(uint32_t pin) {
    *((volatile uint32_t*)GPIO_FSEL0) &= ~(7 << (pin * 3));  // Настройка пина как вход
}


int gpioRead(uint32_t pin) {
    return (*((volatile uint32_t*)GPIO_PIN_LEVEL0) & pin) != 0;  // Чтение уровня на пине
}


uint8_t gpioReads3Sensors(uint32_t dt1, uint32_t dt2, uint32_t dt3){
    uint32_t level = *((volatile uint32_t*)GPIO_PIN_LEVEL0);
    uint8_t res = (((level & dt1) != 0) << 2) | (((level & dt2) != 0) << 1) | ((level & dt3) != 0);
    return res;
}



void initSensors(uint32_t clk, uint32_t dt1, uint32_t dt2, uint32_t dt3){
    // Настроим пины как вход и пины как выход
    gpioSetInput(dt1);
    gpioSetInput(dt2);
    gpioSetInput(dt3);
    gpioSetOutput(clk);
}


SensorsData readSensors(uint32_t clk, uint32_t dt1, uint32_t dt2, uint32_t dt3){
    while (gpioReads3Sensors(dt1, dt2, dt3) != 0x00)
    ;
    SensorsData res;
    res.sens_1 = 0;
    res.sens_2 = 0;
    res.sens_3 = 0;
    
    for(char i = 0; i < 24; i++){
        gpioSetHigh(clk);
        usleep(1);  // Задержка 1 mks
        gpioSetLow(clk);
        usleep(1);
        uint8_t data = gpioReads3Sensors(dt1, dt2, dt3);
        res.sens_1 = (res.sens_1 << 1) | (0x01 & (data >> 2));
        res.sens_2 = (res.sens_2 << 1) | (0x01 & (data >> 1));
        res.sens_3 = (res.sens_3 << 1) | (0x01 & data);
    }
    gpioSetHigh(clk);
    usleep(1);  // Задержка 1 mks
    gpioSetLow(clk);
    usleep(1);
    if (GAIN == 64){
        gpioSetHigh(clk);
        usleep(1);  // Задержка 1 mks
        gpioSetLow(clk);
        usleep(1);
        gpioSetHigh(clk);
        usleep(1);  // Задержка 1 mks
        gpioSetLow(clk);
        usleep(1);
    }
    else if (GAIN == 32){
        gpioSetHigh(clk);
        usleep(1);  // Задержка 1 mks
        gpioSetLow(clk);
        usleep(1);
    }
    res.sens_1 = res.sens_1 ^ 0x800000;
    res.sens_2 = res.sens_2 ^ 0x800000;
    res.sens_3 = res.sens_3 ^ 0x800000;
    return res;
}
