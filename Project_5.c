/*
 * File:   Project_5.c
 * Author: sophiemartin
 *
 * Created on February 21, 2026, 8:52 AM
 */


#include "xc.h"
#pragma config FNOSC = FRCDIV

// left motor: RA0 = DIR; RA1 = STEP
// right motor: RB0 = DIR; RB1 = STEP


int main(void) {
    
    
    ANSA = 0; // Disables analog mode on port A
    ANSB = 0; // Disables analog mode on port B
    LATA = 0; // Sets all pins on port A to low (0V)
    LATB = 0; // Sets all pins on port A to low (0V)
    
    
    TRISAbits.TRISA0 = 0; // Sets RA0 to output
    TRISAbits.TRISA1 = 0; // Sets RA1 to output
    TRISBbits.TRISB0 = 0; // Sets RB0 to output
    TRISBbits.TRISB1 = 0; // Sets RB1 to output
    
    
    long forwardDrive = 2000; // Sets step pulses
    long turn90 = 200;
    long turn180 = 450;

    
    void __delay_us(int us) {
        for (int i = 0; i < us/3; i ++) {         
        }
    }
    
    
    void stepLeft() { // Manually generates one complete step pulse for left motor (Use OCIRS instead)
        LATAbits.LATA1 = 1; // Sets RA1 (STEP) HIGH
        __delay_us(500); // Waits 500 microseconds while the STEP pin stays HIGH
        LATAbits.LATA1 = 0; // Sets RA1 (STEP) LOW
        __delay_us(500); // Waits 500 microseconds while the STEP pin stays LOW
    }

    
    void stepRight() {
        LATBbits.LATB1 = 1;
        __delay_us(500);
        LATBbits.LATB1 = 0;
        __delay_us(500);
    }
    
    
    void driveForward(long steps) { // Sets both motors to HIGH
        LATAbits.LATA0 = 1;  // Left CW
        LATBbits.LATB0 = 1;  // Right CW
    
        for(long i = 0; i < steps; i++) {
            stepLeft();
            stepRight();
        }
    }   
    
    
    void turnRight(long steps) { // Motors spin opposite directions (turns)
        LATAbits.LATA0 = 1;  // Left CW
        LATBbits.LATB0 = 0;  // Right CCW
    
        for(long i = 0; i < steps; i++) {
            stepLeft();
            stepRight();
        }
    }
    
    
    while(1)
    {
        driveForward(forwardDrive);

        turnRight(turn90);

        driveForward(forwardDrive);

        turnRight(turn180);

        driveForward(forwardDrive);

        while(1);
    }
    
          
    return 0;
}