#include "FrMaskMaskTool.h"


FrMaskMaskTool::FrMaskMaskTool() 
: m_Mode(FrMaskMaskTool::Undefined){
}

FrMaskMaskTool::~FrMaskMaskTool(){
}

void FrMaskMaskTool::Start(){

}

void FrMaskMaskTool::Stop(){

}

bool FrMaskMaskTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrMaskMaskTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrMaskMaskTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrMaskMaskTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}
