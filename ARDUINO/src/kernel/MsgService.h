#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"

class MsgServiceClass {
    
public: 
  
  String currentMsg;
  bool msgAvailable;

  void init();  

  bool isMsgAvailable();
  String receiveMsg();
  
  void sendMsg(const String& msg);
};

extern MsgServiceClass MsgService;

#endif