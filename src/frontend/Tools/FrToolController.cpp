#include "FrToolController.h"
#include "FrTool.h"


FrToolController::FrToolController(){

}

FrToolController::~FrToolController(){
    ClearAll();
}

void FrToolController::ClearAll(){    
    while(true){
        FrTool* tool = PopTool();
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
        GetCurrentTool()->Stop();

        tool->SetController(this);
        tool->Start();

        m_tools.push_back(tool);
    }
}

FrTool* FrToolController::PopTool(){
    FrTool* tool = 0;
    if(m_tools.size()){
        tool = GetCurrentTool();
        tool->SetController(0);
        tool->Stop();

        m_tools.pop_back();
    }
    return tool;
}