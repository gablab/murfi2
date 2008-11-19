#include "FrLayerCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrLayerDialog.h"
#include "FrLayerListWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"

#include "Qt/qmessagebox.h"


#define ALL_ITEMS_COUNT 5

FrImageLayerCmd::FrImageLayerCmd() 
: m_Action(FrImageLayerCmd::Undefined), m_isID(false){
}

void FrImageLayerCmd::SetID(int id){
    m_ID = id;
    m_isID = true;
}

bool FrImageLayerCmd::Execute(){
    // Check for safety
    if(!this->GetMainController() || 
        m_Action == FrImageLayerCmd::Undefined) return false;

    bool result = false;
    switch(m_Action){
        case FrImageLayerCmd::Add: result = AddLayer();
            break;
        case FrImageLayerCmd::Delete: result = DeleteLayer();
            break;
        case FrImageLayerCmd::ChangeOld: result = ChangeLayerOld();
            break;
        case FrImageLayerCmd::ChangeParams: result = ChangeLayerParams();
            break;
        case FrImageLayerCmd::ChangeColormap: result = ChangeLayerColormap();
            break;
        case FrImageLayerCmd::UpdateSelectedID: result = UpdateSelectedLayerID();
            break;
        default:
            // Do nothing here
            break;
    }
    return result;
}

bool FrImageLayerCmd::AddLayer(){
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayerDialog dlg(mv, true);
    if(!dlg.SimpleExec()) return false;

    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(CORONAL_IMAGE);
    layeredImage[3] = mv->GetOrthoView()->GetImage(SAGITAL_IMAGE);
    layeredImage[4] = mv->GetOrthoView()->GetImage(AXIAL_IMAGE);

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    std::vector<FrLayerSettings*>* otherLayers[ALL_ITEMS_COUNT];
    otherLayers[0] = &tabSets->GetSliceViewSettings()->OtherLayers;
    otherLayers[1] = &tabSets->GetMosaicViewSettings()->OtherLayers;
    otherLayers[2] = &tabSets->GetOrthoViewSettings()->OtherLayers[CORONAL_IMAGE];
    otherLayers[3] = &tabSets->GetOrthoViewSettings()->OtherLayers[SAGITAL_IMAGE];
    otherLayers[4] = &tabSets->GetOrthoViewSettings()->OtherLayers[AXIAL_IMAGE];

    FrLayerSettings layerParams;
    dlg.GetLayerParams(layerParams);

    // we should update layer settings in all views
    // NOTE: Assume that id is the same for all images
    int id;
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){
        id = layeredImage[i]->AddImageLayer();

        FrLayerSettings* layerSets = new FrLayerSettings(layerParams);
        layerSets->ID = id;
        otherLayers[i]->push_back(layerSets);
        //delete layerSets;	// NEW: to prevent memory leaks
    }
    FrBaseCmd::UpdatePipelineForID(id, FRP_COLORMAP);
    return true;
}

bool FrImageLayerCmd::DeleteLayer(){
    // Get proper ID
    if(!m_isID || m_ID == CUR_LAYER_ID){
        m_ID = GetActiveLayerID();
    }
    // if wrong ID is specified return
    // Default layer cannot be deleted
    if(m_ID < DEFAULT_LAYER_ID) return false;
    if(m_ID == DEFAULT_LAYER_ID){
        QMessageBox::critical(this->GetMainController()->GetMainView(), 
            "Delete Layer Command", "Can't delete default layer...");
        return false;
    }

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(CORONAL_IMAGE);
    layeredImage[3] = mv->GetOrthoView()->GetImage(SAGITAL_IMAGE);
    layeredImage[4] = mv->GetOrthoView()->GetImage(AXIAL_IMAGE);

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    std::vector<FrLayerSettings*>* otherLayers[ALL_ITEMS_COUNT];
    otherLayers[0] = &tabSets->GetSliceViewSettings()->OtherLayers;
    otherLayers[1] = &tabSets->GetMosaicViewSettings()->OtherLayers;
    otherLayers[2] = &tabSets->GetOrthoViewSettings()->OtherLayers[CORONAL_IMAGE];
    otherLayers[3] = &tabSets->GetOrthoViewSettings()->OtherLayers[SAGITAL_IMAGE];
    otherLayers[4] = &tabSets->GetOrthoViewSettings()->OtherLayers[AXIAL_IMAGE];

    // Delete layer from all views
    bool isFound = false;
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){
        if(layeredImage[i]->RemoveImageLayer(m_ID)){
            // Find layer
            LayerCollection::iterator it, itEnd(otherLayers[i]->end());
            for(it = otherLayers[i]->begin(); it != itEnd; ++it){
                if((*it)->ID == m_ID) break;
            }
            // remove it
            if(it != itEnd){
                delete (*it);
                otherLayers[i]->erase(it);
                isFound = true;
            }
        }
    }

    if(isFound){
        // Assume that there is good sync between layers 
        tabSets->GetSliceViewSettings()->ActiveLayerID = DEFAULT_LAYER_ID;
        tabSets->GetMosaicViewSettings()->ActiveLayerID = DEFAULT_LAYER_ID;
        tabSets->GetOrthoViewSettings()->ActiveLayerID = DEFAULT_LAYER_ID;

        FrBaseCmd::UpdatePipelineForID(DEFAULT_LAYER_ID, FRP_COLORMAP);
    }
    return true;
}

