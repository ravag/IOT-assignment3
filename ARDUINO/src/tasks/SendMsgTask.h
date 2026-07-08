#ifndef __SENDMSGTASK__
#define __SENDMSGTASK__

#include "kernel\MsgService.h"
#include "kernel\Task.h"
#include "devices\ServoMotorImpl.h"
#include "SystemGlobals.h"

class SendMsgTask : public Task {
public:
    SendMsgTask(ServoMotorImpl* servo);
    void tick();

private:
    String msg;
    String status;
    ServoMotorImpl* servo;
};


#endif