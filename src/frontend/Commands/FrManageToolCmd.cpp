#include "FrManageToolCmd.h"
#include "FrToolController.h"
#include "FrMainWindow.h"
#include "FrManipulationTool.h"
#include "FrVoxelTool.h"
#include "FrRoiTool.h"
#include "FrVoxelSelectionTool.h"
#include "FrActionManager.h"

// Qt stuff
#include "Qt/qaction.h"


FrManageToolCmd::FrManageToolCmd()
: m_ToolType(UnknownTool), m_ToolAction(UnknownAct),
  m_IsChecked (false){
}

bool FrManageToolCmd::Execute(){
    // Check params
    if(!this->GetMainController() || 
       m_ToolType == UnknownTool || 
       m_ToolAction == UnknownAct) return false;

    bool result = false;
    switch(m_ToolAction){
        case FrManageToolCmd::NewToolAct:
            result = SetNewToolAction();
            break;
        case FrManageToolCmd::UpdateAct:
            result = UpdateCurrentToolAction();
            break;
        case FrManageToolCmd::AddToolAct:
            result = AddToolAction();
            break;
        case FrManageToolCmd::RemoveToolAct:
            result = RemoveToolAction();
            break;
        default:
            // do nothing here
            break;
    }
    return result;
}

bool FrManageToolCmd::SetNewToolAction(){
    FrToolController* tc = this->GetMainController()->GetToolController();
    tc->ClearAll(true);
    return AddToolAction();
}
   
bool FrManageToolCmd::AddToolAction(){
    FrToolController* tc = this->GetMainController()->GetToolController();

    FrTool* tool = CreateToolByType(m_ToolType);
    if(tool != 0L){
        tc->PushTool(tool);
    }
    return (tool != 0L);
}
   
bool FrManageToolCmd::RemoveToolAction(){
    bool result = false;
    FrToolController* tc = this->GetMainController()->GetToolController();
    if(tc->GetCurrentTool() != 0L){
        FrTool* tool = tc->PopTool();
        delete tool;
        result = true;
    }
    return result;
}
   
bool FrManageToolCmd::UpdateCurrentToolAction(){
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrActionManager* am = mv->GetActionManager();
    
    QAction* action = 0L;
    switch(m_ToolType){
        case FrManageToolCmd::ManipulationTool:
            action = am->GetManipulatorToolAction();
            break;
        case FrManageToolCmd::VoxelTool:
            action = am->GetVoxelToolAction();
            break;
        case FrManageToolCmd::RoiTool:
            action = am->GetRoiToolAction();
            break;
        case FrManageToolCmd::VoxelSelectionTool:
            action = am->GetVoxelSelectionToolAction();
            break;
        default:
            // do nothing here
            break;
    }

    if(action != 0L){
        action->blockSignals(true);
        action->setChecked(m_IsChecked);
        action->blockSignals(false);
    }

    return (action != 0L);
}

FrTool* FrManageToolCmd::CreateToolByType(FrManageToolCmd::ToolType toolType){
    FrTool* result = 0;
    switch(toolType){
        case FrManageToolCmd::ManipulationTool:
            result = new FrManipulationTool();
            break;
        case FrManageToolCmd::VoxelTool:
            result = new FrVoxelTool();
            break;
        case FrManageToolCmd::RoiTool:
            result = new FrRoiTool();
            break;
        case FrManageToolCmd::VoxelSelectionTool:
            result = new FrVoxelSelectionTool();
            break;
        default:
            // do nothing here
            break;
    }
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrManageToolCmd::CanUndo(){
    return false;
}

bool FrManageToolCmd::Undo(){
    return false;
}

bool FrManageToolCmd::Redo(){
    return false;
}
