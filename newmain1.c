/* 
 * File:   newmain1.c
 * Author: grace
 *
 * Created on March 9, 2026, 9:41 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "xc.h"
#pragma config FNOSC = FRC

/*
 * 
 */
unsigned int targetOC1RS = 0;
unsigned int targetOC2RS = 0;


void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	_T1IF = 0;	// Clear interrupt flag
    static stepSize = 100;
    if(_LATA0) {
        if (OC1RS < -1*targetOC1RS) {
            OC1RS += stepSize;
            if (OC1RS > -1*targetOC1RS) {
                OC1RS = targetOC1RS;
            }
        } else {
            OC1RS -= stepSize;
            if (OC1RS < -1*targetOC1RS) {
                OC1RS = targetOC1RS;
            }
        }
    } else {
        if (OC1RS < targetOC1RS) {
            OC1RS += stepSize;
            if (OC1RS > targetOC1RS) {
                OC1RS = targetOC1RS;
            }
        } else {
            OC1RS -= stepSize;
            if (OC1RS < targetOC1RS) {
                OC1RS = targetOC1RS;
            }
        }
    }
    OC1R = OC1RS/2;
    
    if(_LATA0) {
        if (OC2RS < -1*targetOC2RS) {
            OC2RS += stepSize;
            if (OC2RS > -1*targetOC2RS) {
                OC2RS = targetOC2RS;
            }
        } else {
            OC2RS -= stepSize;
            if (OC2RS < -1*targetOC2RS) {
                OC2RS = targetOC2RS;
            }
        }
    } else {
        if (OC2RS < targetOC2RS) {
            OC2RS += stepSize;
            if (OC2RS > targetOC2RS) {
                OC2RS = targetOC2RS;
            }
        } else {
            OC2RS -= stepSize;
            if (OC2RS < targetOC2RS) {
                OC2RS = targetOC2RS;
            }
        }
    }
    OC2R = OC2RS/2;
}

int leftQRDIsWhite() {
    return ADC1BUF10 < 4095/3;
}

int rightQRDIsWhite() {
    return ADC1BUF11 < 4095/3;
}

void setRightWheelSpeed(double rot_per_sec) {
    // targetOC1RS = 4000000/(rot_per_sec*2*200);
    // if (rot_per_sec == 0) {
    //     OC1CON1bits.OCM = 0;
    //     OC1RS = 0;
    //     OC1R = 0;
    // } else {
    //     OC1CON1bits.OCM = 0b110;
    // }
    OC1CON1bits.OCM = 0b110;
    OC1RS = 10000/rot_per_sec;
    OC1R = OC1RS/2;
}

void setLeftWheelSpeed(double rot_per_sec) {
    // targetOC2RS = 4000000/(rot_per_sec*2*200);
    // if (rot_per_sec == 0) {
    //     OC2CON1bits.OCM = 0;
    //     OC2RS = 0;
    //     OC2R = 0;
    // } else {
    //     OC2CON1bits.OCM = 0b110;
    // }
    OC2CON1bits.OCM = 0b110;
    OC2RS = 10000/rot_per_sec;
    OC2R = OC1RS/2;
}
void config_ad(void)
{
    
    _ADON = 0;    // AD1CON1<15> -- Turn off A/D during config
    
    // AD1CON1 register
    _ADSIDL = 0;  // AD1CON1<13> -- A/D continues in idle mode
    _MODE12 = 1;  // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;    // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;    // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;    // AD1CON1<2> -- Auto sampling

    // AD1CON2 register
    _PVCFG = 0;   // AD1CON2<15:14> -- Use VDD as positive
                  // ref voltage
    _NVCFG = 0;   // AD1CON2<13> -- Use VSS as negative
                  // ref voltage
    _BUFREGEN = 1;// AD1CON2<11> -- Result appears in buffer
                  // location corresponding to channel
    _CSCNA = 1;   // AD1CON2<10> -- Scans inputs specified
                  // in AD1CSSx registers
    _SMPI = 9;	  // AD1CON2<6:2> -- Every 9th conversion sent (number of channels sampled -1)
                  // to buffer (if sampling 10 channels)
    _ALTS = 0;    // AD1CON2<0> -- Sample MUXA only

    _CN2PDE = 1; 
    
    // AD1CON3 register
    _ADRC = 0;    // AD1CON3<15> -- Use system clock
    _SAMC = 0;    // AD1CON3<12:8> -- Auto sample every A/D
                  // period TAD
    _ADCS = 0b00000010;    // AD1CON3<7:0> -- TAD needs to be at least 750 ns. Thus, _ADCS = 0b00000010 will give us the fastest AD clock given a 4 MHz system clock.

    // AD1CSS registers
    AD1CSSL = 0b0011111000011111; // choose A2D channels you'd like to scan here.
    
    _ADON = 1;    // AD1CON1<15> -- Turn on A/D
}

