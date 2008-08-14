#include "FrMainController.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrView2D.h"

#include "vtkRenderWindowInteractor.h"

// Implementation of FrMainController
FrMainController::FrMainController(FrMainWindow* view, FrMainDocument* doc)
    : m_view(view), m_document(doc), m_toolController(0){

    m_toolController = new FrToolController();
}

FrMainController::~FrMainController(){
    if(m_toolController) delete m_toolController;
}

void FrMainController::Initialize(){
    
    // Initialize view
    if(m_view){
        // Setup interactor style
        if(m_view->GetView2D()) {
            FrInteractorStyle* style = new FrInteractorStyle(this);
            m_view->GetView2D()->SetInteractorStyle(style);
        }
    }

    // Initialize document
    if(m_document){
        // TODO: some initialization
    }
}

bool FrMainController::HasActiveTool(){
    bool result = false;
    if(m_toolController){
        result = (m_toolController->GetCurrentTool() != 0);
    }
    return result;
}

FrTool* FrMainController::GetCurrentTool(){
    FrTool* result = 0;
    if(m_toolController){
        result = m_toolController->GetCurrentTool();
    }
    return result;
}

//void FrMainController::LoadImage(QString& fileName){
//
//}