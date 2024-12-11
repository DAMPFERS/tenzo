#define LEFT    1
#define RIGHT   2


// #define 

extern volatile unsigned char encoder_state;


void encoderInit(unsigned char left_pin, unsigned char right_pin);

unsigned char buttonReading(unsigned char button_pin);