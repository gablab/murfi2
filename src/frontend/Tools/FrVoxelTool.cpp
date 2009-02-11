#include "FrVoxelTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"
#include "FrTBCTool.h"
#include "FrSliceScrollTool.h"

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

    m_tbcTool = new FrTBCTool();
    m_ssTool = new FrSliceScrollTool();
}

FrVoxelTool::~FrVoxelTool(){
    if(m_pointPicker) m_pointPicker->Delete();
    delete m_tbcTool;
	delete m_ssTool;
}

void FrVoxelTool::Start(){
    // Setup controllers and start other tools
    m_tbcTool->SetController(this->GetController());
    m_ssTool->SetController(this->GetController());
    m_tbcTool->Start();
	m_ssTool->Start();

    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::VoxelTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    FrCommandController::Execute(cmd);
    delete cmd;
}

void FrVoxelTool::Stop(){
    // Stop other tools and unregister controller
    m_tbcTool->Stop();
    m_ssTool->Stop();
    m_tbcTool->SetController(0);
    m_ssTool->SetController(0);

    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::VoxelTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    FrCommandController::Execute(cmd);
    delete cmd;
}

bool FrVoxelTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    bool result = false;
    if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        result = m_ssTool->OnMouseUp(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        result = m_tbcTool->OnMouseUp(is, params);
    }
    return result;
}

bool FrVoxelTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        FrVoxelInfoCmd* cmd1 = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
        cmd1->SetAction(FrVoxelInfoCmd::Add);
        cmd1->SetPointPicker(m_pointPicker);
        cmd1->SetMouseXY(params.X, params.Y);

	FrChangeSliceCmd* cmd2 = FrCommandController::CreateCmd<FrChangeSliceCmd>();
	cmd2->SetMouseXY(params.X, params.Y);	

        FrRefreshWidgetsInfoCmd* cmd3 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
        cmd3->SetTarget(FrRefreshWidgetsInfoCmd::GraphPane);

        FrMultiCmd* multiCmd = FrCommandController::CreateCmd<FrMultiCmd>();
        multiCmd->AddCommand(cmd1);
        multiCmd->AddCommand(cmd2);
        multiCmd->AddCommand(cmd3);
        FrCommandController::Execute(multiCmd);
        delete multiCmd;
    }
    else if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        m_ssTool->OnMouseDown(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::LeftButton;
        m_tbcTool->OnMouseDown(is, params);
    }
	return false;
}

bool FrVoxelTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    FrVoxelInfoCmd* cmd = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
    cmd->SetAction(FrVoxelInfoCmd::Update);
    cmd->SetPointPicker(m_pointPicker);
    cmd->SetMouseXY(params.X, params.Y);
    FrCommandController::Execute(cmd);
    delete cmd;

    return false;
}

bool FrVoxelTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        FrVoxelInfoCmd* cmd1 = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
        cmd1->SetAction(FrVoxelInfoCmd::Add);
        cmd1->SetPointPicker(m_pointPicker);
        cmd1->SetMouseXY(params.X, params.Y);

	FrChangeSliceCmd* cmd2 = FrCommandController::CreateCmd<FrChangeSliceCmd>();
	cmd2->SetMouseXY(params.X, params.Y);	

        FrRefreshWidgetsInfoCmd* cmd3 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
        cmd3->SetTarget(FrRefreshWidgetsInfoCmd::GraphPane);

        FrMultiCmd* multiCmd = FrCommandController::CreateCmd<FrMultiCmd>();
        multiCmd->AddCommand(cmd1);
        multiCmd->AddCommand(cmd2);
        multiCmd->AddCommand(cmd3);
        FrCommandController::Execute(multiCmd);
        delete multiCmd;

//        FrVoxelInfoCmd* cmd = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
//        cmd->SetAction(FrVoxelInfoCmd::Update);
//        cmd->SetPointPicker(m_pointPicker);
//        cmd->SetMouseXY(params.X, params.Y);
//        FrCommandController::Execute(cmd);
//        delete cmd;
    }
    if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        m_ssTool->OnMouseDrag(is, params);
    }
    else if(params.Button == FrMouseParams::RightButton){
        params.Button = FrMouseParams::RightButton;
        m_tbcTool->OnMouseDrag(is, params);
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

bool FrVoxelTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}
