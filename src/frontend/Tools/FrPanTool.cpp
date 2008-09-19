#include "FrPanTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

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
    is->CurrentRenderer->GetRenderWindow()->SetCurrentCursor(VTK_CURSOR_DEFAULT);
    return false;
}

bool FrPanTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    // Start pan
    if(params.Button == FrMouseParams::LeftButton){
        m_oldX = params.X;
        m_oldY = params.Y;

        is->CurrentRenderer->GetRenderWindow()->SetCurrentCursor(VTK_CURSOR_HAND);
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
    double newFocus[3] = { motionVector[0] + viewFocus[0], 
                           motionVector[0] + viewFocus[0], 
                           motionVector[0] + viewFocus[0] }; 
    
    camera->GetPosition(viewPoint);
    double newPos[3] = { motionVector[0] + viewFocus[0],
                         motionVector[1] + viewFocus[1],
                         motionVector[2] + viewFocus[2] };

    
    FrChangeCamCmd* cmd = FrCommandController::CreateCmd<FrChangeCamCmd>();
    cmd->SetFocalPoint(newFocus);
    cmd->SetPosition(newPos);
    cmd->Execute();
    delete cmd;
  
    return true;
}