#include "FrCompositeTool.h"
#include "FrMainDocument.h"
#include "FrPanTool.h"
#include "FrTBCTool.h"
#include "FrZoomTool.h"
#include "FrSliceScrollTool.h"

FrCompositeTool::FrCompositeTool() {
    m_panTool = new FrPanTool();        
    m_zoomTool = new FrZoomTool();
    m_tbcTool = new FrTBCTool();
	m_ssTool = new FrSliceScrollTool();
}

FrCompositeTool::~FrCompositeTool(){
    delete m_panTool;
    delete m_tbcTool;
    delete m_zoomTool;
	delete m_ssTool;
}

void FrCompositeTool::Start(){
    // Setup controller
    m_panTool->SetController(this->GetController());
    m_tbcTool->SetController(this->GetController());
    m_zoomTool->SetController(this->GetController());
	m_ssTool->SetController(this->GetController());

    m_panTool->Start();
    m_tbcTool->Start();
    m_zoomTool->Start();
	m_ssTool->Start();
}

void FrCompositeTool::Stop(){

    // Unregister controller
    m_panTool->SetController(0);
    m_tbcTool->SetController(0);
    m_zoomTool->SetController(0);
	m_ssTool->SetController(0);

    m_panTool->Stop();
    m_tbcTool->Stop();
    m_zoomTool->Stop();
	m_ssTool->Stop();
}

bool FrCompositeTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        m_zoomTool->OnMouseUp(is, params);
    }
    //else if(params.Button == FrMouseParams::MidButton){
    //    params.Button = FrMouseParams::LeftButton;
    //    m_zoomTool->OnMouseUp(is, params);
    //}
	else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::MidButton;
        m_ssTool->OnMouseUp(is, params);
    }
/*    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseUp(is, params);
    }   */ 
    
    return false;
}

bool FrCompositeTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){    
    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        m_zoomTool->OnMouseDown(is, params);
    }
    //else if(params.Button == FrMouseParams::MidButton){
    //    params.Button = FrMouseParams::LeftButton;
    //    m_zoomTool->OnMouseDown(is, params);
    //}
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::MidButton;
        m_ssTool->OnMouseDown(is, params);
    }
 /*   else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseDown(is, params);
    }*/
    return false;
}

bool FrCompositeTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrCompositeTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    bool result = false;

    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        result = m_zoomTool->OnMouseDrag(is, params);
    }
    //else if(params.Button == FrMouseParams::MidButton){
    //    params.Button = FrMouseParams::LeftButton;
    //    result = m_zoomTool->OnMouseDrag(is, params);
    //}
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::MidButton;
        result = m_ssTool->OnMouseDrag(is, params);
    }
    //else if(params.Button == FrMouseParams::RightButton){
    //    int x = params.X;
    //    int y = params.Y;

    //    // First change brightness
    //    params.Y = x;
    //    params.Button = FrMouseParams::LeftButton;
    //    result = m_tbcTool->OnMouseDrag(is, params);

    //    // Then change contrast
    //    params.Y = y;
    //    params.Button = FrMouseParams::RightButton;
    //    result |= m_tbcTool->OnMouseDrag(is, params);
    //}

    return result;
}

void FrCompositeTool::SetDocument(FrMainDocument* document){
    // Set to tcb tool
    m_tbcTool->SetDocument(document);
	m_ssTool->SetDocument(document);
}