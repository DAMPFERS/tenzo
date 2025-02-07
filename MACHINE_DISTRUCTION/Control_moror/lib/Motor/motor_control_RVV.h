#include <avr/interrupt.h>
#include <avr/io.h>


#define MIN_TIMER_COUNT     56
#define MIN_TIMER_mS        0.16        
#define MAX_TIMER_COUNT     1568
#define MAX_TIMER_mS        MAX_TIMER_COUNT * MIN_TIMER_mS
#define MAX_TIMER_STEP      MAX_TIMER_COUNT / MIN_TIMER_COUNT

#define STEP_TIMER_MOTOR    MIN_TIMER_COUNT   

/*
v = 4.998; N = 1099
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
v = 84.51; N = 65
v = 90.052; N = 61
v = 94.71; N = 58
*/


#define STEP 4096
#define STEP_ANGLE (double)(360.0 / STEP)
#define UP      2
#define DOWN    3

extern volatile double now_angle;
extern volatile double new_angle;
extern volatile unsigned char run_flag;
extern volatile unsigned char oscil_flag;
extern volatile double oscil_angle;
extern const uint16_t timer_value_settings[19];
extern const float velocity_value_settings[19];
extern volatile unsigned char index_velocity;
extern volatile unsigned char flag_start_timer_motor;

void motorInit(unsigned char in1,unsigned char in2,unsigned char in3, unsigned char in4);
void setAngle(double angle);
void setSpeed();

// void runRotation(unsigned char start);