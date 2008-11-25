#include "FrPenTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"
#include "FrMainWindow.h"
#include "FrUtils.h"
#include "FrLayerListWidget.h"
#include "FrROIToolWidget.h"
#include "FrSpinToolWidget.h"


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
        cmd->SetAction(FrMaskPenCmd::Write);
        cmd->Execute();
        delete cmd;
    }

    return false;
}

bool FrPenTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        // execute command 
        FrMaskPenCmd* cmd = FrCommandController::CreateCmd<FrMaskPenCmd>();
        cmd->SetAction(FrMaskPenCmd::Draw);
 
        Pos center;
        center.x = params.X;
        center.y = params.Y;

        cmd->SetCenter(center);

        // get radius
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        int rad = mv->GetLayerListWidget()->GetRoiToolWidget()->GetPenWidget()->GetValue();

        cmd->SetRadius(rad);    // test
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
        cmd->SetAction(FrMaskPenCmd::Draw);
 
        Pos center;
        center.x = params.X;
        center.y = params.Y;

        cmd->SetCenter(center);

        // get radius
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        int rad = mv->GetLayerListWidget()->GetRoiToolWidget()->GetPenWidget()->GetValue();
        cmd->SetRadius(rad);    // test
        cmd->SetImageNumber(m_ImageNumber);
        cmd->Execute();
        delete cmd;
    }

    return false;
}

bool FrPenTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}
