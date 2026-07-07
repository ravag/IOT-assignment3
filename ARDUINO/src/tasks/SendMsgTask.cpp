#include "Arduino.h"
#include "SendMsgTask.h"

SendMsgTask :: SendMsgTask(Pot* pot) {
    msg = "";
    this->pot = pot;
}

void SendMsgTask::tick() {
    //Bisogna mettere una condizione corretta qua, non questo caso
    //Strutturare attualmente bene il messaggio
    msg = "OPEN: " + String(pot->getPercentage()) + " MODE: " + state; 
    MsgService.sendMsg(msg);
}