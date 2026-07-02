#include "Arduino.h"
#include "devices/PotImpl.h"
#include "devices/ButtonImpl.h"
#include "devices/ServoMotorImpl.h"
#include "devices/PotImpl.h"
#include "LiquidCrystal_I2C.h"
#include "config.h"
#include "tasks/MainTask.h"
#include "SystemGlobals.h"
#include "kernel/Scheduler.h"
#include "tasks/ReceiveMsgTask.h"
#include "tasks/SendMsgTask.h"

ButtonImpl* button;
ServoMotorImpl* servo;
PotImpl* pot;
LiquidCrystal_I2C* lcd;
Scheduler sched;

bool isServoConfiguredForManual = false;

void setup() {
    Serial.begin(115200);

    Serial.println("[DEBUG]: Dispositivi in preparazione");

    button = new ButtonImpl(BUTTON_PIN);
    servo = new ServoMotorImpl(SERVO_PIN);
    pot = new PotImpl(POT_PIN);
    lcd = new LiquidCrystal_I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);

    lcd->init();
    lcd->backlight();
    lcd->clear();

    Task* mainTask = new MainTask(lcd, servo, pot, &isServoConfiguredForManual);
    mainTask->init(500);
    Task* send = new SendMsgTask(pot);
    send->init(250);
    Task* recv = new ReceiveMsgTask();
    recv->init();
    sched.addTask(recv);
    sched.addTask(mainTask);
    sched.addTask(send);
    sched.init(250);

    Serial.println("[DEBUG]: -- Serial Initialized correctly -- READY");
}

void loop() {
    sched.schedule();
}
