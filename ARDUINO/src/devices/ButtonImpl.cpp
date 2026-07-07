#include "ButtonImpl.h"
#include "Arduino.h"

//serve per far in modo di poter chiamare la funzione buttonPressed() in call()
ButtonImpl* instance = nullptr;

void call();

ButtonImpl::ButtonImpl(int pin){
  this->pin = pin;
  this->button = false;
  pinMode(pin, INPUT_PULLUP);
  instance = this;
  attachInterrupt(digitalPinToInterrupt(pin), &call, FALLING);
} 

void ButtonImpl::resetButton(){
  button = false;
}
  
bool ButtonImpl::isPressed(){
  return button;
}

void call(){
  if (instance != nullptr) {
    instance ->buttonPressed();
  }
}

void ButtonImpl::buttonPressed(){
  button = true;
}