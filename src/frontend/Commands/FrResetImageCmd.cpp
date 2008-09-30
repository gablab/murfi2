#include "FrResetImageCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrCommandController.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "Fr2DSliceActor.h"

// VTK stuff
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"


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
            case ActiveView::Slice: targetView = FrResetImageCmd::Slice;
                break;
            case ActiveView::Mosaic: targetView = FrResetImageCmd::Mosaic;
                break;
            case ActiveView::Ortho: targetView = FrResetImageCmd::Ortho;
                break;
        }
    }

    // Get actors and renderers
    std::vector<Fr2DSliceActor*> actors;
    std::vector<vtkRenderer*> renderers;
    switch(targetView){
         case FrResetImageCmd::Slice:
             actors.push_back(mv->GetSliceView()->GetActor());
             renderers.push_back(mv->GetSliceView()->GetRenderer());
             break;
         case FrResetImageCmd::Mosaic:
             actors.push_back(mv->GetSliceView()->GetActor());
             renderers.push_back(mv->GetSliceView()->GetRenderer());
             break;
         case FrResetImageCmd::Ortho:
             for(int i=0; i < RENDERER_COUNT-1; ++i){
                actors.push_back(mv->GetOrthoView()->GetActor(i));
                renderers.push_back(mv->GetOrthoView()->GetRenderer(i));
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

void FrResetImageCmd::ResetCamera(Fr2DSliceActor* actor, vtkRenderer* renderer){
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

    //// Calc scale now
    //double origin[3];
    //actor->GetOrigin(origin);
    //double actorWidth  = (imgCenter[0] - origin[0]) * 2;
    //double actorHeight = (imgCenter[1] - origin[1]) * 2;
    //double winWidth  = renderer->GetSize()[0];
    //double winHeight = renderer->GetSize()[1];
    
    double aspect[2];
    renderer->ComputeAspect();
    renderer->GetAspect(aspect);
    
    double sc = renderer->GetSize()[0] / aspect[0];
    double scale = renderer->GetActiveCamera()->GetParallelScale();
    renderer->GetActiveCamera()->SetParallelScale(scale);
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