bool FrImageLayerCmd::UpdateSelectedLayerID(){
    if(!m_isID || m_ID < DEFAULT_LAYER_ID) return false;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

    // update selected layer settings on LayerListWidget's right panel
    tabSets->GetSliceViewSettings()->ActiveLayerID = m_ID;
    tabSets->GetMosaicViewSettings()->ActiveLayerID = m_ID;
    tabSets->GetOrthoViewSettings()->ActiveLayerID = m_ID;
    return true;
}

bool FrImageLayerCmd::ChangeLayerOld(){
    // Get proper ID
    if(!m_isID || m_ID == CUR_LAYER_ID){
        m_ID = this->GetActiveLayerID();
    }
    // if wrong ID is specified return
    if(m_ID < DEFAULT_LAYER_ID) return false;

    // Init data
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    LayerCollection sliceLayers, mosaicLayers, 
                    orthoLayers0, orthoLayers1, orthoLayers2;
    GetLayerSettings(tabSets->GetSliceViewSettings(), sliceLayers);
    GetLayerSettings(tabSets->GetMosaicViewSettings(), mosaicLayers);
    GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers0, 0);
    GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers1, 1);
    GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers2, 2);

    LayerCollection* otherLayers[ALL_ITEMS_COUNT];    
    otherLayers[0] = &sliceLayers;
    otherLayers[1] = &mosaicLayers;
    otherLayers[2] = &orthoLayers0;
    otherLayers[3] = &orthoLayers1;
    otherLayers[4] = &orthoLayers2;

    // find layer settings
    // NOTE: assume that layers in different views 
    // but with the same ID are the same
    FrLayerSettings* layerSets = 0L;
    LayerCollection::iterator it, itEnd(otherLayers[0]->end());
    for(it = otherLayers[0]->begin(); it != itEnd; ++it){
        if((*it)->ID == m_ID){
            layerSets = (*it);
            break;
        }
    }

    // Update layer params
    bool result = false;
    if(layerSets != 0L){
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrLayerDialog dlg(mv, true);
        dlg.SetLayerParams(*layerSets);

        if(dlg.SimpleExec()) {
            // NOTE: Need to save ID and TBC since they may be changed
            int saveId = layerSets->ID;
            FrTBCSettings saveTbc = layerSets->TbcSettings;
            dlg.GetLayerParams(*layerSets);
            layerSets->ID = saveId;
            layerSets->TbcSettings = saveTbc;

            for(int i=0; i < ALL_ITEMS_COUNT; ++i){
                LayerCollection::iterator itr, itrEnd(otherLayers[i]->end());
                for(itr = otherLayers[i]->begin(); itr != itrEnd; ++itr){
                    if((*itr)->ID == layerSets->ID){
                        FrLayerSettings& ls = *(*itr);
                        // Update params
                        ls.Name = layerSets->Name;
                        ls.Opacity = layerSets->Opacity;
                        ls.Visibility = layerSets->Visibility;
                        ls.ColormapSettings.Type = layerSets->ColormapSettings.Type;
                        ls.ColormapSettings.MinValue = layerSets->ColormapSettings.MinValue;
                        ls.ColormapSettings.MaxValue = layerSets->ColormapSettings.MaxValue;
                        ls.ColormapSettings.MidValue = layerSets->ColormapSettings.MidValue;
                        ls.ColormapSettings.Threshold = layerSets->ColormapSettings.Threshold;
                        ls.ColormapSettings.Color = layerSets->ColormapSettings.Color;
                        break;
                    }
                }
            }
            result = true;
        }
        this->UpdateSelectedLayerID();
        mv->GetCurrentView()->UpdatePipeline(FRP_COLORMAP);
    }
    return result;
}

