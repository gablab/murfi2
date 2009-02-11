#include "FrBackground.h"
#include "RtConfigFmriRun.h"
#include "RtExperiment.h"


/* ohinds 2009-02-02
 * tried to fix this?
 */
void FrBackground::run(){
    if (m_ConfigFmriRun){
      //executeRun(static_cast<RtConfigFmriRun>(*m_ConfigFmriRun));
      executeRun(*m_ConfigFmriRun);
    }
}

