#include "FrSliceScrollTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrMainDocument.h"
#include "FrCommandController.h"

// VTK stuff
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
        
        FrChangeSliceCmd* cmd1 = FrCommandController::CreateCmd<FrChangeSliceCmd>();
        cmd1->SetMouseXY(params.X, params.Y);
        cmd1->SetSliceDelta(0);
        
        FrRefreshWidgetsInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
        cmd2->SetTarget(FrRefreshWidgetsInfoCmd::ImageSettings);

        FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
        cmd->AddCommand(cmd1);
        cmd->AddCommand(cmd2);
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

    FrChangeSliceCmd* cmd1 = FrCommandController::CreateCmd<FrChangeSliceCmd>();
    cmd1->SetMouseXY(params.X, params.Y);
    cmd1->SetSliceDelta(sliceDelta);

    FrRefreshWidgetsInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd2->SetTarget(FrRefreshWidgetsInfoCmd::ImageSettings);

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->Execute();
    delete cmd;
	
    return true;
}

bool FrSliceScrollTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    if (params.IsShift || params.IsControl ||
        is->CurrentRenderer == NULL) return false;
    
    int sliceDelta = 0;
    char keySym = params.KeySym[0];

    switch (keySym){
        case 0x26:              // up arrow
            sliceDelta = 1;
            break;
        case 0x28:              // down arrow
            sliceDelta = -1;
            break;
        default:
            return false;
            break;
    }
    
    FrChangeSliceCmd* cmd1 = FrCommandController::CreateCmd<FrChangeSliceCmd>();
    cmd1->SetSliceDelta(sliceDelta);

    FrRefreshWidgetsInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd2->SetTarget(FrRefreshWidgetsInfoCmd::ImageSettings);

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->Execute();
    delete cmd;

    return true;
}
