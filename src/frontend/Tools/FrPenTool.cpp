#include "FrPenTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"


FrPenTool::FrPenTool(){
}

FrPenTool::~FrPenTool(){
}

void FrPenTool::Start(){

}

void FrPenTool::Stop(){

}

bool FrPenTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        // execute command 
        FrMaskPenCmd* cmd = FrCommandController::CreateCmd<FrMaskPenCmd>();
        cmd->SetAction(FrMaskPenCmd::Action::Write);
        cmd->Execute();
        delete cmd;
    }

    return false;
}

bool FrPenTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        // execute command 
        FrMaskPenCmd* cmd = FrCommandController::CreateCmd<FrMaskPenCmd>();
        cmd->SetAction(FrMaskPenCmd::Action::Draw);
 
        Pos center;
        center.x = params.X;
        center.y = params.Y;

        cmd->SetCenter(center);
        // get radius
        cmd->SetRadius(10);    // test
        cmd->SetImageNumber(m_ImageNumber);
        cmd->Execute();
        delete cmd;
    }

    return false;
}

bool FrPenTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrPenTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        // execute command 
        FrMaskPenCmd* cmd = FrCommandController::CreateCmd<FrMaskPenCmd>();
        cmd->SetAction(FrMaskPenCmd::Action::Draw);
 
        Pos center;
        center.x = params.X;
        center.y = params.Y;

        cmd->SetCenter(center);
        // get radius
        cmd->SetRadius(10);    // test
        cmd->SetImageNumber(m_ImageNumber);
        cmd->Execute();
        delete cmd;
    }

    return false;
}
