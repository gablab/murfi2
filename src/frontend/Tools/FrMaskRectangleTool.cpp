#include "FrMaskRectangleTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"

// VTK stuff
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkPointPicker.h"

// Qt stuff
#include "Qt/qapplication.h"
#include "Qt/qcursor.h"

// Some defines 
#define DEF_TOLERANCE 0.0


FrMaskRectangleTool::FrMaskRectangleTool()
: m_firstPointX(0), m_firstPointY(0){
    SetCurrentState(State::None); 

    m_pointPicker = vtkPointPicker::New();
    m_pointPicker->SetTolerance(DEF_TOLERANCE);
}

FrMaskRectangleTool::~FrMaskRectangleTool(){
    if(m_pointPicker) m_pointPicker->Delete();
}


void FrMaskRectangleTool::Start(){    
}

void FrMaskRectangleTool::Stop(){    
}

bool FrMaskRectangleTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    int x,y;

    if (GetCurrentState() == Drawing)
    {
        if(params.Button == FrMouseParams::LeftButton){
            SetCurrentState(State::None);     
            x = params.X;
            y = params.Y;
   
            // TODO: execute command with Param:Write
            FrMaskRectangleCmd* cmd = FrCommandController::CreateCmd<FrMaskRectangleCmd>();
            cmd->SetAction(FrMaskRectangleCmd::Action::Write);
            cmd->SetPointPicker(m_pointPicker);
            FrMaskRectangleCmd::Rect rect;
            rect.leftX = m_firstPointX;
            rect.leftY = m_firstPointY;
            rect.rightX = x;
            rect.rightY = y;
            cmd->SetRect(rect);
            cmd->Execute();
            delete cmd;
        }
    }
    return false;
}

bool FrMaskRectangleTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
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

bool FrMaskRectangleTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    // do nothing here
    return false;
}

bool FrMaskRectangleTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    int x, y;

    if (GetCurrentState() == Drawing)
    {
        if(params.Button == FrMouseParams::LeftButton){
            x = params.X;
            y = params.Y;
            // TODO: execute command with Param:Draw
            FrMaskRectangleCmd* cmd = FrCommandController::CreateCmd<FrMaskRectangleCmd>();
            cmd->SetAction(FrMaskRectangleCmd::Action::Draw);
            cmd->SetPointPicker(m_pointPicker);
            FrMaskRectangleCmd::Rect rect;
            rect.leftX = m_firstPointX;
            rect.leftY = m_firstPointY;
            rect.rightX = x;
            rect.rightY = y;
            cmd->SetRect(rect);
            cmd->Execute();
            delete cmd;
        }
    }
  
    return true;
}
