#include "FrChangeCamCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"

#include "FrTabSettingsDocObj.h"

FrChangeCamCmd::FrChangeCamCmd()
: m_isPosition(false), m_isFocalPoint(false), 
  m_isViewUp(false), m_isScale(false) {
    
}

void FrChangeCamCmd::SetFocalPoint(double* value){
    m_FocalPoint[0] = value[0];
    m_FocalPoint[1] = value[1];
    m_FocalPoint[2] = value[2];
    m_isFocalPoint = true;
}

void FrChangeCamCmd::SetPosition(double* value){
    m_Position[0] = value[0];
    m_Position[1] = value[1];
    m_Position[2] = value[2];
    m_isPosition = true;
}

void FrChangeCamCmd::SetViewUp(double* value){
    m_ViewUp[0] = value[0];
    m_ViewUp[1] = value[1];
    m_ViewUp[2] = value[2];
    m_isViewUp = true;
}

void FrChangeCamCmd::SetScale(double value){
    m_Scale = value;
    m_isScale = true;
}

bool FrChangeCamCmd::Execute(){
    if(!this->GetMainController()) return false;
    
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView(); 

    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
    switch(sets->GetActiveView()){
    case ActiveView::Slice:
        SetupCameraSettings(&sets->GetSliceViewSettings().CamSettings);
        break;
    case ActiveView::Mosaic:
        SetupCameraSettings(&sets->GetMosaicViewSettings().CamSettings);
        break;
    case ActiveView::Ortho:
        SetupCameraSettings(&sets->GetOrthoViewSettings().CamSettings);
        break;
    default:
        return false;
    }
    mv->GetCurrentView()->UpdatePipeline(FRP_SETCAM);
    
    return true;
}

void FrChangeCamCmd::SetupCameraSettings(void* settings){
    // Assume that settings can only be camera settings
    CameraSettings* camSettings = (CameraSettings*)settings;
    if(m_isFocalPoint){
        camSettings->FocalPoint[0] = m_FocalPoint[0];
        camSettings->FocalPoint[1] = m_FocalPoint[1];
        camSettings->FocalPoint[2] = m_FocalPoint[2];
    }
    
    if(m_isPosition) {
        camSettings->Position[0] = m_Position[0];
        camSettings->Position[1] = m_Position[1];
        camSettings->Position[2] = m_Position[2];
    }

    if(m_isViewUp){
        camSettings->ViewUp[0] = m_ViewUp[0];
        camSettings->ViewUp[1] = m_ViewUp[1];
        camSettings->ViewUp[2] = m_ViewUp[2];
    }

    if(m_isScale) {
        camSettings->Scale = m_Scale;
    }
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrChangeCamCmd::CanUndo(){
    return false;
}

bool FrChangeCamCmd::Undo(){
    return false;
}

bool FrChangeCamCmd::Redo(){
    return false;
}