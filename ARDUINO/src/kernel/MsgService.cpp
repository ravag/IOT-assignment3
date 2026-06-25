#include "Arduino.h"
#include "MsgService.h"

String content;

MsgServiceClass MsgService;

void MsgServiceClass::init(){
  Serial.begin(115200);
  content.reserve(256);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;  
}

bool MsgServiceClass::isMsgAvailable(){
  return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
  if (msgAvailable){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceClass::sendMsg(const String& msg){
  Serial.println(msg);  
}

void serialEvent() {
  /* reading the content */
  while (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();
    MsgService.currentMsg = new Msg(content);
    MsgService.msgAvailable = true;      
  }
}
