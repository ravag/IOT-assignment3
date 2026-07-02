#include "Arduino.h"
#include "SendMsgTask.h"

SendMsgTask :: SendMsgTask(Pot* pot) {
    msg = "";
    this->pot = pot;
}

void SendMsgTask::tick() {
    //Bisogna mettere una condizione corretta qua, non questo caso
<<<<<<< HEAD
    //Strutturare attualmente bene il messaggio
    msg = "OPEN: " + String(pot->getPercentage()) + " MODE: " + state; 
    MsgService.sendMsg(msg);
=======
    if (random(100) > 95)
    {
        //Strutturare attualmente bene il messaggio
        msg = "OPEN: " + String(random(101)) + " MODE: AUTOMATIC"; 
        MsgService.sendMsg(msg);
    }
>>>>>>> 3d2664c34cd09c847abdac999d35e50b0b38f28b
}