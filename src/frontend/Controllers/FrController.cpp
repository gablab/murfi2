#include "FrMainController.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrView2D.h"
#include "FrPanTool.h"
#include "FrZoomTool.h"
#include "FrSliceScrollTool.h"
#include "FrImageDocObj.h"

#include "vtkRenderWindowInteractor.h"

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