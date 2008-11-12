#include "FrVoxelTool.h"
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


FrVoxelTool::FrVoxelTool()
: m_pointPicker(0) {
    m_pointPicker = vtkPointPicker::New();
    m_pointPicker->SetTolerance(DEF_TOLERANCE);
}

FrVoxelTool::~FrVoxelTool(){
    if(m_pointPicker) m_pointPicker->Delete();
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
    if(params.Button == FrMouseParams::LeftButton){
        FrVoxelInfoCmd* cmd = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
        cmd->SetAction(FrVoxelInfoCmd::Update);
        cmd->SetPointPicker(m_pointPicker);
        cmd->SetMouseXY(params.X, params.Y);
        cmd->Execute();
        delete cmd;
    }
	return false;
}

bool FrVoxelTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrVoxelTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        FrVoxelInfoCmd* cmd = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
        cmd->SetAction(FrVoxelInfoCmd::Update);
        cmd->SetPointPicker(m_pointPicker);
        cmd->SetMouseXY(params.X, params.Y);
        cmd->Execute();
        delete cmd;
    }
    return false;
}

int FrVoxelTool::GetPickedPointIndex(int x, int y, vtkRenderer* renderer){
    if (!m_pointPicker->Pick(x, y, 0, renderer)) {
        return BAD_POINT_INDEX;
    }

    int id = m_pointPicker->GetPointId();
    return id;
}
