#include "FrResetImageCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrCommandController.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrMyLayeredImage.h"

// VTK stuff
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageActor.h"


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
    switch(targetView){
         case FrResetImageCmd::Slice:
             actors.push_back(mv->GetSliceView()->GetImage()->GetActor());
             renderers.push_back(mv->GetSliceView()->GetImage()->GetRenderer());
             break;
         case FrResetImageCmd::Mosaic:
             actors.push_back(mv->GetMosaicView()->GetImage()->GetActor());
             renderers.push_back(mv->GetMosaicView()->GetImage()->GetRenderer());
             break;
         case FrResetImageCmd::Ortho:
             for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                actors.push_back(mv->GetOrthoView()->GetImage(i)->GetActor());
                renderers.push_back(mv->GetOrthoView()->GetImage(i)->GetRenderer());
             }
             break;         
         default:
             // Do noting
             break;
     };

    // Performe reset action
    bool result = false;
    for(int i=0; i < actors.size(); ++i){
        ResetCamera(actors[i], renderers[i]);
        result = true;
    }
    return result;
}

void FrResetImageCmd::ResetCamera(vtkImageActor* actor, vtkRenderer* renderer){		// test
    // Move camera at center of image
    double newFocalPt[3];
    double newPosition[3];
    double* imgCenter = actor->GetCenter();
    newFocalPt[0] = newPosition[0] = imgCenter[0];
    newFocalPt[1] = newPosition[1] = imgCenter[1];
    newFocalPt[2] = imgCenter[2];
    newPosition[2] = imgCenter[2] + 1.0;

    renderer->GetActiveCamera()->SetFocalPoint(newFocalPt);
    renderer->GetActiveCamera()->SetPosition(newPosition);
    renderer->Render();

    // Calculate scale 
    double bounds[6];
    actor->GetBounds(bounds);    
    double width = bounds[1] - bounds[0];
    double height = bounds[3] - bounds[2];
    
    int scale = std::max(int(bounds[1]), int(bounds[3])) / 2;
    renderer->GetActiveCamera()->SetParallelScale(scale + 1);
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