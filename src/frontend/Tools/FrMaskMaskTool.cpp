#include "FrMaskMaskTool.h"
#include "FrCommandController.h"

FrMaskMaskTool::FrMaskMaskTool() 
: m_Mode(FrMaskMaskTool::Undefined){
}

FrMaskMaskTool::~FrMaskMaskTool(){
}

void FrMaskMaskTool::Start(){

}

void FrMaskMaskTool::Stop(){

}

bool FrMaskMaskTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    
    // Setup action
    bool result = false;
    if(m_Mode == FrMaskMaskTool::Copy){
       // Create command and performe action
        FrMaskCopyAdjacentCmd* cmd = FrCommandController::CreateCmd<FrMaskCopyAdjacentCmd>();
        cmd->SetImageNumber(m_ImageNumber);
        result = cmd->Execute();
        delete cmd;
    }
    else {
        // Create command and performe action
        FrMaskMaskOperationCmd* cmd = FrCommandController::CreateCmd<FrMaskMaskOperationCmd>();
        switch(m_Mode){
            case FrMaskMaskTool::Union:
                cmd->SetAction(FrMaskMaskOperationCmd::Union);
                break;
            case FrMaskMaskTool::Subtract:
                cmd->SetAction(FrMaskMaskOperationCmd::Subtract);
                break;
            case FrMaskMaskTool::Intersect:
                cmd->SetAction(FrMaskMaskOperationCmd::Intersect);
                break;
            default:
                cmd->SetAction(FrMaskMaskOperationCmd::None);
                break;
        }
        cmd->SetImageNumber(m_ImageNumber);    
        result = cmd->Execute();
        delete cmd;
    }
    return false;
}

bool FrMaskMaskTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrMaskMaskTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrMaskMaskTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrMaskMaskTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}
