#include "FrMainController.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrView2D.h"
#include "FrPanTool.h"
#include "FrZoomTool.h"
#include "FrImageDocObj.h"

#include "vtkRenderWindowInteractor.h"

// Implementation of FrMainController
FrMainController::FrMainController(FrMainWindow* view, FrMainDocument* doc)
    : m_view(view), m_document(doc), m_toolController(0){

    m_toolController = new FrToolController();

    //FrPanTool* tool = new FrPanTool();
    FrZoomTool* tool = new FrZoomTool();
    m_toolController->PushTool(tool);
}

FrMainController::~FrMainController(){
    if(m_toolController) delete m_toolController;
}

void FrMainController::Initialize(){
    
    // Initialize view
    if(m_view){        
        m_view->SetController(this);
        m_view->SetDocument(m_document);

        // Setup interactor style
        if(m_view->GetView2D()) {
            FrInteractorStyle* style = new FrInteractorStyle(this);
            m_view->GetView2D()->SetInteractorStyle(style);
        }
    }

    // Initialize document
    if(m_document){
       // TODO: initialize document
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

#include <Qt/QString.h>
void FrMainController::LoadImage(QString& fileName){
        
    FrImageDocObj* imgObj = new FrImageDocObj();

    if(imgObj->LoadFromFile(fileName)){
        m_document->Add(imgObj);
    }
    else{
        // TODO: process error
    }    
    m_view->GetView2D()->UpdateScene();
}