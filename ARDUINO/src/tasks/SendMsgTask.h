#ifndef __SENDMSGTASK__
#define __SENDMSGTASK__

#include "kernel\MsgService.h"
#include "kernel\Task.h"
#include "devices\Pot.h"
#include "SystemGlobals.h"

class SendMsgTask : public Task {
public:
    SendMsgTask(Pot* pot);
    void tick();

private:
    String msg;
    Pot* pot;
};


#endif