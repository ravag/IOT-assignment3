#include <Arduino.h>
#include "tasks\ReceiveMsgTask.h"



ReceiveMsgTask::ReceiveMsgTask() {
    msg = "";   
    state = AUTOMATIC;
}

void ReceiveMsgTask::tick() {
    if (MsgService.isMsgAvailable())
    {
        msg = MsgService.receiveMsg()->getContent();
        //Msg format: "MODE: mode, OPEN: open"
        int i = 6;
        char ch = msg[i];
        String content = "";
        while (ch != ',')
        {
            content += ch;
            ch = msg[++i];
        }
        if (content == "AUTOMATIC")
        {
            state = AUTOMATIC;
        } else if (content == "MANUAL")
        {
           state = MANUAL;
        } else {
            state = UNCONNECTED;
        }
        i += 7;
        content += " ";
        while (i < msg.length())
        {
            ch = msg[i];
            content += ch;
            i++;
        }
        int n = content.toInt();
        //Istruzione di debug, da rimuovere
        //MsgService.sendMsg(content);
    }
}