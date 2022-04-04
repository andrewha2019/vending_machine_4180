#include "mbed.h"
#include "rtos.h"
#include "MFRC522.h"
#include "uLCD_4DGL.h"

enum state {
    idle = 0, paid, vending, unlocked
};

volatile int uses = 0;

DigitalOut green(p19);
DigitalOut red(p20);
PwmOut left_motor(p21);
PwmOut right_motor(p22);
PwmOut lock_servo(p23);
DigitalOut left_button(p29);
DigitalOut right_button(p30);
MFRC522 pay_terminal(p5, p6, p7, p8, p16);
MFRC522 refill_terminal(p11, p12, p13, p14, p15);
uLCD_4DGL uLCD(p28, p27, p26);

void refill() {
    // Check for card, and check if valid.
    // Check for card UID.
    // If successful, increment uses by 1, and flash green LED.
    // If unsuccessful, flash red LED.
    // Add a "wait" so that it does not increment multiple times.
}

void payment() {
    // Check for card, and check if valid.
    // Check for card UID.
    // Cases: card UID is pay card, card UID is key, default (error).
    // Case 1: card UID is pay card:
        // If state is not idle, ignore this. Else, keep going.
        // Check number of uses. If > 0, decrement uses, and state is now "paid".
        // Display that payment was successful on LCD.
    // Case 2: card UID is pay card:
        // If state is idle, state is now unlocked. (Move servo to unlock.)
        // If state is unlocked, state is now idle. (Move servo to lock.)
        // Display that it is open if open; else, perhaps a welcome screen(?).
    // Case 3: default:
        // Display error on LCD.
}

void motors() {
    // If paid, check for if there are any button inputs.
    // If there are button inputs (left or right), turn motor left or motor right, and turn state to "vending". Else, continue polling.
    // If vending, vend for X amount of time on left or right motor. Then go back to idle.
    // Display a thank you for your purchase on LCD.
}

int main() {
    
}