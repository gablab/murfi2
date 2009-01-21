#include "FrBackground.h"
#include "RtConductor.h"

void FrBackground::run(){
    if (m_Conductor){
        m_Conductor->run();
    }
}

