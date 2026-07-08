#include "SwitchTask.h"

SwitchTask::SwitchTask(ButtonImpl* button) {
    this->button = button;
}

void SwitchTask::tick() {
    if (button->isPressed()) {
        state = state == AUTOMATIC ? MANUAL : state == UNCONNECTED ? UNCONNECTED : AUTOMATIC;
        hasChangedMode = true;
        button->resetButton();
    } 
}