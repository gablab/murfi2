#include "FrMainController.h"



// Implementation of FrMainController
FrController::FrController(FrController* controller)
    : m_Owner(controller){
}

FrController::~FrController(){
}

void FrController::Notify(int notifyCode){
    // NOTE: in base class just transfer notification to owner 
    if(m_Owner){
        m_Owner->Notify(notifyCode);
    }
}