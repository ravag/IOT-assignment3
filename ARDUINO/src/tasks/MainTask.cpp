#include "MainTask.h"

MainTask::MainTask(ServoMotorImpl* servo, PotImpl* pot, SystemMode* mode, bool* valveState) {
    this->servo = servo;
    this->pot = pot;
    this->currentMode = mode;
    this->isServoConfiguredForManual = valveState;
}

void MainTask::tick() {
    if(*currentMode == MANUAL) {
        if(!(*isServoConfiguredForManual)) {
            servo->on();
            Serial.println("[DEBUG]: MANUAL Mode Activated");
            *isServoConfiguredForManual = true;

            int inputPercentage = pot->getPercentage();
            int angle = map(inputPercentage, 0, 100, 0, 90);
            servo->setPosition(angle);
        }

        if(pot->hasChanged()) {
            int inputPercentage = pot->getPercentage();
            int angle = map(inputPercentage, 0, 100, 0, 90);
            servo->setPosition(angle);

            Serial.print("[DEBUG]: Valve open: ");
            Serial.print(inputPercentage);
            Serial.print("%");
            Serial.println("");

            delay(50);
        }
    }
}