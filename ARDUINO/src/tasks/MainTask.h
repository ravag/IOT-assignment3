#ifndef __MAIN_TASK__
#define __MAIN_TASK__

//Questa task deve vedere se la mode è manuale e se lo è deve far girare il motorino con il potenziometro
#include <Arduino.h>
#include "devices/PotImpl.h"
#include "devices/ServoMotorImpl.h"
#include "SystemGlobals.h"
#include "kernel/Task.h"

class MainTask: public Task {
    private:
        ServoMotorImpl* servo;
        PotImpl* pot;
        bool* isServoConfiguredForManual;

        unsigned long timeInState;
        int currentAngle;
        int startAngle;

        unsigned long timeToMove;
        const unsigned long MS_PER_DEGREE = 55;
    
    public:
        MainTask(ServoMotorImpl* servo, PotImpl* pot, bool* valveState);

        void tick();
};


#endif
