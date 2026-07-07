#ifndef __SWITCHTASK__
#define __SWITCHTASK__

#include "kernel/Task.h"
#include "devices/ButtonImpl.h"
#include "SystemGlobals.h"

class SwitchTask: public Task {
    private:
        ButtonImpl* button;
    public:
        SwitchTask(ButtonImpl* button);
        void tick();
};

#endif