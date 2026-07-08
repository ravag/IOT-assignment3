#include "Arduino.h"
#include "MsgService.h"

String content;

MsgServiceClass MsgService;

void MsgServiceClass::init(){
  Serial.begin(115200);
  content.reserve(256);
  content = "";
  currentMsg = "";
  msgAvailable = false;  
}

bool MsgServiceClass::isMsgAvailable(){
  return msgAvailable;
}

String MsgServiceClass::receiveMsg(){
  if (msgAvailable){
    String msg = currentMsg;
    msgAvailable = false;
    currentMsg = "";
    content = "";
    return msg;  
  } else {
    return ""; 
  }
}

void MsgServiceClass::sendMsg(const String& msg){
  Serial.println(msg);  
}

void serialEvent() {
  /* reading the content */
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      MsgService.currentMsg = content;
      MsgService.msgAvailable = true;      
    } else {
      content += ch;      
    }    
  }
}
