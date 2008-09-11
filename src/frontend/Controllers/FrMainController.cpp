#include "FrMainController.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrView2D.h"
#include "FrPanTool.h"
#include "FrZoomTool.h"
#include "FrCompositeTool.h"
#include "FrImageDocObj.h"
#include "QVTKWidget.h"

#include "vtkRenderWindowInteractor.h"

// Implementation of FrMainController
FrMainController::FrMainController(FrMainWindow* view, FrMainDocument* doc)
    : m_view(view), m_document(doc), m_toolController(0){

    m_toolController = new FrToolController(this);

    //FrPanTool* tool = new FrPanTool();
    //FrZoomTool* tool = new FrZoomTool();
    FrCompositeTool* tool = new FrCompositeTool();
    tool->SetDocument(m_document);
    m_toolController->PushTool(tool);
}

FrMainController::~FrMainController(){
    if(m_toolController) delete m_toolController;
}

void FrMainController::Initialize(){
    
    // Initialize view
    if(m_view){        
        m_view->SetMainController(this);
        m_view->SetMainDocument(m_document);
        m_view->Initialize();

        // Setup interactor style
        if(m_view->GetQVTKWidget()) {
            FrInteractorStyle* style = new FrInteractorStyle(this);
            m_view->GetQVTKWidget()->GetInteractor()->SetInteractorStyle(style);
        }
    }
    
    // Initialize document
    if(m_document){
        // TODO: Som initialization
        m_document->SetDefaultValues();
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
    m_view->GetView2D()->UpdatePipeline(FRP_FULL);
}

void FrMainController::SetValueTBC(FrMainController::TBC target, double value){
    // Check for safety
    if(!m_document) return;

    // Setup value
    bool isChanged = true;
    switch(target){
        case FrMainController::Threshold:
            m_document->SetThreshold(value);
            break;
        case FrMainController::Brightness:
            m_document->SetBrightness(value);
            break;
        case FrMainController::Contrast:
            m_document->SetContrast(value);
            break;
        default:
            isChanged = false;
    }

    if(isChanged && m_view){
       m_view->GetView2D()->UpdatePipeline(FRP_TBC);
    }
}


void FrMainController::Notify(int notifyCode){

	switch(notifyCode){
		case FRN_PIPLINE_UPDATE:
			// TODO: implement..
            m_view->GetView2D()->UpdatePipeline(FRP_FULL);
			break;
		case FRN_TBC_UPDATE:
			// TODO: implement..
			m_view->GetView2D()->UpdatePipeline(FRP_TBC);
			break;
		case FRN_SETNEXTSLICE:
            m_view->GetView2D()->UpdatePipeline(FRP_SLICE);
			break;
	}
}
