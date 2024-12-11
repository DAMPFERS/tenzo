#include <encoder_RVV.h>
#include <avr/interrupt.h>

volatile unsigned char encoder_state = 0;

// ISR(TIMER1_COMPA_vect){
//     static unsigned char new_state = 0;
//     static unsigned char old_state = 0;
//     new_state = (PIND & (1 << PD3 | 1 << PD4)) >> 3;
//     switch (old_state | new_state)
//     {
//     case 0x01: case 0x0e:
//         encoder_state = RIGHT;
//         break;
//     case 0x04: case 0x0b:
//         encoder_state = LEFT;
//         break;
//     default:
//         encoder_state = 0;
//         break;
//     }
//     old_state = new_state << 2;
// }


ISR(TIMER0_COMPA_vect){
    static unsigned char new_state = 0;
    static unsigned char old_state = 0;
    new_state = (PIND & (1 << PD3 | 1 << PD4)) >> 3;
    switch (old_state | new_state)
    {
    case 0x01: case 0x0e:
        encoder_state = RIGHT;
        break;
    case 0x04: case 0x0b:
        encoder_state = LEFT;
        break;
    default:
        encoder_state = 0;
        break;
    }
    old_state = new_state << 2;
}

void encoderInit(unsigned char left_pin, unsigned char right_pin){
    DDRD &= ~(1 << PD2 | 1 << PD3| 1 << PD4);
    PORTD = 1 << PD2;
    // cli();
    // OCR1A = 25; // интервал в 100 мкс (10 кГц) при 16 мГц
    // TCCR1B |= 1 << WGM12;
    // TCCR1B |= 1 << CS11 | 1 << CS10;
    // TIMSK1 |= 1 << OCIE1A;
    // sei();

    cli();
    OCR0A = 7;      //  112 uc опрос 
    TCCR0A |= 1 << WGM01;
    TCCR0B |=  1 << CS02; // прескеллер 1/256
    TIMSK0 |= 1 << OCIE0A;
    sei();
    return;
}

unsigned char buttonReading(unsigned char button_pin){
    return (PIND & (1 << PD2)) == 0;
}