void config_PWM() {
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON2bits.SYNCSEL = 0b11111;
    OC1CON2bits.OCTRIG = 0;
    OC1RS = 1000; // Period
    OC1R = OC1RS/2; // Duty cycle
    OC1CON1bits.OCM = 0b110;
    
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON2bits.SYNCSEL = 0b11111;
    OC2CON2bits.OCTRIG = 0;
    OC2RS = 1000; // Period
    OC2R = OC2RS/2; // Duty cycle
    OC2CON1bits.OCM = 0b110;
}

void configT1() {
    T1CONbits.TCKPS = 0b01; // 8 prescaler
    PR1 = 25000; // Period of 0.05 second
    T1CONbits.TON = 1;
    
	// Configure Timer1 interrupt
	_T1IP = 4;	// Select interrupt priority
	_T1IF = 0;	// Clear interrupt flag
	_T1IE = 1;	// Enable interrupt
}

void config() {
    _RCDIV = 0;
    ANSA = 0x0000;
    ANSB = 0xf380;
    TRISB = 0xf380;
    TRISA = 0x0000;
    config_ad();
    config_PWM();
    //configT1();
}

int main(int argc, char** argv) {
    // static enum LineFollowingState {CENTERED, LEFT, RIGHT, LOST};
    // static enum LineFollowingState lineFollowingState = LOST;
    // config();
    // setRightWheelSpeed(1);
    // setLeftWheelSpeed(1);
    // while(1) {
    //     switch (lineFollowingState){
    //         case CENTERED:
    //             if (!leftQRDIsWhite()) {
    //                 lineFollowingState = LEFT;
    //                 setRightWheelSpeed(1);
    //                 setLeftWheelSpeed(2);
    //             } else if (!rightQRDIsWhite()) {
    //                 lineFollowingState = RIGHT;
    //                 setRightWheelSpeed(2);
    //                 setLeftWheelSpeed(1);
    //             }
    //             break;
    //         case LEFT:
    //             if (leftQRDIsWhite()) {
    //                 lineFollowingState = CENTERED;
    //                 setRightWheelSpeed(1);
    //                 setLeftWheelSpeed(1);
    //             } else if (!rightQRDIsWhite()) {
    //                 lineFollowingState = LOST;
    //                 setRightWheelSpeed(0);
    //                 setLeftWheelSpeed(2);
    //             }
    //             break;
    //         case RIGHT:
    //             if (rightQRDIsWhite()) {
    //                 lineFollowingState = CENTERED;
    //                 setRightWheelSpeed(1);
    //                 setLeftWheelSpeed(1);
    //             } else if (!leftQRDIsWhite()) {
    //                 lineFollowingState = LOST;
    //                 setRightWheelSpeed(2);
    //                 setLeftWheelSpeed(0);
    //             }
    //             break;
    //         case LOST:
    //             if (leftQRDIsWhite()) {
    //                 lineFollowingState = RIGHT;
    //                 setRightWheelSpeed(2);
    //                 setLeftWheelSpeed(1);
    //             } else if (!rightQRDIsWhite()) {
    //                 lineFollowingState = LEFT;
    //                 setRightWheelSpeed(1);
    //                 setLeftWheelSpeed(2);
    //             }
    //             break;
    //     }
    // }

    config_PWM();
    while(1) {}
    return (EXIT_SUCCESS);
}

