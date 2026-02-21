/*
* File: Project_5.c
* Author: sophiemartin
*
* Created on February 21, 2026, 8:52 AM
*/


#include "xc.h"
#pragma config FNOSC = FRCDIV

// left motor: RA0 = DIR; RA1 = STEP
// right motor: RB0 = DIR; RB1 = STEP


int main(void) {

    ANSA = 0;
    ANSB = 0;
    LATA = 0;
    LATB = 0;

    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;

    long forwardDrive = 1000;
    long turn90 = 200;
    long turn180 = 450;
    
    void __delay_us(int us) {
        for(int i = 0; i < us/3; i ++) {
        }
    }
    
    void stepLeft() {
        LATAbits.LATA1 = 1;
        __delay_us(500);
        LATAbits.LATA1 = 0;
        __delay_us(500);
    }

    void stepRight() {
        LATBbits.LATB1 = 1;
        __delay_us(500);
        LATBbits.LATB1 = 0;
        __delay_us(500);
    }

    void driveForward(long steps) {
        LATAbits.LATA0 = 1; // Left CW
        LATBbits.LATB0 = 1; // Right CW

        for(long i = 0; i < steps; i++) {
            stepLeft();
            stepRight();
        }
    }

    void turnRight(long steps) {
        LATAbits.LATA0 = 1; // Left CW
        LATBbits.LATB0 = 0; // Right CCW

        for(long i = 0; i < steps; i++) {
            stepLeft();
            stepRight();
        }
    }

    //while(1)
    //{
        driveForward(forwardDrive);

        turnRight(turn90);

        driveForward(forwardDrive);

        turnRight(turn180);

        driveForward(forwardDrive);
    //}
        while(1){
        }


    return 0;
}
