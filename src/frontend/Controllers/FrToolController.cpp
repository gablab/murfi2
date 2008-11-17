#include "FrToolController.h"
#include "FrTool.h"


FrToolController::FrToolController(FrController* owner)
    : FrController(owner){
    
}

FrToolController::~FrToolController(){
    ClearAll(true);
}

void FrToolController::ClearAll(bool doDelete){    
    while(true){
        FrTool* tool = PopTool();
        if(tool && doDelete){
            delete tool;
        } 
        
        if(!tool) break;
    }
}

FrTool* FrToolController::GetCurrentTool(){
    FrTool* tool = 0;
    if(m_tools.size() > 0){
        tool = m_tools.back();
    }
    return tool;
}

void FrToolController::PushTool(FrTool* tool){
    if(tool){
        // Stop current tool first
        FrTool* currentTool = GetCurrentTool();
        if(currentTool){
            currentTool->Stop();
        }

        // setup new one
        tool->SetController(this);
        tool->Start();

        m_tools.push_back(tool);
    }
}

FrTool* FrToolController::PopTool(){
    FrTool* tool = 0;
    if(m_tools.size()){
        tool = GetCurrentTool();

        // NOTE first stop then delete controller
        tool->Stop();
        tool->SetController(0);
        
        m_tools.pop_back();
    }

    // have to start current tool
    if(GetCurrentTool() != 0){
        GetCurrentTool()->Start();
    }
    return tool;
}

void FrToolController::Notify(int notifyCode){

    // TODO: Do some processing if needed
    // ...

    // Transfer notification to owner
    FrController::Notify(notifyCode);
}

