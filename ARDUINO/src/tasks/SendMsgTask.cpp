#include "Arduino.h"
#include "SendMsgTask.h"

SendMsgTask :: SendMsgTask() {
    msg = "";
}

void SendMsgTask::tick() {
    //Bisogna mettere una condizione corretta qua, non questo caso
    if (random(100) > 95)
    {
        //Strutturare attualmente bene il messaggio
        msg = "OPEN: " + String(random(101)) + " MODE: AUTOMATIC"; 
        MsgService.sendMsg(msg);
    }
    
}