#include "HX711_RVV.h"


unsigned char GPIO_Init(unsigned char SCK, unsigned char DT1, unsigned char DT2, unsigned char DT3){
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed\n");
        return 1;
    }
    // Устанавливаем пин в режим вывода
    gpioSetMode(SCK, PI_OUTPUT);

    // Устанавливаем пины в режим ввода
    gpioSetMode(DT1, PI_INPUT);
    gpioSetMode(DT2, PI_INPUT);
    gpioSetMode(DT3, PI_INPUT);

    return 0;
}


uint32_t GPIO_ReadSensor(unsigned char SCK, unsigned char DT, unsigned char GAIN){
    while(gpioRead(DT))
    ;
    uint32_t res = 0;

    for(char i = 0; i < 24; i++){
        gpioWrite(PIN, 1);
        usleep(1);  // Задержка 1 mks
        gpioWrite(PIN, 0);
        usleep(1);  // Задержка 1 mks
        res = (res << 1) | (0x01 & gpioRead(DT));
    }
    gpioWrite(PIN, 1);
    usleep(1);  // Задержка 1 mks
    gpioWrite(PIN, 0);
    usleep(1);  // Задержка 1 mks

    if(GAIN == 64){
    gpioWrite(PIN, 1);
    usleep(1);  // Задержка 1 mks
    gpioWrite(PIN, 0);
    usleep(1);  // Задержка 1 mks
    gpioWrite(PIN, 1);
    usleep(1);  // Задержка 1 mks
    gpioWrite(PIN, 0);
    usleep(1);  // Задержка 1 mks
    }
    else if (GAIN == 32){
        gpioWrite(PIN, 1);
        usleep(1);  // Задержка 1 mks
        gpioWrite(PIN, 0);
        usleep(1);  // Задержка 1 mks
    }
    return res ^ 0x800000;
}


SensorsData GPIO_ReadSensorS(unsigned char SCK, unsigned char DT1, unsigned char DT2, unsigned char DT3, unsigned char GAIN){
    while(gpioRead(DT1) || gpioRead(DT2) || gpioRead(DT3))
    ;
    SensorsData res;
    res.sens_1 = 0;
    res.sens_2 = 0;
    res.sens_3 = 0;

    for(char i = 0; i < 24; i++){
        gpioWrite(PIN, 1);
        usleep(1);  // Задержка 1 mks
        gpioWrite(PIN, 0);
        usleep(1);  // Задержка 1 mks
        res.sens_1 = (res.sens_1 << 1) | (0x01 & gpioRead(DT1));
        res.sens_2 = (res.sens_2 << 1) | (0x01 & gpioRead(DT2));
        res.sens_3 = (res.sens_3 << 1) | (0x01 & gpioRead(DT3));
    }
    gpioWrite(PIN, 1);
    usleep(1);  // Задержка 1 mks
    gpioWrite(PIN, 0);
    usleep(1);  // Задержка 1 mks

    if(GAIN == 64){
    gpioWrite(PIN, 1);
    usleep(1);  // Задержка 1 mks
    gpioWrite(PIN, 0);
    usleep(1);  // Задержка 1 mks
    gpioWrite(PIN, 1);
    usleep(1);  // Задержка 1 mks
    gpioWrite(PIN, 0);
    usleep(1);  // Задержка 1 mks
    }
    else if (GAIN == 32){
        gpioWrite(PIN, 1);
        usleep(1);  // Задержка 1 mks
        gpioWrite(PIN, 0);
        usleep(1);  // Задержка 1 mks
    }
    res.sens_1 = res.sens_1 ^ 0x800000;
    res.sens_2 = res.sens_2 ^ 0x800000;
    res.sens_3 = res.sens_3 ^ 0x800000;
    return res;
}

