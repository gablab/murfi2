#include "FrTBCTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrMainDocument.h"

FrTBCTool::FrTBCTool(FrMainDocument* doc)
    : m_Document(0), m_oldX(0), m_oldY(0) {
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
    m_oldX = params.X;
    m_oldY = params.Y;
    return false;
}

bool FrTBCTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrTBCTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    // Check for safety
    if(!is->CurrentRenderer || m_Document == NULL) return false;
        
    // TODO: implement
	int deltaY = params.Y - m_oldY;
	int deltaX = params.X - m_oldX;
	
	// set contrast
	if ((abs(deltaY/20)) > 0){
		m_oldY = params.Y;
		int inc;
		if (deltaY<0)
			inc = -1;
		else
			inc = 1;		
		this->SetContrast((double)inc, is);
	}
	// set brightness
	if ((abs(deltaX/20)) > 0){
		m_oldX = params.X;
		int inc;
		if (deltaX<0)
			inc = -1;
		else
			inc = 1;
		this->SetBrightness((double)inc, is);
	}

    return true;
}

bool FrTBCTool::SetBrightness(double inc, FrInteractorStyle* is){
    // Check for safety
    if(!is->CurrentRenderer || m_Document == NULL) return false;
     
	m_Document->SetBrightness(m_Document->GetBrightness() + inc/100.0);

	this->GetController()->Notify(FRN_TCB_UPDATE);
    return true;
}

bool FrTBCTool::SetContrast(double inc, FrInteractorStyle* is){
    // Check for safety
    if(!is->CurrentRenderer || m_Document == NULL) return false;
     
	m_Document->SetContrast(m_Document->GetContrast() + inc/100.0);

	this->GetController()->Notify(FRN_TCB_UPDATE);
    return true;
}
