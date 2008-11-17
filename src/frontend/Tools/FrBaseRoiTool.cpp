#include "FrBaseRoiTool.h"
#include "FrMainController.h"
#include "FrToolController.h"


FrBaseRoiTool::FrBaseRoiTool(){
}

FrBaseRoiTool::~FrBaseRoiTool(){
}

void FrBaseRoiTool::Start(){

}

void FrBaseRoiTool::Stop(){

}

bool FrBaseRoiTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrBaseRoiTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrBaseRoiTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrBaseRoiTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}


FrMainController* FrBaseRoiTool::GetMainController(){
     // HACK: getting main controller
     FrMainController* result = 
         dynamic_cast<FrMainController*>
         (this->GetController()->GetOwner());
     return result;
}