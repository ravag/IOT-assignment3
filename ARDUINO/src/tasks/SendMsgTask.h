#ifndef __SENDMSGTASK__
#define __SENDMSGTASK__

#include "kernel\MsgService.h"
#include "kernel\Task.h"

class SendMsgTask : public Task {
public:
    SendMsgTask();
    void tick();

private:

    String msg;
};


#endif