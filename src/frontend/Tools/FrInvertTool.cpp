#include "FrInvertTool.h"
#include "FrCommandController.h"

FrInvertTool::FrInvertTool(){
}

FrInvertTool::~FrInvertTool(){
}

void FrInvertTool::Start(){

}

void FrInvertTool::Stop(){

}

bool FrInvertTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    // Create command and performa action
    FrMaskMaskOperationCmd* cmd = FrCommandController::CreateCmd<FrMaskMaskOperationCmd>();
    cmd->SetAction(FrMaskMaskOperationCmd::Invert);
    cmd->SetImageNumber(m_ImageNumber);
    cmd->Execute();
    delete cmd;

    return false;
}

bool FrInvertTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrInvertTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrInvertTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrInvertTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}
