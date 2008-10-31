#include "FrRoiTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

#include "Qt/qstring.h"
#include "Qt/qmessagebox.h"



FrRoiTool::FrRoiTool(){
}

FrRoiTool::~FrRoiTool(){
}

void FrRoiTool::Start(){
    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::RoiTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    cmd->Execute();
    delete cmd;
}

void FrRoiTool::Stop(){
    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::RoiTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    cmd->Execute();
    delete cmd;
}

bool FrRoiTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrRoiTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    //QMessageBox::information(0, QString("Tool info"), QString("ROI tool is working"));
    return false;
}

bool FrRoiTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrRoiTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}
