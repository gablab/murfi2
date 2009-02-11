#include "FrCommandController.h"
#include "FrMainController.h"

ACE_Mutex  FrCommandController::m_Mutex;

FrCommandController* FrCommandController::m_controller = 0L;

FrCommandController::FrCommandController()
: FrController(0L){
}

FrCommandController::~FrCommandController(){
}

FrMainController* FrCommandController::GetMainController(){
    // NOTE: MainController have to be owner of this 
    // FrCommandController in our application !!!
    return (FrMainController*)this->GetOwner();
}

bool FrCommandController::Execute(FrBaseCmd* cmd){
  /* ohinds 2009-02-05
   * getting stuck on the lock on app init
  lock();
  */
  bool result = cmd->Execute();
  /*
  unlock();
  */

  return result;
}

void FrCommandController::lock(){
    ACE_TRACE(("FrCommandController::lock"));
    m_Mutex.acquire();
}

void FrCommandController::unlock(){
    ACE_TRACE(("FrCommandController::unlock"));
    m_Mutex.release();
}
