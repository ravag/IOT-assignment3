#ifndef __MAIN_TASK__
#define __MAIN_TASK__

//Questa task deve vedere se la mode è manuale e se lo è deve far girare il motorino con il potenziometro
#include <Arduino.h>
#include "devices/PotImpl.h"
#include "devices/ServoMotorImpl.h"
#include "SystemGlobals.h"
#include "kernel/Task.h"
#include "LiquidCrystal_I2C.h"

class MainTask: public Task {
    private:
        LiquidCrystal_I2C* lcd;
        ServoMotorImpl* servo;
        PotImpl* pot;

        unsigned long timeInState;
        int currentAngle;
        int startAngle;
        int oldAngle;

        unsigned long timeToMove;
        const unsigned long MS_PER_DEGREE = 55;

        void updateLCD();
    
    public:
        MainTask(LiquidCrystal_I2C* lcd, ServoMotorImpl* servo, PotImpl* pot, bool* valveState);

        void tick();
};


#endif
