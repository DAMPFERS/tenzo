#include <HX711_RVV_asm2.h>


int mem_fd;
void *gpio_map;

// Используемые регистры
volatile unsigned int *gpio;


// Функция для установки бита в регистре
void gpioSet(int pin) {
    *(gpio + 7) = 1 << pin;  // GPFSEL0
}

// Функция для сброса бита в регистре
void gpioClear(int pin) {
    *(gpio + 10) = 1 << pin;  // GPCLR0
}

// Функция для чтения состояния пина
int gpioRead(int pin) {
    return (*(gpio + 13) & (1 << pin)) ? 1 : 0;  // GPLEV0
}

// Функция для инициализации доступа к GPIO
void setup_io() {
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        printf("can't open /dev/mem \n");
        exit(-1);
    }

    gpio_map = mmap(
        NULL,
        BLOCK_SIZE,
        PROT_READ|PROT_WRITE,
        MAP_SHARED,
        mem_fd,
        GPIO_BASE
    );

    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        printf("mmap error %d\n", (int)gpio_map);
        exit(-1);
    }

    gpio = (volatile unsigned *)gpio_map;
}

SensorsData readSensors(uint32_t clk, uint32_t dt1, uint32_t dt2, uint32_t dt3){
    return 0;
}
/*
int main() {
    int pin = 17;  // Номер GPIO пина

    setup_io();

    // Устанавливаем пин в режим вывода
    *(gpio + ((pin)/10)) = (*(gpio + ((pin)/10)) & ~(7<<(((pin)%10)*3))) | (1<<(((pin)%10)*3));

    // Устанавливаем пин в высокий уровень
    gpioSet(pin);
    sleep(1);  // Ждем 1 секунду

    // Устанавливаем пин в низкий уровень
    gpioClear(pin);
    sleep(1);  // Ждем 1 секунду

    // Устанавливаем пин в высокий уровень
    gpioSet(pin);
    sleep(1);  // Ждем 1 секунду

    // Устанавливаем пин в низкий уровень
    gpioClear(pin);

    return 0;
}
*/