bool FrImageLayerCmd::ChangeLayerParams(){
    // Get proper ID
    if(!m_isID || m_ID == CUR_LAYER_ID){
        m_ID = this->GetActiveLayerID();
    }

    FrMainWindow* mw = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    if(this->IsRoiLayer(m_ID)){
        // Update ROI layer
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        std::vector<FrDocumentObj*> objects;
        doc->GetObjectsByType(objects, FrDocumentObj::RoiObject);

        FrLayerSettings ls;
        if(!mw->GetLayerListWidget()->GetLayerParams(m_ID, ls)) return false;

        // Iterate through all found rois
        std::vector<FrDocumentObj*>::iterator itr, itrEnd(objects.end());
        for(itr = objects.begin(); itr != itrEnd; ++itr){
            FrRoiDocObj* roiDO = (FrRoiDocObj*)(*itr);
            if(roiDO->GetID() == m_ID) {
                roiDO->SetVisibility(ls.Visibility);
                roiDO->SetOpacity(ls.Opacity);
                roiDO->SetName(ls.Name);
                break;
            }
        }
    }
    else {
        // Update colormap layer

        // Init data
        FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
        LayerCollection sliceLayers, mosaicLayers, 
                        orthoLayers0, orthoLayers1, orthoLayers2;
        GetLayerSettings(tabSets->GetSliceViewSettings(), sliceLayers);
        GetLayerSettings(tabSets->GetMosaicViewSettings(), mosaicLayers);
        GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers0, 0);
        GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers1, 1);
        GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers2, 2);

        LayerCollection* otherLayers[ALL_ITEMS_COUNT];    
        otherLayers[0] = &sliceLayers;
        otherLayers[1] = &mosaicLayers;
        otherLayers[2] = &orthoLayers0;
        otherLayers[3] = &orthoLayers1;
        otherLayers[4] = &orthoLayers2;

        // Get layer params
        FrLayerSettings ls;
        if(!mw->GetLayerListWidget()->GetLayerParams(m_ID, ls)) return false;

        // Update simple params
        for(int i=0; i < ALL_ITEMS_COUNT; ++i){
            LayerCollection::iterator it, itEnd(otherLayers[i]->end());
            for(it = otherLayers[i]->begin(); it != itEnd; ++it){
                if((*it)->ID == m_ID){
                    (*it)->Visibility = ls.Visibility;
                    (*it)->Opacity = ls.Opacity;
                    (*it)->Name = ls.Name;
                    break;
                }
            }
        }
    }
    FrBaseCmd::UpdatePipelineForID(m_ID, FRP_OPACITY_VISIBILITY);    
    return true;
}

bool FrImageLayerCmd::ChangeLayerColormap(){
    // Get proper ID
    if(!m_isID || m_ID == CUR_LAYER_ID){
        m_ID = this->GetActiveLayerID();
    }

    // Init data
    FrMainWindow* mw = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    LayerCollection sliceLayers, mosaicLayers, 
                    orthoLayers0, orthoLayers1, orthoLayers2;
    GetLayerSettings(tabSets->GetSliceViewSettings(), sliceLayers);
    GetLayerSettings(tabSets->GetMosaicViewSettings(), mosaicLayers);
    GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers0, 0);
    GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers1, 1);
    GetLayerSettings(tabSets->GetOrthoViewSettings(), orthoLayers2, 2);

    LayerCollection* otherLayers[ALL_ITEMS_COUNT];    
    otherLayers[0] = &sliceLayers;
    otherLayers[1] = &mosaicLayers;
    otherLayers[2] = &orthoLayers0;
    otherLayers[3] = &orthoLayers1;
    otherLayers[4] = &orthoLayers2;

    FrLayerSettings ls;
    if(!mw->GetLayerListWidget()->GetLayerParams(m_ID, ls)) return false;

    // Update simple params
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){
        LayerCollection::iterator it, itEnd(otherLayers[i]->end());
        for(it = otherLayers[i]->begin(); it != itEnd; ++it){
            if((*it)->ID == m_ID){
                (*it)->ColormapSettings = ls.ColormapSettings;
                break;
            }
        }
    }
    FrBaseCmd::UpdatePipelineForID(m_ID, FRP_COLORMAP);
    return true;
}

// delete active layer
int FrImageLayerCmd::GetActiveLayerID(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

    int result = BAD_LAYER_ID;
    switch(tabSets->GetActiveView()){
        case FrTabSettingsDocObj::SliceView: 
            result = tabSets->GetSliceViewSettings()->ActiveLayerID;
            break;
        case FrTabSettingsDocObj::MosaicView: 
            result = tabSets->GetMosaicViewSettings()->ActiveLayerID;
            break;
        case FrTabSettingsDocObj::OrthoView: 
            result = tabSets->GetOrthoViewSettings()->ActiveLayerID;
            break;
    }
    return result;
}

bool FrImageLayerCmd::IsRoiLayer(int id){
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
bool FrImageLayerCmd::CanUndo(){
    return false;
}

bool FrImageLayerCmd::Undo(){
    return false;
}

bool FrImageLayerCmd::Redo(){
    return false;
}
