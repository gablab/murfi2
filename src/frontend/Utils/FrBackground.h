#ifndef FR_BACKGROUND
#define FR_BACKGROUND

#include "Qt/qthread.h"
#include "FrMacro.h"

class RtConductor;

class FrBackground : public QThread {
public:
     void run();
     //void stop();

    // properties
     FrSetPropMacro(RtConductor*, Conductor);

private:
    //bool stopped;
};

#endif