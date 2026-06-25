#include "Arduino.h"
#include "devices/PotImpl.h"
#include "devices/ButtonImpl.h"
#include "devices/ServoMotorImpl.h"
#include "devices/PotImpl.h"
#include "LiquidCrystal_I2C.h"
#include "config.h"
#include "tasks/MainTask.h"


/*
void LandingTask::openDoor(){
    long dt = millis() - timeInState;
    currentPosition = dt / TIME_TO_OPEN (=5000 );
    pMotor->setPosition(currentPosition * 90);

    if(pMotor->getPosition() >= 90){
        pMotor->off();
    }
}
*/



ButtonImpl* button;
ServoMotorImpl* servo;
PotImpl* pot;
LiquidCrystal_I2C* lcd;
MainTask* mainTask;

SystemMode currentMode = AUTOMATIC;
bool isServoConfiguredForManual = false;

void updateLCD();
void checkSerialIncoming();
void sendDataToCUS();

void setup() {
    Serial.begin(115200);

    button = new ButtonImpl(BUTTON_PIN);
    servo = new ServoMotorImpl(SERVO_PIN);
    pot = new PotImpl(POT_PIN);
    lcd = new LiquidCrystal_I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);

    mainTask = new MainTask(servo, pot, &currentMode, &isServoConfiguredForManual);

    lcd->init();
    lcd->backlight();
    lcd->clear();
    
    updateLCD();
    sendDataToCUS();

    Serial.println("[DEBUG]: -- Serial Initialized correctly --");
}

void loop() {
    //Controllo se sono arrivati messaggi dal CUS
    checkSerialIncoming();

    //Controllo l'input locale del pulsante fisico
    if(currentMode != UNCONNECTED && button->isPressed()) {
        Serial.println("[DEBUG]: Pressed local button");
        button->resetButton();
        
        currentMode = (currentMode == AUTOMATIC) ? MANUAL : AUTOMATIC;

        lcd->clear();
        updateLCD();
        sendDataToCUS();
    } 

    mainTask->tick();

    //Logica degli stati hardware
    switch(currentMode) {
        
        case MANUAL:
            if(pot->hasChanged()) {
                lcd->clear();
                updateLCD();
                sendDataToCUS();
            } 

            break;

        case AUTOMATIC:
            if(servo->isOn() || isServoConfiguredForManual) {
                servo->setPosition(0);
                Serial.println("[DEBUG]: Valve closed");
                delay(300);
                servo->off(); // per debug in realtà il servo non si spegne mai finché è acceso
                Serial.println("[DEBUG]: AUTOMATIC Mode Activated");
                isServoConfiguredForManual = false;

                lcd->clear();
                updateLCD();
                sendDataToCUS();
            }
            break;

        case UNCONNECTED:
            Serial.println("[DEBUG]: Connection Lost! Switching to MANUAL Mode");
            currentMode = MANUAL;

            isServoConfiguredForManual = false;

            lcd->clear();
            updateLCD();
            sendDataToCUS();
            break;
    }

    delay(20);
}

void updateLCD() {
    lcd->setCursor(0, 0);
    if(currentMode == MANUAL) {
        lcd->print("Mode: MANUAL       ");
    } else if(currentMode == AUTOMATIC) {
        lcd->print("Mode: AUTOMATIC    ");
    } else if(currentMode == UNCONNECTED) {
        lcd->print("Mode: UNCONNECTED  ");
    }

    lcd->setCursor(0, 1);
    lcd->print("Value: ");

    if(currentMode == MANUAL) {
        lcd->print(pot->getPercentage());
    } else if(currentMode == AUTOMATIC) {
        if(servo->isOn()) {
            lcd->print("AUTOMATIC Mode Active");
        } else {
            lcd->print("0%    ");
        }
    } else if(currentMode == UNCONNECTED) {
        lcd->print("Switching...");
    }
}

void checkSerialIncoming() {
    if(Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        msg.trim();

        int modeIndex = msg.indexOf("MODE:");

        if(modeIndex != -1) {
            char modeChar = msg.charAt(modeIndex + 6);

            if(modeChar == 'M' && currentMode != MANUAL) {
                currentMode = MANUAL;
                Serial.println("[DEBUG]: Switch to MANUAL from Serial");
                lcd->clear();
                updateLCD();
                sendDataToCUS();
            } else if(modeChar == 'A' && currentMode != AUTOMATIC) {
                currentMode = AUTOMATIC;
                Serial.println("[DEBUG]: Switch to AUTOMATIC from Serial");
                lcd->clear();
                updateLCD();
                sendDataToCUS();
            } else if(modeChar == 'U') {
                currentMode = UNCONNECTED;
                Serial.println("[DEBUG]: Switch to UNCONNECTED from Serial");
                lcd->clear();
                updateLCD();
                sendDataToCUS();
            }
        }
    }
}

void sendDataToCUS() {
    String modeString = "";
    int currentOpening = 0;

    if(currentMode == MANUAL) {
        modeString = "MANUAL";
        currentOpening = pot->getPercentage();
    } else if(currentMode == AUTOMATIC) {
        modeString = "AUTOMATIC";
        currentOpening = 0;
    } else {
        modeString = "UNCONNECTED";
        currentOpening = 0;
    }

    Serial.print("MODE: ");
    Serial.println(modeString);

    Serial.print("OPEN: ");
    Serial.print(currentOpening);
    Serial.println("%");
}