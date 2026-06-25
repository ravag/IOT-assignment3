#ifndef __MAIN_TASK__
#define __MAIN_TASK__

//Questa task deve vedere se la mode è manuale e se lo è deve far girare il motorino con il potenziometro
#include <Arduino.h>
#include "devices/PotImpl.h"
#include "devices/ServoMotorImpl.h"

enum SystemMode { AUTOMATIC, MANUAL, UNCONNECTED };

class MainTask {
    private:
        ServoMotorImpl* servo;
        PotImpl* pot;
        SystemMode* currentMode;
        bool* isServoConfiguredForManual;
    
    public:
        MainTask(ServoMotorImpl* servo, PotImpl* pot, SystemMode* mode, bool* valveState);

        void tick();
};


#endif
