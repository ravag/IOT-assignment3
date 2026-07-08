#include "MainTask.h"

MainTask::MainTask(LiquidCrystal_I2C* lcd, ServoMotorImpl* servo, PotImpl* pot) {
    this->lcd = lcd;
    this->servo = servo;
    this->pot = pot;

    this->currentAngle = 0;
    this->startAngle = 0;
    this->timeInState = 0;
    this->timeToMove = 0;
    this->oldAngle = 0;
    /* this->servo->on(); */
}

void MainTask::tick() {
    if (state != AUTOMATIC) {

        //valori settati se in MANUALE, leggo il target dal potenziometro
        if (this->pot->hasChanged()) {
            int potPercentage = pot->getPercentage();
            targetAngle = map(potPercentage, 0, 100, 0, 90);
            oldAngle = targetAngle;
            timeInState = millis();
            startAngle = servo->getPosition();

            unsigned long degreesToTravel = abs(targetAngle - startAngle);
            timeToMove = degreesToTravel * MS_PER_DEGREE;

            timeToMove = timeToMove == 0 ? 1 : timeToMove;
        } else if(hasReceivedOpening && state != UNCONNECTED) {    //Se il potenziometro non è variato provo a vedere se mi hanno mandato un'apertura
            hasReceivedOpening = false;
            targetAngle = sentTarget;
            timeInState = millis();
            startAngle = servo->getPosition();

            unsigned long degreesToTravel = abs(targetAngle - startAngle);
            timeToMove = degreesToTravel * MS_PER_DEGREE;

            timeToMove = timeToMove == 0 ? 1 : timeToMove;
        }
    } else {        //valori settati se in AUTOMATICO, il target lo prendo da quello passato tramite messaggio, aggiorno l'oldAngle tramite il vecchio targetAngle
        if (abs(oldAngle - targetAngle) >= 2) {
            oldAngle = targetAngle;
            timeInState = millis();
            startAngle = servo->getPosition();

            unsigned long degreesToTravel = abs(targetAngle - startAngle);
            timeToMove = degreesToTravel * MS_PER_DEGREE;

            timeToMove = timeToMove == 0 ? 1 : timeToMove;
        }
    }
    unsigned long dt = millis() - timeInState;

    //muovo il servo solo se ho ancora tempo e l'angolo di target è diverso da quello a cui è adesso il servo
    if(dt <= timeToMove && servo->getPosition() != targetAngle) {
        float progress = (float)dt /timeToMove;

        currentAngle = startAngle + (progress * (targetAngle - startAngle));
        servo->setPosition(currentAngle);

    } else if(currentAngle != targetAngle) {        //muovo il servo fino alla fine dato che non ho piu tempo
        currentAngle = targetAngle;
        servo->setPosition(currentAngle);
    }
    updateLCD();
}

void MainTask::updateLCD() {
    lcd->clear();
    lcd->setCursor(0, 0);
    if(state == MANUAL) {
        lcd->print("Mode: MANUAL       ");
    } else if(state == AUTOMATIC) {
        lcd->print("Mode: AUTOMATIC    ");
    } else if (state == UNCONNECTED) {
        lcd->print("Mode: UNCONNECTED  ");
    }

    lcd->setCursor(0, 2);
    lcd->print("Value: ");
    lcd->print(map(servo->getPosition(), 0, 90, 0, 100));
    lcd->print("%");
}
