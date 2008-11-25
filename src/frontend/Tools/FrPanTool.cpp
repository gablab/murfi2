#include "FrPanTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

// VTK stuff
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
// Qt stuff
#include "Qt/qapplication.h"
#include "Qt/qcursor.h"


FrPanTool::FrPanTool()
: m_oldX(0), m_oldY(0){
}

FrPanTool::~FrPanTool(){
}


void FrPanTool::Start(){    
}

void FrPanTool::Stop(){    
}

bool FrPanTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    // NOTE: do nothing here
    //is->CurrentRenderer->GetRenderWindow()->SetCurrentCursor(VTK_CURSOR_DEFAULT);
	//QCursor cursor(Qt::ArrowCursor);
	//QApplication::setOverrideCursor(cursor);   
	return false;
}

bool FrPanTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    // Start pan
    if(params.Button == FrMouseParams::LeftButton){
        m_oldX = params.X;
        m_oldY = params.Y;

        //is->CurrentRenderer->GetRenderWindow()->SetCurrentCursor(VTK_CURSOR_HAND);
		//QCursor cursor(Qt::ClosedHandCursor);
		//QApplication::setOverrideCursor(cursor);
	}
    return false;
}

bool FrPanTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    // NOTE: do nothing here
    return false;
}

bool FrPanTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    // Do pan here    
    double viewFocus[4], viewPoint[3], focalDepth;
    double newPickPoint[4], oldPickPoint[4], motionVector[3];
    
    if (params.Button != FrMouseParams::LeftButton ||
        is->CurrentRenderer == NULL) return false;

    // Calculate the focal depth
    vtkCamera *camera = is->CurrentRenderer->GetActiveCamera();
    camera->GetFocalPoint(viewFocus);
    focalDepth = viewFocus[2];
    is->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2], viewFocus);    
    is->ComputeDisplayToWorld(double(params.X), double(params.Y), focalDepth, newPickPoint);
    
    // Has to recalc old mouse point since the viewport has moved
    is->ComputeDisplayToWorld(double(m_oldX), double(m_oldY), focalDepth, oldPickPoint);
    m_oldX = params.X;
    m_oldY = params.Y;
  
    // Camera motion is reversed
    motionVector[0] = oldPickPoint[0] - newPickPoint[0];
    motionVector[1] = oldPickPoint[1] - newPickPoint[1];
    motionVector[2] = oldPickPoint[2] - newPickPoint[2];
    
    camera->GetFocalPoint(viewFocus);
    camera->GetPosition(viewPoint);
    
    FrChangeCamCmd* cmd = FrCommandController::CreateCmd<FrChangeCamCmd>();
    cmd->SetMouseXY(params.X, params.Y);
    cmd->SetPosition(motionVector[0] + viewPoint[0],
                     motionVector[1] + viewPoint[1],
                     motionVector[2] + viewPoint[2]);

    cmd->SetFocalPoint( motionVector[0] + viewFocus[0],
                        motionVector[1] + viewFocus[1],
                        motionVector[2] + viewFocus[2] );
    cmd->Execute();
    delete cmd;
  
    return true;
}

bool FrPanTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    vtkCamera *camera = is->CurrentRenderer->GetActiveCamera();

    // TODO: implement

    //FrChangeCamCmd* cmd = FrCommandController::CreateCmd<FrChangeCamCmd>();
    //cmd->SetMouseXY(params.X, params.Y);
    //cmd->SetPosition(motionVector[0] + viewPoint[0],
    //                 motionVector[1] + viewPoint[1],
    //                 motionVector[2] + viewPoint[2]);

    //cmd->SetFocalPoint( motionVector[0] + viewFocus[0],
    //                    motionVector[1] + viewFocus[1],
    //                    motionVector[2] + viewFocus[2] );
    //cmd->Execute();
    //delete cmd;

    return true;
}