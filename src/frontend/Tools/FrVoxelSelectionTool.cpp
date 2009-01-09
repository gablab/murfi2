#include "FrVoxelSelectionTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

// Qt stuff
#include "Qt/qstring.h"
#include "Qt/qmessagebox.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkPointPicker.h"

// Some defines 
#define DEF_TOLERANCE 0.0
#define BAD_POINT_INDEX -1


FrVoxelSelectionTool::FrVoxelSelectionTool()
: m_pointPicker(0) {
    m_pointPicker = vtkPointPicker::New();
    m_pointPicker->SetTolerance(DEF_TOLERANCE);
}

FrVoxelSelectionTool::~FrVoxelSelectionTool(){
    if(m_pointPicker) m_pointPicker->Delete();
}

void FrVoxelSelectionTool::Start(){
    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd1 = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd1->SetToolType(FrManageToolCmd::VoxelSelectionTool);
    cmd1->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd1->SetIsChecked(true);
    //cmd1->Execute();
    //delete cmd;

    FrSetCursorCmd* cmd2 = FrCommandController::CreateCmd<FrSetCursorCmd>();
    cmd2->SetCursorType(FrSetCursorCmd::Arrow);

    FrMultiCmd* multiCmd = FrCommandController::CreateCmd<FrMultiCmd>();
    multiCmd->AddCommand(cmd1);
    multiCmd->AddCommand(cmd2);
    multiCmd->Execute();
    delete multiCmd;
}

void FrVoxelSelectionTool::Stop(){
    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::VoxelSelectionTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    cmd->Execute();
    delete cmd;
}

bool FrVoxelSelectionTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrVoxelSelectionTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        FrVoxelSelectionCmd* cmd1 = FrCommandController::CreateCmd<FrVoxelSelectionCmd>();
        cmd1->SetAction(FrVoxelSelectionCmd::Add);
        cmd1->SetPointPicker(m_pointPicker);
        cmd1->SetMouseXY(params.X, params.Y);
        
        //cmd->Execute();
        //delete cmd;

        FrRefreshWidgetsInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();

        FrMultiCmd* multiCmd = FrCommandController::CreateCmd<FrMultiCmd>();
        multiCmd->AddCommand(cmd1);
        multiCmd->AddCommand(cmd2);
        multiCmd->Execute();
        delete multiCmd;
    }
    //else if (params.Button == FrMouseParams::RightButton){
    //    FrVoxelSelectionCmd* cmd = FrCommandController::CreateCmd<FrVoxelSelectionCmd>();
    //    cmd->SetAction(FrVoxelSelectionCmd::Remove);
    //    cmd->SetPointPicker(m_pointPicker);
    //    cmd->SetMouseXY(params.X, params.Y);
    //    cmd->Execute();
    //    delete cmd;
    //}


	return false;
}

bool FrVoxelSelectionTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrVoxelSelectionTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrVoxelSelectionTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}
