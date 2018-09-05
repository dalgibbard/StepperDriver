/*
 * MP6500 - 2.2A Stepper Motor Driver with Digital Current Selection
 *   Code based on 5v only! Tune the mapping at the end of the file for 3v.
 * 
 * Copyright (C)2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include "MP6500.h"

MP6500::MP6500(short steps, short dir_pin, short step_pin, short m0, short m1, short trq0)
:BasicStepperDriver(steps, dir_pin, step_pin), m0(m0), m1(m1), trq0(trq0)
{}

MP6500::MP6500(short steps, short dir_pin, short step_pin, short m0, short m1, short trq0, short trq1)
:BasicStepperDriver(steps, dir_pin, step_pin), m0(m0), m1(m1), trq0(trq0), trq1(trq1)
{}

void MP6500::begin(short rpm, short microsteps){
    BasicStepperDriver::begin(rpm, microsteps);
    setCurrent(0.5);
}

short MP6500::getMaxMicrostep(){
    return MP6500::MAX_MICROSTEP;
}

/*
 * Set microstepping mode (1:divisor)
 * Allowed ranges for MP6500 are 1:1 to 1:8
 * If the control pins are not connected, we recalculate the timing only
 */
short MP6500::setMicrostep(short microsteps){
    BasicStepperDriver::setMicrostep(microsteps);

    if (!IS_CONNECTED(m0) || !IS_CONNECTED(m1)){
        return this->microsteps;
    }

    /*
     * Step mode truth table
     * M0 M1    step mode
     *  0  0     1
     *  1  0     2
     *  0  1     4
     *  1  1     8
     *
     */

    pinMode(m0, OUTPUT);
    pinMode(m1, OUTPUT);
    switch(this->microsteps){
        case 1:
            digitalWrite(m0, LOW);
            digitalWrite(m1, LOW);
            break;
        case 2:
            digitalWrite(m0, HIGH);
            digitalWrite(m1, LOW);
            break;
        case 4:
            digitalWrite(m0, LOW);
            digitalWrite(m1, HIGH);
            break;
        case 8:
            digitalWrite(m0, LOW);
            digitalWrite(m1, );
            break;
    }
    return this->microsteps;
}

void MP6500::setCurrent(float amps){
    if (!IS_CONNECTED(trq0)){
        return;
    }
    if (IS_CONNECTED(trq1)){
        // If trq1 is defined, push it low to allow for PWM control on I1
        pinMode(trq1, OUTPUT);
        pinMode(trq1, LOW); 
    }
    pinMode(trq0, OUTPUT);
    // map PWM 0% = 2.2amps, PWM 70%+ = 0 amps (on 5v boards- on 3.3v boards, PWM for 0amps is 100%)
    //      0% = write(0)           (255/100)*70 = write(178.5+) // opted for 180 here.
    int bigamps = amps * 100 // gives better resolution in the mapping.
    int pwmcycle = map(bigamps, 0, 220, 180, 0); //220 is 2.2amps (max) * 100 to match input for better scaling.
    analogWrite(trq0, pwmcycle);
}
