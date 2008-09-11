#include "FrCompositeTool.h"
#include "FrMainDocument.h"
#include "FrPanTool.h"
#include "FrTBCTool.h"
#include "FrZoomTool.h"
#include "FrSliceScrollTool.h"
#include "FrInteractorStyle.h"

// VTK stuff
#include "vtkRenderer.h"

FrCompositeTool::FrCompositeTool() {
    m_panTool = new FrPanTool();        
    m_zoomTool = new FrZoomTool();
    m_tbcTool = new FrTBCTool();
	m_ssTool = new FrSliceScrollTool();

	isZoom = false;
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
        if (isZoom)
			m_zoomTool->OnMouseUp(is, params);
		else
			m_panTool->OnMouseUp(is, params);
	}
	else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::MidButton;
        m_ssTool->OnMouseUp(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseUp(is, params);
    }    
    
    return false;
}

bool FrCompositeTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){    
    // here we should check what tool to use: pan or zoom, it depends on mouse coords
	isZoom = CheckMouseParams(is, params);

	// Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        if (isZoom)
			m_zoomTool->OnMouseDown(is, params);
		else
			m_panTool->OnMouseDown(is, params);
	}
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::MidButton;
        m_ssTool->OnMouseDown(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseDown(is, params);
    }
    return false;
}

bool FrCompositeTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrCompositeTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    bool result = false;

    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        if (isZoom)
			result = m_zoomTool->OnMouseDrag(is, params);
		else
			result = m_panTool->OnMouseDrag(is, params);
	}
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::MidButton;
        result = m_ssTool->OnMouseDrag(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        int x = params.X;
        int y = params.Y;
		
		params.Button = FrMouseParams::RightButton;
		result = m_tbcTool->OnMouseDrag(is, params);

        //// First change brightness
        //params.X = x;
        //params.Button = FrMouseParams::RightButton;
        //result = m_tbcTool->OnMouseDrag(is, params);

        //// Then change contrast
        //params.Y = y;
        //params.Button = FrMouseParams::RightButton;
        //result = m_tbcTool->OnMouseDrag(is, params);
    }

    return result;
}

void FrCompositeTool::SetDocument(FrMainDocument* document){
    // Set to tcb tool
    m_tbcTool->SetDocument(document);
	m_ssTool->SetDocument(document);
}

bool FrCompositeTool::CheckMouseParams(FrInteractorStyle* is, FrMouseParams& params){
	// check if coordinates are near left/right border of viewport
	int *size = is->CurrentRenderer->GetSize();
	double *aspect = is->CurrentRenderer->GetAspect();
	int XBorder = size[0];
	int YBorder = size[0]/aspect[0];

	if ((params.X+40 >= XBorder) || (params.X <= 40))
		return true;
	if ((params.Y+40 >= YBorder) || (params.Y <= 40))
		return true;

	return false;
}