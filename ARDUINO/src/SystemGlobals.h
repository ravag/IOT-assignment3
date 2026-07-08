#ifndef __SYSTEMSTATUS__
#define __SYSTEMSTATUS__

enum SystemStatus {AUTOMATIC,MANUAL,UNCONNECTED};

extern SystemStatus state;

extern int targetAngle;

extern bool hasChangedMode;

extern int sentTarget;
extern bool hasReceivedOpening;

#endif