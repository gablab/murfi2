#include "FrChangeCamCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrOrthoView.h"
#include "FrTabSettingsDocObj.h"
#include "FrMyLayeredImage.h"

#include "vtkRenderer.h"

// Defines
#define INVALIDE_IMAGE_NUM -1


FrChangeCamCmd::FrChangeCamCmd()
: m_isPosition(false), m_isFocalPoint(false), 
  m_isViewUp(false), m_isScale(false), m_isXY(false) {
    
}

void FrChangeCamCmd::SetFocalPoint(double* value){
    m_FocalPoint[0] = value[0];
    m_FocalPoint[1] = value[1];
    m_FocalPoint[2] = value[2];
    m_isFocalPoint = true;
}

void FrChangeCamCmd::SetFocalPoint(double x, double y, double z){
    m_FocalPoint[0] = x;
    m_FocalPoint[1] = y;
    m_FocalPoint[2] = z;
    m_isFocalPoint = true;
}

void FrChangeCamCmd::SetPosition(double* value){
    m_Position[0] = value[0];
    m_Position[1] = value[1];
    m_Position[2] = value[2];
    m_isPosition = true;
}

void FrChangeCamCmd::SetPosition(double x, double y, double z){
    m_Position[0] = x;
    m_Position[1] = y;
    m_Position[2] = z;
    m_isPosition = true;
}

void FrChangeCamCmd::SetViewUp(double* value){
    m_ViewUp[0] = value[0];
    m_ViewUp[1] = value[1];
    m_ViewUp[2] = value[2];
    m_isViewUp = true;
}

void FrChangeCamCmd::SetViewUp(double x, double y, double z){
    m_ViewUp[0] = x;
    m_ViewUp[1] = y;
    m_ViewUp[2] = z;
    m_isViewUp = true;
}

void FrChangeCamCmd::SetScale(double value){
    m_Scale = value;
    m_isScale = true;
}

void FrChangeCamCmd::SetMouseXY(int x, int y){
    m_X = x; m_Y = y;
    m_isXY = true;
}

bool FrChangeCamCmd::Execute(){
    if(!this->GetMainController()) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // Suppose command is working well in common case
    // bad in case of unknown active view or if ortho view has some problems
    bool result = true;
    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
    switch(sets->GetActiveView()){
    case FrTabSettingsDocObj::SliceView:
        SetupCameraSettings(sets->GetSliceViewSettings()->CamSettings);
        break;
    case FrTabSettingsDocObj::MosaicView:
        SetupCameraSettings(sets->GetMosaicViewSettings()->CamSettings);
        break;
    case FrTabSettingsDocObj::OrthoView:
        result = SetupOrthoViewCamSettings();
        break;
    default:
        result = false;
    }
    m_isFocalPoint = m_isPosition = m_isViewUp = m_isScale = m_isXY = false;
    mv->GetCurrentView()->UpdatePipeline(FRP_SETCAM);
    
    return result;
}

bool FrChangeCamCmd::SetupOrthoViewCamSettings(){
    
    if( !m_isXY ) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrOrthoView* orthoView = mv->GetOrthoView();
    
    // Find working renderer
    // Find Image where click's occured
    int imgIndex = INVALIDE_IMAGE_NUM;
    for(int idx=0; idx < ORTHO_IMAGE_COUNT; ++idx){
        if (orthoView->GetImage(idx)->GetRenderer()->IsInViewport(m_X, m_Y)){
            imgIndex = idx; break;
        }
    }
    
    bool result = false;
    if(imgIndex != INVALIDE_IMAGE_NUM){
        // Change info in document
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        FrOrthoViewSettings* settings = doc->GetCurrentTabSettings()->GetOrthoViewSettings();
        SetupCameraSettings(settings->CamSettings[imgIndex]);
        result = true;
    }
    return result;
}

void FrChangeCamCmd::SetupCameraSettings(FrCameraSettings& settings){
    
    if(m_isFocalPoint){
        COPY_ARR3(settings.FocalPoint, m_FocalPoint);
    }
    
    if(m_isPosition) {
        COPY_ARR3(settings.Position, m_Position);
    }

    if(m_isViewUp){
        COPY_ARR3(settings.ViewUp, m_ViewUp);
    }

    if(m_isScale) {
        settings.Scale = m_Scale;
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