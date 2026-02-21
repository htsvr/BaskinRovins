/* 
 * File:   newmain.c
 * Author: htsvr
 *
 * Created on February 17, 2026, 11:43 AM
 */
#pragma config FNOSC = LPFRC
//#pragma config ICS = PGx3

#include <stdio.h>
#include <stdlib.h>
#include "xc.h"

enum State {
    FORWARD,
    TURN
}

/*
 * 
 */
int main(int argc, char** argv) {
    static enum State state = FORWARD;
    setTimer(1);

    while(1) {
        switch (state) {
            case FORWARD:
                if(timerFinished()) {
                    state = TURN;
                    setLeftMotor(FORWARD, 100);
                    setRightMotor(FORWARD, 0);
                    setLeftSteps(90);
                }
                break;
            case TURN:
           
                break;
            default:     if(angleReached()) {
                    state = FORWARD;
                }

        }
    }
    
    return (EXIT_SUCCESS);
}

