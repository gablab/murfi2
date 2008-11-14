#include "FrRectangleTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

// Qt stuff
#include "Qt/qapplication.h"
#include "Qt/qcursor.h"

// Some defines 
#define DEF_TOLERANCE 0.0


FrRectangleTool::FrRectangleTool()
: m_firstPointX(0), m_firstPointY(0){
    SetCurrentState(State::None); 
}

FrRectangleTool::~FrRectangleTool(){
}


void FrRectangleTool::Start(){    
}

void FrRectangleTool::Stop(){    
}

bool FrRectangleTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    int x,y;

    if (GetCurrentState() == Drawing)
    {
        if(params.Button == FrMouseParams::LeftButton){
            SetCurrentState(State::None);     
            x = params.X;
            y = params.Y;
   
            // execute command with Action:Write
            FrMaskRectangleCmd* cmd = FrCommandController::CreateCmd<FrMaskRectangleCmd>();
            cmd->SetAction(FrMaskRectangleCmd::Action::Write);
            FrMaskRectangleCmd::Rect rect;
            rect.firstPoint.x = m_firstPointX;
            rect.firstPoint.y = m_firstPointY;
            rect.secondPoint.x = x;
            rect.secondPoint.y = y;
            cmd->SetRect(rect);
            cmd->SetImageNumber(m_ImageNumber);
            cmd->Execute();
            delete cmd;
        }
    }
    return false;
}

bool FrRectangleTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if (GetCurrentState() == None)
    {
        if(params.Button == FrMouseParams::LeftButton){
            SetCurrentState(State::Drawing);     
            m_firstPointX = params.X;
            m_firstPointY = params.Y;
        }
    }
    return false;
}

bool FrRectangleTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    // do nothing here
    return false;
}

bool FrRectangleTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    int x, y;

    if (GetCurrentState() == Drawing)
    {
        if(params.Button == FrMouseParams::LeftButton){
            x = params.X;
            y = params.Y;
            // execute command with Action:Draw
            FrMaskRectangleCmd* cmd = FrCommandController::CreateCmd<FrMaskRectangleCmd>();
            cmd->SetAction(FrMaskRectangleCmd::Action::Draw);
            FrMaskRectangleCmd::Rect rect;
            rect.firstPoint.x = m_firstPointX;
            rect.firstPoint.y = m_firstPointY;
            rect.secondPoint.x = x;
            rect.secondPoint.y = y;
            cmd->SetRect(rect);
            cmd->SetImageNumber(m_ImageNumber);
            cmd->Execute();
            delete cmd;
        }
    }
  
    return true;
}
