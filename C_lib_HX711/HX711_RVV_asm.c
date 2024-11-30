#include <HX711_RVV_asm.h>

void gpio_set_output(uint32_t pin) {
    *((volatile uint32_t*)GPIO_FSEL0) |= (1 << (pin * 3));  // Настройка пина как выход
}

void gpio_set_high(uint32_t pin) {
    *((volatile uint32_t*)GPIO_SET0) = pin;  // Установка высокого уровня
}

void gpio_set_low(uint32_t pin) {
    *((volatile uint32_t*)GPIO_CLR0) = pin;  // Установка низкого уровня
}


void gpio_set_input(uint32_t pin) {
    *((volatile uint32_t*)GPIO_FSEL0) &= ~(7 << (pin * 3));  // Настройка пина как вход
}


int gpio_read(uint32_t pin) {
    return (*((volatile uint32_t*)GPIO_PIN_LEVEL0) & pin) != 0;  // Чтение уровня на пине
}

int main() {
    // Настроим пин 17 как вход и пин 18 как выход
    gpio_set_input(17);
    gpio_set_output(18);

    while (1) {
        // Считываем состояние пина 17
        int value = gpio_read(GPIO_PIN_17);

        // Если пин 17 высокий, делаем пин 18 высоким, иначе низким
        if (value) {
            gpio_set_high(GPIO_PIN_18);
        } else {
            gpio_set_low(GPIO_PIN_18);
        }

        usleep(100000);  // Задержка 100ms
    }

    return 0;
}