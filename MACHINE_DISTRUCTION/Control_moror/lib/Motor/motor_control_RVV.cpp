#include <motor_control_RVV.h>
/*
v =  4.998; N = 1099
v = 10.006; N = 549
v = 15.009; N = 366
v = 19.975; N = 275
v = 24.969; N = 220
v = 30.017; N = 183
v = 34.988; N = 157
v = 40.096; N = 137
v = 45.026; N = 122
v = 49.938; N = 110
v = 54.932; N = 100
v = 59.708; N = 92
v = 64.625; N = 85
v = 70.425; N = 78
v = 75.249; N = 73
v = 79.611; N = 69
v = 84.510; N = 65
v = 90.052; N = 61
v = 94.710; N = 58
*/


const uint16_t timer_value_settings[19] = {
    1099,
    549,
    366,
    275,
    220,
    183, 
    157,
    137,
    122,
    110,
    100,
    92,
    85,
    78,
    73,
    69,
    65,
    61, 
    58
};

const float velocity_value_settings[19] = {
    4.998,
    10.006,
    15.009,
    19.975,
    24.969,
    30.017,
    34.988,
    40.096,
    45.026,
    49.938,
    54.932,
    59.708,
    64.625,
    70.425,
    75.249,
    79.611,
    84.51,
    90.052,
    94.71,
};

volatile unsigned char index_velocity = 8;

volatile double now_angle = 0.0;
volatile double new_angle = 0.0;

volatile unsigned char flag_start_timer_motor = 0;
volatile unsigned char run_flag = 0;

volatile unsigned char oscil_flag = 0;
volatile double oscil_angle = 0.0;

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
    static unsigned char count = 0;
    static double now_oscil_angle = 0.0;


    if ((new_angle != now_angle) || run_flag){ // режим установки заданного угла или режим вращения
        if(flag_start_timer_motor == UP){
            PORTB = mask_pin_motor_step_4096[(count++)%8];
            now_angle += STEP_ANGLE;
        }
        else if (flag_start_timer_motor == DOWN){
            PORTB = mask_pin_motor_step_4096[(count--)%8];
            // now_angle -= STEP_ANGLE;
            now_angle = now_angle - STEP_ANGLE > 0 ? now_angle - STEP_ANGLE : 0;
        }
    }


    else if (oscil_flag){
        
        if ((oscil_angle  == now_oscil_angle) || ( oscil_angle == -now_oscil_angle ))
            flag_start_timer_motor = flag_start_timer_motor == UP ? DOWN : UP;
            

        if(flag_start_timer_motor == UP){ // режим вращения
            PORTB = mask_pin_motor_step_4096[(count++)%8];
            now_oscil_angle += STEP_ANGLE;
        }
        
        else if (flag_start_timer_motor == DOWN){
            PORTB = mask_pin_motor_step_4096[(count--)%8];
            now_oscil_angle -= STEP_ANGLE;
        }
    }
    

    return;
}


void motorInit(unsigned char in1, unsigned char in2, unsigned char in3, unsigned char in4){
    DDRB |= 1 << in1 | 1 << in2 | 1 << in3 | 1 << in4;
    PORTB &= ~(1 << in1 | 1 << in2 | 1 << in3 | 1 << in4);

    cli();
    OCR1A = timer_value_settings[index_velocity]; 
    TCCR1B |= 1 << WGM12;
    TCCR1B |= 1 << CS12; // 1 / 256
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

void setSpeed(){
    OCR1A = timer_value_settings[index_velocity];
    return;
}


void runOscilation(unsigned char angle){
    setAngle(angle);
    return;
}