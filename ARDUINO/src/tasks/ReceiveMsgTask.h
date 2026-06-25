#ifndef __RECEIVETASK__
#define __RECEIVETASK__

#include "kernel\Task.h"
#include "kernel\MsgService.h"

enum State {AUTOMATIC,MANUAL,UNCONNECTED};

class ReceiveMsgTask: public Task {

public:
    ReceiveMsgTask();
    void tick();

private:
    
    String msg;
    State state;
};

extern int targetOpening;

#endif
