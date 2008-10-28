#include "FrSliceScrollTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrMainDocument.h"
#include "FrCommandController.h"


#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkImageActor.h"

#define MOUSE_MOVE_THRESHOLD 0.0 


FrSliceScrollTool::FrSliceScrollTool(){
}

FrSliceScrollTool::~FrSliceScrollTool(){
}

void FrSliceScrollTool::Start(){

}

void FrSliceScrollTool::Stop(){

}

bool FrSliceScrollTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrSliceScrollTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
	if(params.Button == FrMouseParams::LeftButton){
        m_oldX = params.X;
        m_oldY = params.Y;
        
        FrChangeSliceCmd* cmd = FrCommandController::CreateCmd<FrChangeSliceCmd>();
        cmd->SetMouseXY(params.X, params.Y);
        cmd->SetSliceDelta(0);
        cmd->Execute();
        delete cmd;
    }
    return false;
}

bool FrSliceScrollTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrSliceScrollTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
	if (params.Button != FrMouseParams::LeftButton ||
        is->CurrentRenderer == NULL) return false;
    
    int deltaY = params.Y - m_oldY;
    int sliceDelta = 0;
    if(abs(deltaY / 20) > 0){
        sliceDelta = (deltaY < 0) ? -1 : 1;
        m_oldY = params.Y;
    }

    FrChangeSliceCmd* cmd = FrCommandController::CreateCmd<FrChangeSliceCmd>();
    cmd->SetMouseXY(params.X, params.Y);
    cmd->SetSliceDelta(sliceDelta);
    cmd->Execute();
    delete cmd;
	
    return true;
}