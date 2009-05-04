#include "FrCommandController.h"
#include "FrMainController.h"

//ACE_Mutex  FrCommandController::m_Mutex;
QMutex  FrCommandController::m_Mutex(QMutex::Recursive);

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
  // * getting stuck on the lock on app init  
  */
  lock();  
  
  bool result = cmd->Execute();
  
  unlock();
  

  return result;
}

// scopic Alan, NOTE: now we use recursive mutex, that means same thread can lock mutex multiple times 
// (and it should also unlock it same number of times) while other threads can't get access to data 
// and should wait while mutex will be released by this thread.
// This will not deadblock application when one command invoke another in the same thread

void FrCommandController::lock(){
    //ACE_TRACE(("FrCommandController::lock"));
    m_Mutex.lock();
    //m_Mutex.acquire();
}

void FrCommandController::unlock(){
    //ACE_TRACE(("FrCommandController::unlock"));
    //m_Mutex.release();
    m_Mutex.unlock();
}
