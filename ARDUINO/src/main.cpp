#include "Arduino.h"
#include "devices/PotImpl.h"
#include "devices/ButtonImpl.h"
#include "devices/ServoMotorImpl.h"
#include "devices/PotImpl.h"
#include "LiquidCrystal_I2C.h"
#include "config.h"
#include "tasks/MainTask.h"

ButtonImpl* button;
ServoMotorImpl* servo;
PotImpl* pot;
LiquidCrystal_I2C* lcd;
MainTask* mainTask;

SystemMode currentMode = AUTOMATIC;
bool isServoConfiguredForManual = false;

void setup() {
    Serial.begin(115200);

    Serial.println("[DEBUG]: Dispositivi in preparazione");

    button = new ButtonImpl(BUTTON_PIN);
    servo = new ServoMotorImpl(SERVO_PIN);
    pot = new PotImpl(POT_PIN);
    lcd = new LiquidCrystal_I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);

    mainTask = new MainTask(lcd, servo, pot, &currentMode, &isServoConfiguredForManual);

    Serial.println("[DEBUG]: Provo inizializzazione LCD...");

    lcd->init();
    lcd->backlight();
    lcd->clear();
    
    Serial.println("[DEBUG]: -- Serial Initialized correctly -- READY");
}

void loop() {
//Controllo l'input locale del pulsante fisico
    if(currentMode != UNCONNECTED && button->isPressed()) {
        Serial.println("[DEBUG]: Pressed local button");
        button->resetButton();
        
        currentMode = (currentMode == AUTOMATIC) ? MANUAL : AUTOMATIC;
    } 

    mainTask->tick();

    //Logica degli stati hardware
    switch(currentMode) {

        case MANUAL:
        break;

        case AUTOMATIC:
            if(servo->isOn() || isServoConfiguredForManual) {
                servo->setPosition(0);
                Serial.println("[DEBUG]: Valve closed");
                delay(300);
                servo->off(); // per debug in realtà il servo non si spegne mai finché è acceso
                Serial.println("[DEBUG]: AUTOMATIC Mode Activated");
                isServoConfiguredForManual = false;
            }
            break;

        case UNCONNECTED:
            Serial.println("[DEBUG]: Connection Lost! Switching to MANUAL Mode");
            currentMode = MANUAL;

            isServoConfiguredForManual = false;

            break;
    }

    delay(20);
}

