#ifndef __POTENTIOMETER_IMPL__
#define __POTENTIOMETER_IMPL__

#include "Pot.h"

class PotImpl : public Pot {
    private:
        int pin;
        int lastValue;

    public:
        PotImpl(int pin);

        int getValue() override;
        int getPercentage() override;
        bool hasChanged() override;
};

#endif