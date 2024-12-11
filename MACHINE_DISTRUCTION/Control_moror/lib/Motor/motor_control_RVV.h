#include <avr/interrupt.h>
#include <avr/io.h>


#define MIN_TIMER_COUNT     56
#define MIN_TIMER_mS        0.16        
#define MAX_TIMER_COUNT     1568
#define MAX_TIMER_mS        MAX_TIMER_COUNT * MIN_TIMER_mS
#define MAX_TIMER_STEP      MAX_TIMER_COUNT / MIN_TIMER_COUNT

#define STEP_TIMER_MOTOR    MIN_TIMER_COUNT   



#define STEP 4096
#define STEP_ANGLE (double)(360.0 / STEP)
#define UP      2
#define DOWN    3

extern volatile double now_angle;
extern volatile double new_angle;

void motorInit(unsigned char in1,unsigned char in2,unsigned char in3, unsigned char in4);
void setAngle(double angle);
void setSeed(double speed);