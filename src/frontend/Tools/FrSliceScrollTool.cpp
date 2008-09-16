#include "FrSliceScrollTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrMainDocument.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkImageActor.h"

FrSliceScrollTool::FrSliceScrollTool(FrMainDocument* doc)
	: m_Document(0), m_sliceNumber(0){
}

FrSliceScrollTool::~FrSliceScrollTool(){
}

void FrSliceScrollTool::Start(){

}

void FrSliceScrollTool::Stop(){

}

bool FrSliceScrollTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrSliceScrollTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
	if(params.Button == FrMouseParams::MidButton){
        m_oldX = params.X;
        m_oldY = params.Y;
    }

	//this->SetSlice(1, is);
	this->SetMousePosition(params.X, params.Y);

    return false;
}

bool FrSliceScrollTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrSliceScrollTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
	if (params.Button != FrMouseParams::MidButton ||
        is->CurrentRenderer == NULL) return false;
        
    int deltaY = params.Y - m_oldY;
//    m_oldX = params.X;
//    m_oldY = params.Y;

  //  double centerY = is->CurrentRenderer->GetCenter()[1];
  //  double dyFactor = deltaY / centerY;
	if ((abs(deltaY/20)) > 0){
		m_oldY = params.Y;
		int inc;
		if (deltaY<0)
			inc = -1;
		else
			inc = 1;

		this->SetSlice(inc, is);
	}

	this->SetMousePosition(params.X, params.Y);

    return true;
}

bool FrSliceScrollTool::SetSlice(int inc, FrInteractorStyle* is){
    // Check for safety
    if(!is->CurrentRenderer || m_Document == NULL) return false;
     
	m_Document->SetSlice(inc);

	this->GetController()->Notify(FRN_SETNEXTSLICE);
    return true;
}

bool FrSliceScrollTool::SetMousePosition(int x, int y){
	m_Document->SetXCoord(x);
	m_Document->SetYCoord(y);
	
	return true;
}