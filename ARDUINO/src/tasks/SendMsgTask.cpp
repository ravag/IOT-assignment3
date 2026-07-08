#include "Arduino.h"
#include "SendMsgTask.h"

SendMsgTask :: SendMsgTask(ServoMotorImpl* servo) {
    msg = "";
    this->servo = servo;
}

void SendMsgTask::tick() {
    //Bisogna mettere una condizione corretta qua, non questo caso
    //Strutturare attualmente bene il messaggio
    status = state == AUTOMATIC ? "AUTOMATIC" : state == MANUAL ? "MANUAL" : "UNCONNECTED";
    change = hasChangedMode ? "true" : "false";
    msg = "OPEN: " + String(map(servo->getPosition(),0,90,0,100)) + ",MODE: " + status + ",CHANGE: " + change; 
    hasChangedMode = change == "true" ? false : hasChangedMode;
    MsgService.sendMsg(msg);
}