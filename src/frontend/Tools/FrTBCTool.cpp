#include "FrTBCTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrMainDocument.h"
#include "FrCommandController.h"


#define MOUSE_MOVE_THRESHOLD 0.0

FrTBCTool::FrTBCTool()
    : m_oldX(0), m_oldY(0) {
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
    if(!is->CurrentRenderer) return false;
    		
    // create and init command
    int inc;
    int deltaX = params.X - m_oldX;
    FrChangeTbcCmd* cmd1 = FrCommandController::CreateCmd<FrChangeTbcCmd>();

    if(!params.IsControl){
        if ((abs(deltaX / 20)) > MOUSE_MOVE_THRESHOLD){
            // set brightness
		    m_oldX = params.X;		
		    inc = (deltaX < 0) ? -1 : 1;
            cmd1->SetBrightnessDelta( inc / 100.0 );
	    }

        int deltaY = params.Y - m_oldY;
	    if ((abs(deltaY / 20)) > MOUSE_MOVE_THRESHOLD){
            // set contrast
		    m_oldY = params.Y;
            inc = (deltaY < 0) ? -1 : 1;
            cmd1->SetContrastDelta( inc / 100.0 );
	    }
    }
    else {
        // Threshold changing
        int deltaY = params.Y - m_oldY;
	    if ((abs(deltaY / 20)) > MOUSE_MOVE_THRESHOLD){
            // set threshold
		    m_oldY = params.Y;
            inc = (deltaY < 0) ? -1 : 1;
            cmd1->SetThresholdDelta( inc / 100.0 );
	    }
    }
	
    FrRefreshWidgetsInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd2->SetTarget(FrRefreshWidgetsInfoCmd::ImageSettings);

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    bool result = FrCommandController::Execute(cmd);
    delete cmd;

    return result;
}

bool FrTBCTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}
