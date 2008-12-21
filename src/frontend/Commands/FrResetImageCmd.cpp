#include "FrResetImageCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrCommandController.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrLayeredImage.h"
#include "FrUtils.h"
#include "FrViewDocObj.h"
#include "FrLayerDocObj.h"

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

    // Reset TBC
    this->ResetTBC();

    // Get camera settings
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    std::vector<vtkImageActor*> actors;
    std::vector<FrCameraSettings*> camSettings;
    switch(viewDO->GetActiveView()){
        case SliceView:
            actors.push_back(mv->GetSliceView()->GetImage()->GetActor());
            camSettings.push_back(&viewDO->GetSliceViewSettings()->CamSettings);
        break;
        case MosaicView:
            actors.push_back(mv->GetMosaicView()->GetImage()->GetActor());
            camSettings.push_back(&viewDO->GetMosaicViewSettings()->CamSettings);
        break;
        case OrthoView:
            for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
                actors.push_back(mv->GetOrthoView()->GetImage(i)->GetActor());
                camSettings.push_back(&viewDO->GetOrthoViewSettings()->CamSettings[i]);
            }
        break;
        default:
        // Do noting
        break;
     }

    // Performe reset action
    bool result = false;
    for(int i=0; i < camSettings.size(); ++i){
        this->ResetCamera(camSettings[i], actors[i]);
        result = true;
    }

    if(result){
        mv->GetCurrentView()->UpdatePipeline(FRP_TBC);
    }
    return result;
}

void FrResetImageCmd::ResetCamera(FrCameraSettings* camSets, vtkImageActor* actor){
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

void FrResetImageCmd::ResetTBC(){
    // Reset tBC to defaults
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    std::vector<FrDocumentObj*> layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);

    // Iterate through all found layers
    std::vector<FrDocumentObj*>::iterator it, itEnd(layers.end());

    for(it = layers.begin(); it != itEnd; ++it){
        FrLayerDocObj* layerDO = (FrLayerDocObj*)(*it);

        if (layerDO->IsRoi()){
            // do nothing
        }
        else if (layerDO->IsImage()){
            FrImageLayerSettings* imageParams = (FrImageLayerSettings*)layerDO->GetSettings();

            imageParams->TbcSettings.Threshold  = DEF_TBC_THRESHOLD;
            imageParams->TbcSettings.Brightness = DEF_TBC_BRIGHTNESS;
            imageParams->TbcSettings.Contrast   = DEF_TBC_CONTRAST;

            layerDO->SetSettings(imageParams);
        }
        else if (layerDO->IsColormap()){
            FrColormapLayerSettings* colormapParams = (FrColormapLayerSettings*)layerDO->GetSettings();
            
            colormapParams->TbcSettings.Threshold  = DEF_TBC_THRESHOLD;
            colormapParams->TbcSettings.Brightness = DEF_TBC_BRIGHTNESS;
            colormapParams->TbcSettings.Contrast   = DEF_TBC_CONTRAST;

            layerDO->SetSettings(colormapParams);
        }
    }
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
