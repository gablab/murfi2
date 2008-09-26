#include "FrChangeCamCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrOrthoView.h"
#include "FrTabSettingsDocObj.h"

#include "vtkRenderer.h"

// Defines
#define UNDEF_RENDERER_INDEX -1


FrChangeCamCmd::FrChangeCamCmd()
: m_isPosition(false), m_isFocalPoint(false), 
  m_isViewUp(false), m_isScale(false), m_Renderer(0) {
    
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

bool FrChangeCamCmd::Execute(){
    if(!this->GetMainController()) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // Suppose command is working well in comman case
    // bad in case of unknown active view or if ortho view has some problems
    bool result = true;
    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
    switch(sets->GetActiveView()){
    case ActiveView::Slice:
        SetupCameraSettings(&sets->GetSliceViewSettings().CamSettings);
        break;
    case ActiveView::Mosaic:
        SetupCameraSettings(&sets->GetMosaicViewSettings().CamSettings);
        break;
    case ActiveView::Ortho:
        result = SetupOrthoViewCamSettings();
        break;
    default:
        result = false;
    }
    mv->GetCurrentView()->UpdatePipeline(FRP_SETCAM);
    
    return result;
}

void FrChangeCamCmd::SetupCameraSettings(void* settings){
    // Assume that settings can only be camera settings
    CameraSettings* camSettings = (CameraSettings*)settings;
    if(m_isFocalPoint){
        camSettings->FocalPoint[0] = m_FocalPoint[0];
        camSettings->FocalPoint[1] = m_FocalPoint[1];
        camSettings->FocalPoint[2] = m_FocalPoint[2];
        m_isFocalPoint = false;
    }
    
    if(m_isPosition) {
        camSettings->Position[0] = m_Position[0];
        camSettings->Position[1] = m_Position[1];
        camSettings->Position[2] = m_Position[2];
        m_isPosition = false;
    }

    if(m_isViewUp){
        camSettings->ViewUp[0] = m_ViewUp[0];
        camSettings->ViewUp[1] = m_ViewUp[1];
        camSettings->ViewUp[2] = m_ViewUp[2];
        m_isViewUp = false;
    }

    if(m_isScale) {
        camSettings->Scale = m_Scale;
        m_isScale = false;
    }
}

bool FrChangeCamCmd::SetupOrthoViewCamSettings(){
    
    if(m_Renderer == 0L) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrOrthoView* orthoView = mv->GetOrthoView();
    
    // Find working renderer, default is CORONAL
    int rendererIndex = UNDEF_RENDERER_INDEX;
    if(m_Renderer == orthoView->GetRenderer(CORONAL_RENDERER)){
        rendererIndex = CORONAL_RENDERER;
    }
    else if(m_Renderer == orthoView->GetRenderer(SAGITAL_RENDERER)){
        rendererIndex = SAGITAL_RENDERER;
    }
    else if(m_Renderer == orthoView->GetRenderer(AXIAL_RENDERER)){
        rendererIndex = AXIAL_RENDERER;
    }
    
    bool result = false;
    if(rendererIndex != UNDEF_RENDERER_INDEX){
        // Change info in document
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        OViewSettings& settings = doc->GetCurrentTabSettings()->GetOrthoViewSettings();
        SetupCameraSettings(&settings.CamSettings[rendererIndex]);
        result = true;
    }
    return result;
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