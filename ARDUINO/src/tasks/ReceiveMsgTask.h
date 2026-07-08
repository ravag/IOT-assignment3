#ifndef __RECEIVETASK__
#define __RECEIVETASK__

#include "kernel\Task.h"
#include "kernel\MsgService.h"
#include "SystemGlobals.h"

class ReceiveMsgTask: public Task {

public:
    ReceiveMsgTask();
    void tick();

private:
    
    String msg;
    int oldSentTarget;
};

extern int targetOpening;

#endif
