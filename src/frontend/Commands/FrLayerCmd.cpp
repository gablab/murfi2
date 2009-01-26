#include "FrLayerCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrLayerDialog.h"
#include "FrLayerListWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"
#include "FrLayerSettings.h"
#include "FrLayerDocObj.h"
#include "FrViewDocObj.h"
#include "FrColormapWidget.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"

#include "Qt/qmessagebox.h"


#define ALL_ITEMS_COUNT 5

FrLayerCmd::FrLayerCmd() 
: m_Action(FrLayerCmd::Undefined), 
  m_DocObj(0), m_isID(false){
}

void FrLayerCmd::SetID(int id){
    m_ID = id;
    m_isID = true;
}

bool FrLayerCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
        case FrLayerCmd::Add: result = AddLayer();
            break;
        case FrLayerCmd::Delete: result = DeleteLayer();
            break;
        case FrLayerCmd::ChangeOld: result = ChangeLayerOld();
            break;
        case FrLayerCmd::ChangeParams: result = ChangeLayerParams();
            break;
        case FrLayerCmd::ChangeColormap: result = ChangeLayerColormap();
            break;
        case FrLayerCmd::UpdateSelectedID: result = UpdateSelectedLayerID();
            break;
        case FrLayerCmd::Undefined:
        default:
            // Do nothing here
            break;
    }
    return result;
}

bool FrLayerCmd::AddLayer(){
    if(m_DocObj == 0) return false;

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);

    bool result = false;
    if(m_DocObj->IsImage()){
        // set id to image layer
        for(int i=0; i < ALL_ITEMS_COUNT; ++i){
            // Do we need to copy values
            // HACK: set default layer id (we have only 1 image layer)
            m_DocObj->SetID(DEF_LAYER_ID);
            layeredImage[i]->AddLayer(m_DocObj->GetID(), FrLayeredImage::Image);
        }
        result = true;
    }
    else if(m_DocObj->IsRoi()){
        for(int i=0; i < ALL_ITEMS_COUNT; ++i){
            // Do we need to copy values ???
            layeredImage[i]->AddLayer(m_DocObj->GetID(), FrLayeredImage::Roi);
        }
        FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
        result = true;
    }
    else if(m_DocObj->IsColormap()){
        for(int i=0; i < ALL_ITEMS_COUNT; ++i){
            // Do we need copy params here???
            layeredImage[i]->AddLayer(m_DocObj->GetID(), FrLayeredImage::Colormap);
        }
        FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
        result = true;
    }
    
    // set new layer as active
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    
    int id = m_DocObj->GetID();
    viewDO->GetSliceViewSettings()->ActiveLayerID = id;
    viewDO->GetMosaicViewSettings()->ActiveLayerID = id;
    viewDO->GetOrthoViewSettings()->ActiveLayerID = id;

    return result;
}

bool FrLayerCmd::DeleteLayer(){
    if(m_DocObj == 0) return false;

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    // delete layer from LayeredImage
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){
        layeredImage[i]->RemoveLayer(m_DocObj->GetID());
    }

    // Assume that there is good sync between layers 
    viewDO->GetSliceViewSettings()->ActiveLayerID = DEF_LAYER_ID;
    viewDO->GetMosaicViewSettings()->ActiveLayerID = DEF_LAYER_ID;
    viewDO->GetOrthoViewSettings()->ActiveLayerID = DEF_LAYER_ID;

    if (m_DocObj->GetID() != DEF_LAYER_ID)
        FrBaseCmd::UpdatePipelineForID(DEF_LAYER_ID, FRP_COLORMAP);

    return true;
}

bool FrLayerCmd::UpdateSelectedLayerID(){
    if(!m_isID || m_ID < DEF_LAYER_ID) return false;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    // update selected layer settings on LayerListWidget's right panel
    viewDO->GetSliceViewSettings()->ActiveLayerID = m_ID;
    viewDO->GetMosaicViewSettings()->ActiveLayerID = m_ID;
    viewDO->GetOrthoViewSettings()->ActiveLayerID = m_ID;
    return true;
}

// change Colormap params with dialog (only for colormap layers)
bool FrLayerCmd::ChangeLayerOld(){
    // Get proper ID
    if(!m_isID){
        m_ID = this->GetActiveLayerID();
    }
    // if wrong ID is specified return
    if(m_ID < DEF_LAYER_ID) return false;

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();    
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    // find layer settings
    // NOTE: assume that layers in different views 
    // but with the same ID are the same
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    // delete layer from LayeredImage
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){
        layeredImage[i]->RemoveLayer(m_ID);
    }

    FrLayerDocObj* layerDO = 0L;
    if(layers.size() > 0){
        // delete appropriate LayerDocObj
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID) break;
        }
    }    

    // Update layer params
    bool result = false;
    if(layerDO != 0L){
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrLayerDialog dlg(mv, true);
        dlg.SetLayerParams(*((FrColormapLayerSettings*)layerDO->GetSettings()));

        if(dlg.SimpleExec()){
            // NOTE: Need to save ID and TBC since they may be changed
            FrColormapLayerSettings* cmlParams = (FrColormapLayerSettings*)layerDO->GetSettings();
            FrTbcSettings saveTbc = cmlParams->TbcSettings;
            dlg.GetLayerParams(*cmlParams);
            cmlParams->TbcSettings = saveTbc;
            layerDO->SetSettings(cmlParams);

            result = true;
        }
        this->UpdateSelectedLayerID();
        mv->GetCurrentView()->UpdatePipeline(FRP_COLORMAP);
    }
    return result;
}

