#include "FrInteractorStyle.h"
#include "FrMainController.h"
#include "FrTool.h"

#include <vtkRenderWindowInteractor.h>

FrInteractorStyle::FrInteractorStyle(FrMainController* controller)
: m_controller(controller), m_state(Undefined){
}

FrInteractorStyle::~FrInteractorStyle(){
}

void FrInteractorStyle::OnMouseMove(){
    if(m_controller->HasActiveTool()){
        FrMouseParams params;
        params.X = Interactor->GetEventPosition()[0];
        params.Y = Interactor->GetEventPosition()[1];
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);

        // Setup button
        switch(m_state){
            case Left: 
                params.Button = FrMouseParams::LeftButton; 
                break;
            case Mid: 
                params.Button = FrMouseParams::MidButton; 
                break;
            case Right: 
                params.Button = FrMouseParams::RightButton; 
                break;
        }

        if(m_state != Undefined){
            m_controller->GetCurrentTool()->OnMouseDrag(params);
        }
        else{
            m_controller->GetCurrentTool()->OnMouseMove(params);
        }
    }
}

void FrInteractorStyle::OnLeftButtonDown(){
    m_state = Left;

    if(m_controller->HasActiveTool()){
        FrMouseParams params;
        params.X = Interactor->GetEventPosition()[0];
        params.Y = Interactor->GetEventPosition()[1];
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);
        params.Button = FrMouseParams::LeftButton;
        
        m_controller->GetCurrentTool()->OnMouseDown(params);
    }
}

void FrInteractorStyle::OnLeftButtonUp(){
    m_state = Undefined;

    if(m_controller->HasActiveTool()){
        FrMouseParams params;
        params.X = Interactor->GetEventPosition()[0];
        params.Y = Interactor->GetEventPosition()[1];
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);
        params.Button = FrMouseParams::LeftButton;
        
        m_controller->GetCurrentTool()->OnMouseUp(params);
    }
}

void FrInteractorStyle::OnMiddleButtonDown(){
    m_state = Mid;

    if(m_controller->HasActiveTool()){
        FrMouseParams params;
        params.X = Interactor->GetEventPosition()[0];
        params.Y = Interactor->GetEventPosition()[1];
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);
        params.Button = FrMouseParams::MidButton;

        m_controller->GetCurrentTool()->OnMouseDown(params);
    }
}

void FrInteractorStyle::OnMiddleButtonUp(){
    m_state = Undefined;

    if(m_controller->HasActiveTool()){
        FrMouseParams params;
        params.X = Interactor->GetEventPosition()[0];
        params.Y = Interactor->GetEventPosition()[1];
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);
        params.Button = FrMouseParams::MidButton;
        
        m_controller->GetCurrentTool()->OnMouseUp(params);
    }
}

void FrInteractorStyle::OnRightButtonDown(){
    m_state = Right;

    if(m_controller->HasActiveTool()){
        FrMouseParams params;
        params.X = Interactor->GetEventPosition()[0];
        params.Y = Interactor->GetEventPosition()[1];
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);
        params.Button = FrMouseParams::RightButton;
        
        m_controller->GetCurrentTool()->OnMouseDown(params);
    }
}

void FrInteractorStyle::OnRightButtonUp(){
    m_state = Undefined;

    if(m_controller->HasActiveTool()){
        FrMouseParams params;
        params.X = Interactor->GetEventPosition()[0];
        params.Y = Interactor->GetEventPosition()[1];
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);
        params.Button = FrMouseParams::RightButton;
        
        m_controller->GetCurrentTool()->OnMouseUp(params);
    }
}

void FrInteractorStyle::OnMouseWheelForward(){
    /*if(m_controller.HasActiveTool()){
        FrMouseParams params;
        params.Delta = int(this->MouseWheelMotionFactor);
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);
        
        m_controller->GetCurrentTool()->OnMouseMove(params);
    }*/
}

void FrInteractorStyle::OnMouseWheelBackward(){
    /*if(m_controller.HasActiveTool()){
        FrMouseParams params;
        params.Delta = int(this->MouseWheelMotionFactor);
        params.IsShift = (Interactor->GetShiftKey() != 0);
        params.IsControl = (Interactor->GetControlKey() != 0);
        
        m_controller->GetCurrentTool()->OnMouseMove(params);
    }*/
}