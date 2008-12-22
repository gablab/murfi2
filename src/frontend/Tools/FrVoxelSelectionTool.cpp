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
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::VoxelSelectionTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    cmd->Execute();
    delete cmd;
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
        FrVoxelSelectionCmd* cmd = FrCommandController::CreateCmd<FrVoxelSelectionCmd>();
        cmd->SetAction(FrVoxelSelectionCmd::Add);
        cmd->SetPointPicker(m_pointPicker);
        cmd->SetMouseXY(params.X, params.Y);
        cmd->Execute();
        delete cmd;
    }
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