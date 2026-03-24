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
#pragma config OSCIOFNC = OFF
#pragma config SOSCSRC = DIG

/*
 * 
 */
unsigned int targetOC1RS = 0;
unsigned int targetOC2RS = 0;
unsigned int numSteps = 0;
unsigned int targetNumSteps = 0;
char t2Done = 0;
char sampleReturnAllowed = 0;

void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void)
{
    _OC2IF = 0;
    numSteps ++;
}

void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void)
{
    _OC1IF = 0;
    numSteps ++;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    _T2IF = 0; // Clear interrupt flag
    T2CONbits.TON = 0; //Turn off timer
    t2Done = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    _T3IF = 0; // Clear interrupt flag
    T3CONbits.TON = 0; //Turn off timer
    sampleReturnAllowed = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	_T1IF = 0;	// Clear interrupt flag
    static int stepSize = 100;
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

char rightWallDetected() {
    return ADC1BUF3 != 0 && ADC1BUF3 < 4095/5;
}

char leftWallDetected() {
    return ADC1BUF4 != 0 && ADC1BUF4 < 4095/5;
}

char frontWallDetected() {
    return ADC1BUF9 < 4095/7;
}

char shouldTurnLeft() {
    return ADC1BUF4 < ADC1BUF3;
}

char ballIsWhite() {
    return ADC1BUF12 < 4095/3; 
}

void setRightWheelSpeed(double rot_per_sec) {
    // targetOC1RS = 4000000/(rot_per_sec*2*200);
    if (rot_per_sec == 0) {
        OC1R = 0;
        OC1CON1bits.OCM = 0;
    } else if (rot_per_sec > 0) {
        _LATB4 = 1;
        OC1RS = 10000/rot_per_sec;
        OC1R = OC1RS/2;
        OC1CON1bits.OCM = 0b110;
    } else if (rot_per_sec < 0) {
        _LATB4 = 0;
        OC1RS = 10000/(-1*rot_per_sec);
        OC1R = OC1RS/2;
        OC1CON1bits.OCM = 0b110;
    }
}

char stepTargetReached() {
    if (numSteps >= targetNumSteps) {
        //turn off step counting interupt
        _OC1IE = 0;
        _OC2IE = 0;
        return 1;
    }
    return 0;
}

char timer2Done() {
    if (t2Done) {
        t2Done = 0;
        return 1;
    }
    return 0;
}

void setTimer(unsigned int ms) {
    PR2 = ms * 15.625;
    TMR2 = 0;
    T2CONbits.TON = 1;
}

void setAngleTarget(unsigned int degrees) {
    targetNumSteps = degrees * 10;
    numSteps = 0;
    _OC1IE = 1;
    _OC2IE = 1;
}

void setDistanceTarget(unsigned int inches) {
    targetNumSteps = inches * 150;
    numSteps = 0;
    _OC1IE = 1;
    _OC2IE = 1;
}

void setLeftWheelSpeed(double rot_per_sec) {
    //targetOC2RS = 4000000/(rot_per_sec*2*200);
    if (rot_per_sec == 0) {
        OC2R = 0;
        OC2CON1bits.OCM = 0;
    } else if (rot_per_sec > 0) {
        _LATA4 = 0;
        OC2RS = 10000/rot_per_sec;
        OC2R = OC2RS/2;
        OC2CON1bits.OCM = 0b110;
    } else if (rot_per_sec < 0) {
        _LATA4 = 1;
        OC2RS = 10000/(-1*rot_per_sec);
        OC2R = OC2RS/2;
        OC1CON1bits.OCM = 0b110;
    }
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
    OC1R = 0; // Duty cycle
    OC1CON1bits.OCM = 0b110;
    // Set up the interrupt on OC2
    _OC1IP = 3; // Select the interrupt priority
    _OC1IF = 0; // Clear the interrupt flag
    _OC1IE = 0; // Disable the OC2 interrupt
    
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON2bits.SYNCSEL = 0b11111;
    OC2CON2bits.OCTRIG = 0;
    OC2RS = 1000; // Period
    OC2R = 0; // Duty cycle
    OC2CON1bits.OCM = 0b110;
    _OC2IP = 2; // Select the interrupt priority
    _OC2IF = 0; // Clear the interrupt flag
    _OC2IE = 0; // Disable the OC2 interrupt
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

void configT2() {
    T2CONbits.TCKPS = 0b11; // 256 prescaler
    PR2 = 15625; // Period of 1 second
    T2CONbits.TON = 0;
    
	// Configure Timer1 interrupt
	_T2IP = 5;	// Select interrupt priority
	_T2IF = 0;	// Clear interrupt flag
	_T2IE = 1;	// Enable interrupt
}

void configT3() {
    T3CONbits.TCKPS = 0b11; // 256 prescaler
    PR3 = 62500; // Period of 4 seconds
    T3CONbits.TON = 1;
    
	// Configure Timer1 interrupt
	_T3IP = 5;	// Select interrupt priority
	_T3IF = 0;	// Clear interrupt flag
	_T3IE = 1;	// Enable interrupt
}

void config() {
    _RCDIV = 0;
    ANSA = 0x0;
    ANSB = 0x0;
    _ANSB14 = 1; //leftQRD
    _ANSB13 = 1; //rightQRD
    _ANSB12 = 1; //ballQRD
    _ANSB15 = 1; //front ultrasonic
    _ANSB1 = 1; //right ultrasonic
    _ANSA2 = 1; //left ultrasonic
    TRISB = 0x0;
    TRISA = 0x0;
    _TRISB14 = 1; //leftQRD
    _TRISB13 = 1; //rightQRD
    _TRISB12 = 1; //ballQRD
    _TRISB15 = 1; //front ultrasonic
    _TRISB1 = 1; //right ultrasonic
    _TRISA2 = 1; //left ultrasonic
    config_ad();
    config_PWM();
    configT3();
    configT2();
    //configT1();
}

int main(int argc, char** argv) {
    static enum State {CENTERED, LEFT, RIGHT, LOST, 
        CANYONSTRAIGHT, CANYONTURN, CANYONEXIT, TURNTOLINE, 
        RETURNWHITETURN, RETURNBLACKTURN, RETURNWHITEFORWARD, RETURNBLACKFORWARD,
        RETURNWHITEBACKWARD, RETURNBLACKBACKWARD, RETURNTURN, RETURNSTRAIGHT};
    static enum State state = LOST;
    config();
     setRightWheelSpeed(0);
     setLeftWheelSpeed(0);
//     setDistanceTarget(1);
//     while(1){
//         if(stepTargetReached()) {
//             setRightWheelSpeed(0);
//            setLeftWheelSpeed(0);
//         }
//     }
//    // setAngleTarget(90);
//    while(1){
//        if(!rightWallDetected() || !leftWallDetected()) {
//            setRightWheelSpeed(1);
//            setLeftWheelSpeed(1);
//        } else {
//            setRightWheelSpeed(0);
//            setLeftWheelSpeed(0);
//        }
//    }
//     while(!stepTargetReached()) {}  
//     setRightWheelSpeed(0);
//     setLeftWheelSpeed(0); 
     while(1) {
        switch (state){
            case CENTERED:
                if (!leftQRDIsWhite()) {
                    state = LEFT;
                    setRightWheelSpeed(2);
                    setLeftWheelSpeed(0.5);
                } else if (!rightQRDIsWhite()) {
                    state = RIGHT;
                    setRightWheelSpeed(0.5);
                    setLeftWheelSpeed(2);
                }
                else if (leftWallDetected() && rightWallDetected() && sampleReturnAllowed) {
                    setRightWheelSpeed(1);
                    setLeftWheelSpeed(1);
                    setDistanceTarget(4);
                    state = RETURNSTRAIGHT;
                }
                break;
            case LEFT:
                if (leftQRDIsWhite()) {
                    state = CENTERED;
                    setRightWheelSpeed(1);
                    setLeftWheelSpeed(1);
                } else if (!rightQRDIsWhite()) {
                    state = LOST;
                    setTimer(500);
                } 
                else if (leftWallDetected() && rightWallDetected() && sampleReturnAllowed) {
                    setRightWheelSpeed(1);
                    setLeftWheelSpeed(1);
                    setDistanceTarget(4);
                    state = RETURNSTRAIGHT;
                }
                break;
            case RIGHT:
                if (rightQRDIsWhite()) {
                    state = CENTERED;
                    setRightWheelSpeed(1);
                    setLeftWheelSpeed(1);
                } else if (!leftQRDIsWhite()) {
                    state = LOST;
                    setTimer(500);
                }
                else if (leftWallDetected() && rightWallDetected() && sampleReturnAllowed) {
                    setRightWheelSpeed(1);
                    setLeftWheelSpeed(1);
                    setDistanceTarget(4);
                    state = RETURNSTRAIGHT;
                }
                break;
            case LOST:
               if (leftQRDIsWhite()) {
                   state = RIGHT;
                   setRightWheelSpeed(0.5);
                   setLeftWheelSpeed(2);
               } else if (rightQRDIsWhite()) {
                   state = LEFT;
                   setRightWheelSpeed(2);
                   setLeftWheelSpeed(0.5);
               } 
//               else if (rightWallDetected() || leftWallDetected() || frontWallDetected()) {
//                        setRightWheelSpeed(1);
//                        setLeftWheelSpeed(1);
//                        state = CANYONSTRAIGHT;
//                }
//                else if (timer2Done()) {
//                    if (rightWallDetected() || leftWallDetected () || frontWallDetected()) {
//                        setRightWheelSpeed(1);
//                        setLeftWheelSpeed(1);
//                        state = CANYONSTRAIGHT;
//                    } else {
//                        setRightWheelSpeed(0);
//                        setLeftWheelSpeed(0);
//                    }
//                }
                break;
            case CANYONSTRAIGHT:
                if (frontWallDetected()){
                    if(shouldTurnLeft()) {
                        setRightWheelSpeed(-1);
                        setLeftWheelSpeed(1);
                        setAngleTarget(90);
                        state = CANYONTURN;
                    } else {
                        setRightWheelSpeed(1);
                        setLeftWheelSpeed(-1);
                        setAngleTarget(90);
                        state = CANYONTURN;
                    }
                } 
               else if (leftQRDIsWhite() || rightQRDIsWhite()) {
                   setDistanceTarget(1);
                   setRightWheelSpeed(1);
                   setLeftWheelSpeed(1);
                   state = CANYONEXIT;
               }
                break;
            case CANYONTURN:
                if (stepTargetReached()) {
                    setLeftWheelSpeed(1);
                    setRightWheelSpeed(1);
                    state = CANYONSTRAIGHT;
                } 
               else if (leftQRDIsWhite() || rightQRDIsWhite()) {
                   setDistanceTarget(1);
                   setRightWheelSpeed(1);
                   setLeftWheelSpeed(1);
                   state = CANYONEXIT;
               }
                break;
            case CANYONEXIT:
                if (stepTargetReached()) {
                    if(leftWallDetected()) {
                        setLeftWheelSpeed(1);
                        setRightWheelSpeed(0);
                    } else {
                        setLeftWheelSpeed(1);
                        setRightWheelSpeed(0);
                    }
                    state = TURNTOLINE;
                }
                break;
            case TURNTOLINE:
               if (leftQRDIsWhite()) {
                   state = RIGHT;
                   setRightWheelSpeed(1);
                   setLeftWheelSpeed(2);
               } else if (rightQRDIsWhite()) {
                   state = LEFT;
                   setRightWheelSpeed(2);
                   setLeftWheelSpeed(1);
               }
                break;
            case RETURNWHITETURN:
                if (stepTargetReached()) {
                    setLeftWheelSpeed(-1);
                    setRightWheelSpeed(-1);
                    setDistanceTarget(7);
                    state = RETURNWHITEFORWARD;
                }
                break;
            case RETURNBLACKTURN:
                if (stepTargetReached()) {
                    setLeftWheelSpeed(-1);
                    setRightWheelSpeed(-1);
                    setDistanceTarget(7);
                    state = RETURNBLACKFORWARD;
                }
                break;
            case RETURNWHITEFORWARD:
                if (stepTargetReached()) {
                    setLeftWheelSpeed(1);
                    setRightWheelSpeed(1);
                    setDistanceTarget(7);
                    state = RETURNWHITEBACKWARD;
                }     
                break;
            case RETURNBLACKFORWARD:
                if (stepTargetReached()) {
                    setLeftWheelSpeed(1);
                    setRightWheelSpeed(1);
                    setDistanceTarget(7);
                    state = RETURNBLACKBACKWARD;
                }
                break;
            case RETURNWHITEBACKWARD:
                if(stepTargetReached()) {
                    setLeftWheelSpeed(-1);
                    setRightWheelSpeed(1);
                    setAngleTarget(45);
                    state = RETURNTURN;
                }
                break;
            case RETURNBLACKBACKWARD:
                if(stepTargetReached()) {
                    setLeftWheelSpeed(1);
                    setRightWheelSpeed(-1);
                    setAngleTarget(45);
                    state = RETURNTURN;
                }
                break;
            case RETURNTURN:
                if(stepTargetReached()) {
                    T3CONbits.TON = 1; //Turn on timer
                    TMR3 = 0;
                    sampleReturnAllowed = 0;
                    state = TURNTOLINE;
                }
                break;
            case RETURNSTRAIGHT:
                if(stepTargetReached()) {
                    if (ballIsWhite()) {
                        setRightWheelSpeed(-1);
                        setLeftWheelSpeed(1);
                        setAngleTarget(90);
                        state = RETURNWHITETURN;
                    } else {
                        setRightWheelSpeed(1);
                        setLeftWheelSpeed(-1);
                        setAngleTarget(90);
                        state = RETURNBLACKTURN;
                    }
                }
        }
    }
    return (EXIT_SUCCESS);
}

