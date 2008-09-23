#include "FrTBCTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrMainDocument.h"
#include "FrCommandController.h"

#define MOUSE_MOVE_THRESHOLD 0.0

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
    		
    // create and init command
    int inc;
    int deltaX = params.X - m_oldX;
    FrChangeTbcCmd* cmd = FrCommandController::CreateCmd<FrChangeTbcCmd>();

    if ((abs(deltaX / 20)) > MOUSE_MOVE_THRESHOLD){
        // set brightness
		m_oldX = params.X;		
		inc = (deltaX < 0) ? -1 : 1;
        cmd->SetBrightnessDelta( inc / 100.0 );
	}

    int deltaY = params.Y - m_oldY;
	if ((abs(deltaY / 20)) > MOUSE_MOVE_THRESHOLD){
        // set contrast
		m_oldY = params.Y;
        inc = (deltaY < 0) ? -1 : 1;
        cmd->SetContrastDelta( inc / 100.0 );
	}
	
    // Execute command
    bool result = cmd->Execute();
    delete cmd;

    return result;
}