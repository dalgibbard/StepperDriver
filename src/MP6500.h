/*
 * MP6500 - 2A Stepper Motor Driver with Digital Current Control
 *
 * Copyright (C)2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#ifndef MP6500_H
#define MP6500_H
#include <Arduino.h>
#include "BasicStepperDriver.h"

class MP6500 : public BasicStepperDriver {
protected:
    short m0 = PIN_UNCONNECTED;
    short m1 = PIN_UNCONNECTED;
    short trq0 = PIN_UNCONNECTED;
    short trq1 = PIN_UNCONNECTED;
    // tWH(STEP) pulse duration, STEP high, min value
    static const int step_high_min = 0;   // 0.47us
    // tWL(STEP) pulse duration, STEP low, min value
    static const int step_low_min = 0;    // 0.47us
    // tWAKE wakeup time, nSLEEP inactive to STEP
    static const int wakeup_time = 1500;
    // also 200ns between ENBL/DIR/Mx changes and STEP HIGH

    // Get max microsteps supported by the device
    short getMaxMicrostep() override;

private:
    // microstep range (1, 16, 32 etc)
    float amps = 0.5;
    static const short MAX_MICROSTEP = 8;

public:
    /*
    Fully wired with digital current control
    Assumes I2 is tied low in hardware.
    Current is set according to 5v hardware, PWM set on I2 (trq0)
    */
    MP6500(short steps, short dir_pin, short step_pin, short m0, short m1, short trq0);
    // alternative with i2 wired too, just gets pulled low though.
    MP6500(short steps, short dir_pin, short step_pin, short m0, short m1, short trq0, short trq1);
 
    void begin(short rpm=60, short microsteps=1);

    short setMicrostep(short microsteps) override;

    /*
     * Torque DAC Control
     * current percent value must be 25, 50, 75 or 100.
     */
    void setCurrent(float amps);
};
#endif // MP6500_H
