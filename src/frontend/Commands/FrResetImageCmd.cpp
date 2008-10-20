#include "FrResetImageCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrCommandController.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrLayeredImage.h"

// VTK stuff
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageActor.h"

#define DEF_DELTA 10

FrResetImageCmd::FrResetImageCmd()
: m_TargetView(FrResetImageCmd::Unknown){
}

bool FrResetImageCmd::Execute(){
    if(!this->GetMainController() || 
        m_TargetView == FrResetImageCmd::Unknown) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tsDO = doc->GetCurrentTabSettings();
    
    // Determine target view
    FrResetImageCmd::View targetView = m_TargetView;
    if(targetView == FrResetImageCmd::Current){
        switch(tsDO->GetActiveView()){
            case FrTabSettingsDocObj::SliceView: targetView = FrResetImageCmd::Slice;
                break;
            case FrTabSettingsDocObj::MosaicView: targetView = FrResetImageCmd::Mosaic;
                break;
            case FrTabSettingsDocObj::OrthoView: targetView = FrResetImageCmd::Ortho;
                break;
        }
    }

    // Get actors and renderers
    std::vector<vtkImageActor*> actors;
	std::vector<vtkRenderer*> renderers;
    std::vector<FrCameraSettings*> camSettings;
    switch(targetView){
         case FrResetImageCmd::Slice:
             actors.push_back(mv->GetSliceView()->GetImage()->GetActor());
             renderers.push_back(mv->GetSliceView()->GetImage()->GetRenderer());
             camSettings.push_back(&tsDO->GetSliceViewSettings()->CamSettings);
             break;
         case FrResetImageCmd::Mosaic:
             actors.push_back(mv->GetMosaicView()->GetImage()->GetActor());
             renderers.push_back(mv->GetMosaicView()->GetImage()->GetRenderer());
             camSettings.push_back(&tsDO->GetMosaicViewSettings()->CamSettings);
             break;
         case FrResetImageCmd::Ortho:
             for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                actors.push_back(mv->GetOrthoView()->GetImage(i)->GetActor());
                renderers.push_back(mv->GetOrthoView()->GetImage(i)->GetRenderer());
                camSettings.push_back(&tsDO->GetOrthoViewSettings()->CamSettings[i]);
             }
             break;         
         default:
             // Do noting
             break;
     };

    // Performe reset action
    bool result = false;
    for(int i=0; i < actors.size(); ++i){
        ResetCamera(camSettings[i], actors[i], renderers[i]);
        result = true;
    }

    if(result){
        mv->GetCurrentView()->UpdatePipeline(FRP_SETCAM);
    }
    return result;
}

void FrResetImageCmd::ResetCamera(FrCameraSettings* camSets,
                                  vtkImageActor* actor, 
                                  vtkRenderer* renderer){
    // Up vector directed along Y axis
    double newViewUp[3];
    newViewUp[0] = 0.0;
    newViewUp[1] = 1.0;
    newViewUp[2] = 0.0;

    // Move camera at center of image
    double newFocalPt[3];
    double newPosition[3];
    double* imgCenter = actor->GetCenter();
    newFocalPt[0] = newPosition[0] = imgCenter[0];
    newFocalPt[1] = newPosition[1] = imgCenter[1];
    newFocalPt[2] = imgCenter[2];
    newPosition[2] = imgCenter[2] + 1.0;
    
    // Calculate scale 
    double bounds[6];
    actor->GetBounds(bounds);    
    double width = bounds[1] - bounds[0];
    double height = bounds[3] - bounds[2];
        
    double newScale = std::max(width, height) / 2;
    newScale = newScale + DEF_DELTA;

    COPY_ARR3(camSets->FocalPoint, newFocalPt);
    COPY_ARR3(camSets->Position, newPosition);
    COPY_ARR3(camSets->ViewUp, newViewUp);
    camSets->Scale = newScale;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrResetImageCmd::CanUndo(){
    return false;
}

bool FrResetImageCmd::Undo(){
    return false;
}

bool FrResetImageCmd::Redo(){
    return false;
}