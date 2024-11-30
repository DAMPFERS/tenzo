Для работы с портами ввода/вывода (GPIO) на Raspberry Pi 4 на языке Си, вам потребуется использовать библиотеку, которая предоставляет функции для управления GPIO. Одной из популярных библиотек для этого является wiringPi. Однако, начиная с Raspberry Pi 4, wiringPi больше не поддерживается, поэтому рекомендуется использовать библиотеку pigpio.

Установка библиотеки pigpio:
sudo apt-get update
sudo apt-get install pigpio libpigpio-dev


Компиляция и запуск программы:
gcc -o gpio_example gpio_example.c -lpigpio -lpthread -lrt
sudo ./gpio_example

----------------------------------------------------
asm:

Отключение кэширования и включение доступа к физической памяти:

sudo sh -c "echo 3 > /proc/cpu/alignment"

________________________-----------____________
gcc -o gpio_dup gpio_dup.c
sudo ./gpio_dup
