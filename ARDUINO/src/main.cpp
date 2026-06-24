#include "Arduino.h"
#include "devices/PotImpl.h"
#include "devices/ButtonImpl.h"
#include "devices/ServoMotorImpl.h"
#include "devices/PotImpl.h"

const int BUTTON_PIN = 3;
const int SERVO_PIN = 6;
const int POT_PIN = A0;

ButtonImpl* button;
ServoMotorImpl* servo;
PotImpl* pot;

bool isValveOn = false;
bool oldValveState = false;

void setup() {
    Serial.begin(9600);

    button = new ButtonImpl(BUTTON_PIN);
    servo = new ServoMotorImpl(SERVO_PIN);
    pot = new PotImpl(POT_PIN);

    Serial.println("-- Serial Initialized correctly --");
}

void loop() {
    if(button->isPressed()) {
        Serial.println("Premuto");
        button->resetButton();
        isValveOn = !isValveOn;
    } 

    if (isValveOn) {
        if(!oldValveState) {
            servo->on();
            int inputPercentage = pot->getPercentage();
            int angle = map(inputPercentage, 0, 100, 0, 90);
            servo->setPosition(angle);
            Serial.println("Modalità MANUALE Attiva");
            oldValveState = true;

        }
            
        if(pot->hasChanged()) {
            Serial.print("Valvola aperta al: ");
            Serial.print(pot->getPercentage());
            Serial.print("%");
            Serial.println("");
            int inputPercentage = pot->getPercentage();
            int angle = map(inputPercentage, 0, 100, 0, 90);
            servo->setPosition(angle);
            delay(50);
            
        } 
    } else {
        if(servo->isOn()) {
            servo->setPosition(0);
            Serial.println("Valvola riposizionata in posizione 0");
            delay(300);
            servo->off();
            Serial.println("Modalità MANUALE Disattivata\nModalità AUTOMATICA Attivata");
            oldValveState = false;
        }
    }
    delay(20);
}