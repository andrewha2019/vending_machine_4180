#include "mbed.h"
#include "Motor.h"
#include "rtos.h"
#include "PinDetect.h"
volatile bool paid = false;
volatile bool right_motor_on = false;
volatile bool left_motor_on = false;
Motor right(p21, p5,p6);
Motor left(p22, p7,p8);
PinDetect pb1(p13);
PinDetect pb2(p14);

void right_motor() {
    if (right_motor_on) {
        right.speed(0.05);
        Thread::wait(2000);
        right_motor_on = false;   
    }else {
        right.speed(0);
    }
}
void left_motor() {
    if (left_motor_on) {
        left.speed(0.05);
        Thread::wait(2000);
        left_motor_on = false;   
    }else {
        left.speed(0);
    }
}
void pb1_hit_callback (void) {
    if(paid) {
        right_motor_on = true;
    }
}
// Callback routine is interrupt activated by a debounced pb2 hit
void pb2_hit_callback (void) {
    if(paid) {
        left_motor_on = true;
    }
}


int main() {
    Thread t1;
    Thread t2;
    t1.start(right_motor);
    pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);
    pb1.setSampleFrequency();
    pb2.setSampleFrequency();
    while(1){
        
    }
}
