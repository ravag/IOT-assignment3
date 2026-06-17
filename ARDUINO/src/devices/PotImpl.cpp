#include "PotImpl.h"
#include "Arduino.h"

PotImpl::PotImpl(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
    this->lastValue = analogRead(pin);
}

int PotImpl::getValue() {
    return analogRead(this->pin);
}

int PotImpl::getPercentage() {
    int value = analogRead(this->pin);
    return map(value, 0, 1023, 0, 100);
}

bool PotImpl::hasChanged() {
    int currentValue = analogRead(this->pin);
    int tolerance = 5;

    if (abs(currentValue - this->lastValue) > tolerance) {
        this->lastValue = currentValue;
        return true;
    }

    return false;
}