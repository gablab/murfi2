#include "FrLayerCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrLayerDialog.h"
#include "FrTabSettingsDocObj.h"
#include "FrMyLayeredImage.h"
#include "FrUtils.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"

#include "Qt/QMessageBox.h"

#define ALL_ITEMS_COUNT 5

FrLayerCmd::FrLayerCmd() 
: m_Action(FrLayerCmd::Undefined), m_isID(false){
}

void FrLayerCmd::SetID(int id){
    m_ID = id;
    m_isID = true;
}

bool FrLayerCmd::Execute(){
    // Check for safety
    if(!this->GetMainController() || 
        m_Action == FrLayerCmd::Undefined) return false;

    bool result = false;
    switch(m_Action){
        case FrLayerCmd::Add: 
            result = AddLayer();
            break;
        case FrLayerCmd::Delete: 
            result = DeleteLayer();
            break;
        case FrLayerCmd::Change: 
            result = ChangeLayer();
            break;
        case FrLayerCmd::UpdateSelectedID: 
            result = UpdateSelectedLayerID();
            break;
        default:
            // Do nothing here
            break;
    }
	return result;
}

bool FrLayerCmd::AddLayer(){
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayerDialog dlg(mv, true);
	if(!dlg.SimpleExec()) return false;
	
    FrMyLayeredImage* layeredImage[ALL_ITEMS_COUNT];
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
        id = layeredImage[i]->AddLayer();
                
        FrLayerSettings* layerSets = new FrLayerSettings(layerParams);
        layerSets->ID = id;
        otherLayers[i]->push_back(layerSets);
    }
    FrBaseCmd::UpdatePipelineForID(id, FRP_COLORMAP);
}

bool FrLayerCmd::DeleteLayer(){
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
    FrMyLayeredImage* layeredImage[ALL_ITEMS_COUNT];
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
        if(layeredImage[i]->RemoveLayer(m_ID)){
            // Find layer
            LayerCollection::iterator it, itEnd(otherLayers[i]->end());
            for(it = otherLayers[i]->begin(); it != itEnd; ++it){
                if((*it)->ID == m_ID) break;
            }
            // remove and update layer IDs            
            if(it != itEnd){
                delete (*it);
                otherLayers[i]->erase(it);

                int newID = 1;
                LayerCollection::iterator it, itEnd(otherLayers[i]->end());
                for(it = otherLayers[i]->begin(); it != itEnd; ++it){
                    (*it)->ID = newID++;
                }
                isFound = true;
            }
        }
    }

    if(isFound){
        // Assume that there is good sync between layers 
        tabSets->GetSliceViewSettings()->ActiveLayerID--;
        tabSets->GetMosaicViewSettings()->ActiveLayerID--;
        tabSets->GetOrthoViewSettings()->ActiveLayerID--;

        FrBaseCmd::UpdatePipelineForID(0, FRP_COLORMAP);
    }
    return true;
}

bool FrLayerCmd::UpdateSelectedLayerID(){
    if(!m_isID || m_ID < DEFAULT_LAYER_ID) return false;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

    tabSets->GetSliceViewSettings()->ActiveLayerID = m_ID;
    tabSets->GetMosaicViewSettings()->ActiveLayerID = m_ID;
    tabSets->GetOrthoViewSettings()->ActiveLayerID = m_ID;
}

bool FrLayerCmd::ChangeLayer(){
    // Get proper ID
    if(!m_isID || m_ID == CUR_LAYER_ID){
        m_ID = GetActiveLayerID();
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
                        // Just keep TBC
                        saveTbc = ls.TbcSettings;
                        ls = *layerSets;
                        ls.TbcSettings = saveTbc;
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

// delete active layer
int FrLayerCmd::GetActiveLayerID(){
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