bool FrLayerCmd::ChangeLayerParams(){
    // Get proper ID
    if(!m_isID){
        m_ID = this->GetActiveLayerID();
    }

    FrMainWindow* mw = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // find appropriate layer doc obj
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    FrLayerDocObj* layerDO = 0L;
    if(layers.size() > 0){
        // get appropriate LayerDocObj
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID) 
                break;
            else 
                layerDO = 0L;
        }
    }    

    int opacity = mw->GetLayerListWidget()->GetOpacity();
    FrLayerSettings* params = layerDO->GetSettings();           // probably we should use the old code below
    params->Opacity = opacity/100.0;
    
    bool visibility = mw->GetLayerListWidget()->GetLayerVisibility(m_ID);
    params->Visibility = visibility;

    layerDO->SetSettings(params);

    //// TODO: visibility, name??
    //if (layerDO->IsRoi()){
    //    // get opacity
    //    int opacity = mw->GetLayerListWidget()->GetOpacity();
    //    FrRoiLayerSettings* roiParams = (FrRoiLayerSettings*)layerDO->GetSettings();
    //    roiParams->Opacity = opacity;
    //    
    //    bool visibillity = mw->GetLayerListWidget()->GetLayerVisibility(m_ID);
    //    roiParams->Visibility = visibility;

    //    layerDO->SetSettings(roiParams);
    //}
    //else if (layerDO->IsImage()){
    //    // get opacity
    //    int opacity = mw->GetLayerListWidget()->GetOpacity();
    //    FrImageLayerSettings* imageParams = (FrImageLayerSettings*)layerDO->GetSettings();
    //    imageParams->Opacity = opacity;

    //    bool visibility = mw->GetLayerListWidget()->GetLayerVisibility(m_ID);
    //    imageParams->Visibility = visibility;

    //    layerDO->SetSettings(imageParams);
    //}
    //else if (layerDO->IsColormap()){
    //    // get colormap params and opacity
    //    int opacity = mw->GetLayerListWidget()->GetOpacity();
    //    FrColormapLayerSettings* colormapParams = (FrColormapLayerSettings*)layerDO->GetSettings();
    //    colormapParams->Opacity = opacity;

    //    bool visibility = mw->GetLayerListWidget()->GetLayerVisibility(m_ID);
    //    colormapParams->Visibility = visibility;

    //    mw->GetLayerListWidget()->GetColormapWidget()->GetColormapParams(*colormapParams);
    //    
    //    layerDO->SetSettings(colormapParams);
    //}

    FrBaseCmd::UpdatePipelineForID(m_ID, FRP_OPACITY_VISIBILITY);    
    return true;
}

bool FrLayerCmd::ChangeLayerColormap(){
    // Get proper ID
    if(!m_isID){
        m_ID = this->GetActiveLayerID();
    }

    // Init data
    FrMainWindow* mw = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // find appropriate layer doc obj
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    FrLayerDocObj* layerDO = 0L;
    if(layers.size() > 0){
        // delete appropriate LayerDocObj
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID) break;
        }
    }    

    if (layerDO->IsColormap()){
        // get colormap params and opacity
        int opacity = mw->GetLayerListWidget()->GetOpacity();
        FrColormapLayerSettings* colormapParams = (FrColormapLayerSettings*)layerDO->GetSettings();
        colormapParams->Opacity = opacity;

        bool visibility = mw->GetLayerListWidget()->GetLayerVisibility(m_ID);
        colormapParams->Visibility = visibility;

        mw->GetLayerListWidget()->GetColormapWidget()->GetColormapParams(*colormapParams);
        
        layerDO->SetSettings(colormapParams);
    }
    else
        return false;   // trying to update colormap params of non colormap layer

    FrBaseCmd::UpdatePipelineForID(m_ID, FRP_COLORMAP);
    return true;
}

// delete active layer
int FrLayerCmd::GetActiveLayerID(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    return viewDO->GetActiveLayerID();
}

bool FrLayerCmd::IsRoiLayer(int id){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    
    std::vector<FrDocumentObj*> objects;
    doc->GetObjectsByType(objects, FrDocumentObj::RoiObject);

    // Iterate through all found rois
    std::vector<FrDocumentObj*>::iterator itr, itrEnd(objects.end());
    for(itr = objects.begin(); itr != itrEnd; ++itr){
        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*itr);
        if(id == roiDO->GetID()) return true;
    }
    return false;
}
///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrLayerCmd::CanUndo(){
    return false;
}

bool FrLayerCmd::Undo(){
    return false;
}

bool FrLayerCmd::Redo(){
    return false;
}
