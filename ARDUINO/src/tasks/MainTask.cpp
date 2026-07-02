#include "MainTask.h"

MainTask::MainTask(ServoMotorImpl* servo, PotImpl* pot, bool* isManualInit) {
    this->servo = servo;
    this->pot = pot;
    this->isServoConfiguredForManual = isManualInit;

    this->currentAngle = 0;
    this->startAngle = 0;
    this->startAngle = 0;
    this->timeInState = 0;
    this->timeToMove = 0;
}

void MainTask::tick() {
    if(state == MANUAL) {
        if(!(*isServoConfiguredForManual)) {
            servo->on();
            Serial.println("[DEBUG]: MANUAL Mode Activated");
            *isServoConfiguredForManual = true;

            int potPercentage = pot->getPercentage();
            currentAngle = map(potPercentage, 0, 100, 0, 90);
            targetAngle = currentAngle;
            startAngle = currentAngle;
            timeInState = millis();
            timeToMove = 0;
        }

        int potPercentage = pot->getPercentage();
        int newTarget = map(potPercentage, 0, 100, 0, 90);

        //Ho messo che si sposta solo se il potenziometro si sposta di almeno 2 gradi per evitare micro flicker analogici
        if(abs(newTarget - targetAngle) >= 2) {
            startAngle = currentAngle;
            targetAngle = newTarget;
            timeInState = millis();

            unsigned long degreesToTravel = abs(targetAngle - currentAngle);
            timeToMove = degreesToTravel * MS_PER_DEGREE;

            //Evito divisioni per 0 se il movimento è per qualche motivo minore a 1
            if(timeToMove == 0) {
                timeToMove = 1;
            }
        }

        unsigned long dt = millis() - timeInState;

        if(dt <= timeToMove && currentAngle != targetAngle) {
            float progress = (float)dt /timeToMove;

            currentAngle = startAngle + (progress * (targetAngle - startAngle));
            servo->setPosition(currentAngle);

            Serial.print("[DEBUG]: Target: ");
            Serial.print(targetAngle);
            Serial.print("° | Current Angle: ");
            Serial.print(currentAngle);
            Serial.print("°");
            Serial.println("");

        } else if(currentAngle != targetAngle) {
            currentAngle = targetAngle;
            servo->setPosition(currentAngle);
        }
    }
}