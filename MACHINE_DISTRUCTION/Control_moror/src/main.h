#include <avr/io.h>
#include <avr/interrupt.h>


#define __AVR_ATmega168__
#define F_CPU 16000000UL  // Частота тактирования микроконтроллера
#define TWI_FREQ 100000UL  // Желаемая частота I2C


/* пины для опроса энкодера*/
#define BUTTON_PIN           PD2
#define LEFT_ENCODER_PIN     PD3
#define RIGHT_ENCODER_PIN    PD4
