#include "FrUpdatePipelineCmd.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"


FrUpdatePipelineCmd::FrUpdatePipelineCmd()
: m_PipelinePoint(FRP_FULL) {

}

bool FrUpdatePipelineCmd::Execute(){
    // Safty
    if(!this->GetMainController()) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    mv->GetCurrentView()->UpdatePipeline(m_PipelinePoint);
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrUpdatePipelineCmd::CanUndo(){
    return false;
}

bool FrUpdatePipelineCmd::Undo(){
    return false;
}

bool FrUpdatePipelineCmd::Redo(){
    return false;
}
