#ifndef __MAIN_TASK__
#define __MAIN_TASK__

//Questa task deve vedere se la mode è manuale e se lo è deve far girare il motorino con il potenziometro
#include <Arduino.h>
#include "devices/PotImpl.h"
#include "devices/ServoMotorImpl.h"
#include "LiquidCrystal_I2C.h"

enum SystemMode { AUTOMATIC, MANUAL, UNCONNECTED };

class MainTask {
    private:
        LiquidCrystal_I2C* lcd;
        ServoMotorImpl* servo;
        PotImpl* pot;
        SystemMode* currentMode;
        bool* isServoConfiguredForManual;

        unsigned long timeInState;
        int currentAngle;
        int startAngle;
        int targetAngle;

        unsigned long timeToMove;
        const unsigned long MS_PER_DEGREE = 55;

        void updateLCD();
    
    public:
        MainTask(LiquidCrystal_I2C* lcd, ServoMotorImpl* servo, PotImpl* pot, SystemMode* mode, bool* valveState);

        void tick();
};


#endif
