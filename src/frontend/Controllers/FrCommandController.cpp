#include "FrCommandController.h"
#include "FrMainController.h"


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