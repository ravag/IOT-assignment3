#include "Arduino.h"
#include "devices/PotImpl.h"
#include "devices/ButtonImpl.h"
#include "devices/ServoMotorImpl.h"
#include "devices/PotImpl.h"
#include "LiquidCrystal_I2C.h"
#include "config.h"

ButtonImpl* button;
ServoMotorImpl* servo;
PotImpl* pot;
LiquidCrystal_I2C* lcd;

bool isValveOn = false;
bool oldValveState = false;

void updateLCD();

void setup() {
    Serial.begin(9600);

    button = new ButtonImpl(BUTTON_PIN);
    servo = new ServoMotorImpl(SERVO_PIN);
    pot = new PotImpl(POT_PIN);
    lcd = new LiquidCrystal_I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);

    lcd->init();
    lcd->backlight();
    lcd->clear();
    
    updateLCD();

    Serial.println("-- Serial Initialized correctly --");
}

void loop() {
    if(button->isPressed()) {
        Serial.println("Premuto");
        button->resetButton();
        isValveOn = !isValveOn;

        lcd->clear();
        updateLCD();
    } 

    if (isValveOn) {
        if(!oldValveState) {
            servo->on();
            int inputPercentage = pot->getPercentage();
            int angle = map(inputPercentage, 0, 100, 0, 90);
            servo->setPosition(angle);
            Serial.println("Modalità MANUALE Attiva");
            oldValveState = true;
            lcd->clear();
            updateLCD();

        }
            
        if(pot->hasChanged()) {
            Serial.print("Valvola aperta al: ");
            Serial.print(pot->getPercentage());
            Serial.print("%");
            Serial.println("");
            int inputPercentage = pot->getPercentage();
            int angle = map(inputPercentage, 0, 100, 0, 90);
            servo->setPosition(angle);
            updateLCD();
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

            lcd->clear();
            updateLCD();
        }
    }
    delay(20);
}

void updateLCD() {
    lcd->setCursor(0, 0);
    if(isValveOn) {
        lcd->print("Mode: MANUAL       ");
    } else {
        lcd->print("Mode: AUTOMATIC    ");
    }

    lcd->setCursor(0, 1);
    lcd->print("Value: ");

    if(isValveOn) {
        lcd->print(pot->getPercentage());
    } else {
        if(servo->isOn()) {
            lcd->print("Apertura automatica salvata in una variabile in futuro");
        } else {
            lcd->print("0");
        }
    }
    lcd->print("%   ");
}