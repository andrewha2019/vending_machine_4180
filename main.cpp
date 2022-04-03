#include "mbed.h"
#include "Motor.h"
#include "rtos.h"
#include "PinDetect.h"
volatile bool paid = true;
volatile bool right_motor_on = false;
volatile bool left_motor_on = false;
Motor right(p21, p5,p6);
Motor left(p22, p7,p8);
PinDetect pb1(p13);
PinDetect pb2(p14);
DigitalOut led(LED1);
DigitalOut led2(LED2);
void right_motor() {
    while(1){
        if (right_motor_on) {
            right.speed(0.2);
            right_motor_on = false; 
            Thread::wait(1000);    
        }else {
            right.speed(0);
        }
    }
}
void left_motor() {
    while(1){
        if (left_motor_on) {
            led2 = 1;
            left.speed(0.2);
            left_motor_on = false;
            Thread::wait(1000);   
        }else {
            left.speed(0);
        }
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
    t2.start(left_motor);
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);
    pb1.setSampleFrequency();
    pb2.setSampleFrequency();
    led2 = 0;
    while(1){
        led=!led;
        Thread::wait(200);
    }
}

