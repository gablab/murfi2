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
    if(params.Button == FrMouseParams::LeftButton){
        // execute command 
        FrMaskSphereCmd* cmd = FrCommandController::CreateCmd<FrMaskSphereCmd>();
        cmd->SetAction(FrMaskSphereCmd::Write);

        Pos center;
        center.x = params.X;
        center.y = params.Y;

        cmd->SetCenter(center);
        // get radius
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        int rad = mv->GetLayerListWidget()->GetRoiToolWidget()->GetSphereWidget()->GetValue();

        cmd->SetRadius(rad);
        cmd->SetImageNumber(m_ImageNumber);
        cmd->Execute();
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
    cmd->Execute();
    delete cmd;

    return false;
}

bool FrSphereTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}
