#include "FrSphereTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"
#include "FrMainWindow.h"
#include "FrUtils.h"
#include "FrLayerListWidget.h"
#include "FrROIToolWidget.h"
#include "FrSpinToolWidget.h"


FrSphereTool::FrSphereTool(){
}

FrSphereTool::~FrSphereTool(){
}

void FrSphereTool::Start(){

}

void FrSphereTool::Stop(){

}

bool FrSphereTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrSphereTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton || params.Button == FrMouseParams::RightButton){
        // execute command 
        FrMaskSphereCmd* cmd = FrCommandController::CreateCmd<FrMaskSphereCmd>();
        //cmd->SetAction(FrMaskSphereCmd::Write);

        if(params.Button == FrMouseParams::LeftButton)
            cmd->SetAction(FrMaskSphereCmd::Write);
        else 
            cmd->SetAction(FrMaskSphereCmd::Erase); 
        
        Pos center;
        center.x = params.X;
        center.y = params.Y;

        cmd->SetCenter(center);
        // get radius
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        int rad = mv->GetLayerListWidget()->GetRoiToolWidget()->GetSphereWidget()->GetValue();

        cmd->SetRadius(rad);
        cmd->SetImageNumber(m_ImageNumber);
        FrCommandController::Execute(cmd);
        delete cmd;
    }
    
    return false;
}

bool FrSphereTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    // execute command 
    FrMaskSphereCmd* cmd = FrCommandController::CreateCmd<FrMaskSphereCmd>();
    cmd->SetAction(FrMaskSphereCmd::DrawSelection);

    Pos center;
    center.x = params.X;
    center.y = params.Y;

    cmd->SetCenter(center);
    // get radius
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    int rad = mv->GetLayerListWidget()->GetRoiToolWidget()->GetSphereWidget()->GetValue();

    cmd->SetRadius(rad);
    cmd->SetImageNumber(m_ImageNumber);
    FrCommandController::Execute(cmd);
    delete cmd;

    return true;
}

bool FrSphereTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrSphereTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}
