#include "FrSphereTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"


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
        Pos center;
        center.x = params.X;
        center.y = params.Y;

        cmd->SetCenter(center);
        // get radius
        cmd->SetRadius(100);    // test
        cmd->SetImageNumber(m_ImageNumber);
        cmd->Execute();
        delete cmd;
    }
    
    return false;
}

bool FrSphereTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrSphereTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}
