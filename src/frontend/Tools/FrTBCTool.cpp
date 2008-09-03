#include "FrTBCTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrMainDocument.h"

FrTBCTool::FrTBCTool(FrMainDocument* doc)
    : m_Document(0), m_downX(0), m_downY(0) {
}

FrTBCTool::~FrTBCTool(){
}

void FrTBCTool::Start(){

}

void FrTBCTool::Stop(){

}

bool FrTBCTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrTBCTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    m_downX = params.X;
    m_downY = params.Y;
    return false;
}

bool FrTBCTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrTBCTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    // Check for safety
    if(!is->CurrentRenderer || !m_Document == NULL) return false;
        
    // TODO: implement

    this->GetController()->Notify(FRN_TCB_UPDATE);
    return true;
}