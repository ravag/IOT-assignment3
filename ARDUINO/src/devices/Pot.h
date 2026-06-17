#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

class Pot {

    public:
        virtual int getValue() = 0;

        virtual int getPercentage() = 0;

        virtual bool hasChanged();
    
};

#endif