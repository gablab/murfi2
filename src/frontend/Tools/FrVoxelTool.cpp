#include "FrVoxelTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

#include "Qt/QString.h"
#include "Qt/QMessageBox.h"

FrVoxelTool::FrVoxelTool(){
}

FrVoxelTool::~FrVoxelTool(){
}

void FrVoxelTool::Start(){
    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::VoxelTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    cmd->Execute();
    delete cmd;
}

void FrVoxelTool::Stop(){
    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::VoxelTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    cmd->Execute();
    delete cmd;
}

bool FrVoxelTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrVoxelTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    QMessageBox::information(0, QString("Tool info"), QString("Voxel tool is working"));
    return false;
}

bool FrVoxelTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrVoxelTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}