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
Mutex lcd;
State_VM state;

void refill() {
    bool card_valid;
    while (1) {
        if (!refill_terminal.PICC_IsNewCardPresent()) {
            Thread::wait(100);
            continue;
        }

        if (!refill_terminal.PICC_ReadCardSerial()) {
            Thread::wait(100);
            continue;
        }

        card_valid = true;

        for (int i = 0; i < refill_terminal.uid.size; i++) {
            if (refill_terminal.uid.uidByte[i] != card[i]) {
                card_valid = false;
                break;
            }
        }

        if (card_valid) {
            uses++;
            green = 1;
            Thread::wait(1000);
            green = 0;
        } else {
            red = 1;
            Thread::wait(1000);
            red = 0;
        }
    }
}

void payment() {
    bool card_valid;
    bool key_valid;

    while (1) {
        if (!pay_terminal.PICC_IsNewCardPresent()) {
            Thread::wait(100);
            continue;
        }

        if (!pay_terminal.PICC_ReadCardSerial()) {
            Thread::wait(100);
            continue;
        }

        card_valid = true;
        key_valid = true;

        for (int i = 0; i < pay_terminal.uid.size; i++) {
            if (pay_terminal.uid.uidByte[i] != card[i]) {
                card_valid = false;
                break;
            }
        }

        for (int i = 0; i < pay_terminal.uid.size; i++) {
            if (pay_terminal.uid.uidByte[i] != key[i]) {
                key_valid = false;
                break;
            }
        }

        if (card_valid) {
            if (state != idle) continue;
            if (uses > 0) {
                uses--;
                state = paid;
            } else {
                lcd.lock();
                LCD.cls();
                LCD.printf("You do not have sufficient funds.");
                lcd.unlock();
                Thread::(3000);
                lcd.lock();
                LCD.cls();
                LCD.printf("Please make payment.");
                lcd.unlock();
            }
        } else if (key_valid) {
            if (state == idle) {
                state = unlocked;
                lock_servo = 0.05f;
            } else if (state == unlocked) {
                state = idle;
                lock_servo = 0.15f;
            }
        } else {
            lcd.lock();
            LCD.cls();
            LCD.printf("Please use a valid card!");
            lcd.unlock();
            Thread::(3000);
            lcd.lock();
            LCD.cls();
            LCD.printf("Please make payment.");
            lcd.unlock();
        }

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
    while(1) {
        //left motor moves
        lcd.lock();
        if(state == idle) {
            uLCD.printf("Please make payment");
        }
        while(state == idle) {}
        uLCD.cls()
        if(state == paid){
            uLCD.printf("Please choose snack option");
            while(1) {
                if(left_button) {
                    uLCD.cls()
                    uLCD.printf("vending left");
                    state == vending;
                    left_motor.write(0.2); //duty cycle to adjust  speed
                    break;
                if(right_button) {
                    uLCD.cls()
                    uLCD.printf("vending right");
                    state == vending;
                    right_motor.write(0.2); 
                    break;
                }
        }
        if(state == vending) {
            Thread::wait(1500);
            right_motor.write(0);
            left_motor.write(0);
            state = idle;
            uLCD.printf("Thank you for printing");
            Thread::wait(2000);
            uLCD.cls();
        }
        lcd.unlock();
    }
    
    // If paid, check for if there are any button inputs.
    // If there are button inputs (left or right), turn motor left or motor right, and turn state to "vending". Else, continue polling.
    // If vending, vend for X amount of time on left or right motor. Then go back to idle.
    // Display a thank you for your purchase on LCD.
}

int main() {
    
}
