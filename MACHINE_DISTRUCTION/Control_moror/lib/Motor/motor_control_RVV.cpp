#include <motor_control_RVV.h>

volatile double now_angle = 0.0;
volatile double new_angle = 0.0;
volatile unsigned char flag_start_timer_motor = 0;

const unsigned char mask_pin_motor_step_4096[8]={
    0b1000,
    0b1100,
    0b0100,
    0b0110,
    0b0010,
    0b0011,
    0b0001,
    0b1001
};


ISR(TIMER1_COMPA_vect){
    // PORTB ^= 1 << PB0; 
    static unsigned char count = 0;
    if (new_angle != now_angle){
        if(flag_start_timer_motor == UP){
            PORTB = mask_pin_motor_step_4096[(count++)%8];
            // now_angle += STEP_ANGLE;
        }
        else if (flag_start_timer_motor == DOWN){
            PORTB = mask_pin_motor_step_4096[(count--)%8];
            // now_angle -= STEP_ANGLE;
        }
    }
    return;
}


void motorInit(unsigned char in1, unsigned char in2, unsigned char in3, unsigned char in4){
    DDRB |= 1 << in1 | 1 << in2 | 1 << in3 | 1 << in4;
    PORTB |= (1 << in1 | 1 << in2 | 1 << in3 | 1 << in4);
    // PORTB ^= 1 << PB0;
    // OCR0A = 255;      // время (0.064; 16.320) ms 
    // TCCR0A |= 1 << WGM01;
    // TCCR0B |= 1 << CS00 | 1 << CS02; // прескеллер 1024
    // TIMSK0 |= 1 << OCIE0A;

    cli();
    OCR1A = (8)* MIN_TIMER_COUNT; 
    TCCR1B |= 1 << WGM12;
    TCCR1B |= 1 << CS12;
    TIMSK1 |= 1 << OCIE1A;
    sei();
    
}

void setAngle(double angle){    
    if (angle > now_angle)
        flag_start_timer_motor = UP;
    else if ((angle < now_angle) && (angle > 0))
        flag_start_timer_motor = DOWN;
    else
        flag_start_timer_motor = 0;
    new_angle = angle;
    return;
}

void setSeed(double speed){
    
    setAngle(new_angle + STEP_ANGLE);
    return;
}