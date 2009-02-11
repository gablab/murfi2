#ifndef FR_BACKGROUND
#define FR_BACKGROUND

#include "Qt/qthread.h"
#include "FrMacro.h"

class RtConfigFmriRun;

class FrBackground : public QThread {
public:
     void run();
     //void stop();

    // properties
     FrSetPropMacro(RtConfigFmriRun*, ConfigFmriRun);

private:
    //bool stopped;
};

#endif
