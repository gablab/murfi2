#include "FrManipulationTool.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrPanTool.h"
#include "FrZoomTool.h"
#include "FrTBCTool.h"
#include "FrSliceScrollTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"
#include "FrToolController.h"
#include "QVTKWidget.h"

// VTK stuff
#include "vtkRenderer.h"

// Qt stuff
#include "Qt/qcursor.h"
#include "Qt/qpixmap.h"
#include "Qt/qapplication.h"



FrManipulationTool::FrManipulationTool() {
    m_panTool = new FrPanTool();        
    m_zoomTool = new FrZoomTool();
    m_tbcTool = new FrTBCTool();
    m_ssTool = new FrSliceScrollTool();

    isZoom = false;
    ZoomActivated = false;
}

FrManipulationTool::~FrManipulationTool(){
    delete m_panTool;
    delete m_zoomTool;
    delete m_tbcTool;
	delete m_ssTool;
}

void FrManipulationTool::Start(){
    // Setup controller and start tools
    m_panTool->SetController(this->GetController());
    m_zoomTool->SetController(this->GetController());
    m_panTool->Start();
    m_zoomTool->Start();

    m_tbcTool->SetController(this->GetController());
    m_ssTool->SetController(this->GetController());
    m_tbcTool->Start();
	m_ssTool->Start();

    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::ManipulationTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    cmd->Execute();
    delete cmd;
}

void FrManipulationTool::Stop(){
    // Stop tools and unregister controller
    m_panTool->Stop();
    m_zoomTool->Stop();
    m_panTool->SetController(0);
    m_zoomTool->SetController(0);

    m_tbcTool->Stop();
    m_ssTool->Stop();
    m_tbcTool->SetController(0);
    m_ssTool->SetController(0);

    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::ManipulationTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    cmd->Execute();
    delete cmd;
}

bool FrManipulationTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        if (isZoom)
            m_zoomTool->OnMouseUp(is, params);
        else
            m_panTool->OnMouseUp(is, params);
    }
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        m_ssTool->OnMouseUp(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseUp(is, params);
    }
    
    ZoomActivated = false;

    return false;
}

bool FrManipulationTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){    
    // here we should check what tool to use: pan or zoom, it depends on mouse coords
    isZoom = CheckMouseParams(is, params);

    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        if (isZoom){
            m_zoomTool->OnMouseDown(is, params);
            ZoomActivated = true;
        }
        else
            m_panTool->OnMouseDown(is, params);
    }
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        m_ssTool->OnMouseDown(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseDown(is, params);
    }
    return false;
}

bool FrManipulationTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    // here we should check what tool to use: pan or zoom, it depends on mouse coords
    isZoom = CheckMouseParams(is, params);
    //bool isInViewport = IsInViewPort(is, params);

    //if (!isInViewport){
    //	//change cursor to simple mode
    //	QCursor cursor(Qt::ArrowCursor);
    //	QApplication::setOverrideCursor(cursor);
    //}
    
    // get QVTK widget and set cursor
    // HACK: getting main controller
    //FrMainController* mc = dynamic_cast<FrMainController*>(this->GetController()->GetOwner());
    //FrMainWindow* mw = mc->GetMainView();
    //QVTKWidget* qvtkWidget = mw->GetQVTKWidget();

    FrSetCursorCmd* cmd = FrCommandController::CreateCmd<FrSetCursorCmd>();
    if (isZoom)
        cmd->SetCursorType(FrSetCursorCmd::Zoom);
    else
        cmd->SetCursorType(FrSetCursorCmd::Pan);

    cmd->Execute();
    delete cmd;

    return false;
}

bool FrManipulationTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    bool result = false;

    // Delegate events to appropriate tool
    if(params.Button == FrMouseParams::LeftButton){
        if (isZoom)
            result = m_zoomTool->OnMouseDrag(is, params);
        else
            result = m_panTool->OnMouseDrag(is, params);
    }
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        result = m_ssTool->OnMouseDrag(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::RightButton;
        result = m_tbcTool->OnMouseDrag(is, params);
    }

    return result;
}

bool FrManipulationTool::CheckMouseParams(FrInteractorStyle* is, FrMouseParams& params){
    // check if coordinates are near left/right border of viewport
    int *size = is->CurrentRenderer->GetSize();
    int *origin = is->CurrentRenderer->GetOrigin();

    int XBorder = size[0];
    int XOrigin = origin[0];


    // Leave side only
    int xDelta = size[0] / 6; // delta is about 15%
    if (( (params.X - XOrigin) >= (XBorder-xDelta)) ||
    ( (params.X - XOrigin) <= xDelta)) return true;
    /*if ((params.Y+40 >= YBorder) || (params.Y <= 40)) return true;*/

    return false;
}

bool FrManipulationTool::IsInViewPort(FrInteractorStyle* is, FrMouseParams& params){
    // check if coordinates are near left/right border of viewport
    int *size = is->CurrentRenderer->GetSize();
    int *origin = is->CurrentRenderer->GetOrigin();

    int XBorder = size[0];
    int YBorder = size[1];
    int XOrigin = origin[0];
    int YOrigin = origin[0];

    // Leave side only
    if (( (params.X - XOrigin) == XBorder) ||
    ( (params.X - XOrigin) == 0) ||
            ( (params.Y - YOrigin) == YBorder) ||
    ( (params.Y - YOrigin) == 0)) return false;

    return true;
}

bool FrManipulationTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}
