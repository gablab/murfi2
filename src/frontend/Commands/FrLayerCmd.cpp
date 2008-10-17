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
    if(!m_isID) return false;
    return false;
}

bool FrLayerCmd::ChangeLayer(){
    if(!m_isID) return false;
        
    // get layers of active view
    int* ptrToActiveID = 0L;
    LayerCollection layers;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    switch(tabSets->GetActiveView()){
        case FrTabSettingsDocObj::SliceView:
            GetLayerSettings(tabSets->GetSliceViewSettings(), layers);
            ptrToActiveID = &tabSets->GetSliceViewSettings()->ActiveLayerID;
            break;
        case FrTabSettingsDocObj::MosaicView:
            GetLayerSettings(tabSets->GetMosaicViewSettings(), layers);
            ptrToActiveID = &tabSets->GetMosaicViewSettings()->ActiveLayerID;
            break;
        case FrTabSettingsDocObj::OrthoView:
            GetLayerSettings(tabSets->GetOrthoViewSettings(), layers, CORONAL_SLICE);
            ptrToActiveID = &tabSets->GetOrthoViewSettings()->ActiveLayerID;
            break;
    }
    if(layers.size() <= 0 ) return false;

    // Find layer by id
    FrLayerSettings* layerSets = 0L;
    LayerCollection::iterator it, itEnd(layers.end());

    if(m_ID == CUR_LAYER_ID) m_ID = *ptrToActiveID;
    for(it = layers.begin(); it != itEnd; ++it){
        if((*it)->ID == m_ID){
            layerSets = (*it);
            break;
        }
    }

    // Get new values and set them
    bool result = false;
    if(layerSets){
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrLayerDialog dlg(mv, true);
        dlg.SetLayerParams(*layerSets);

        if(dlg.SimpleExec()) {
            // Need to save ID since it may be broken
            int oldID = layerSets->ID;
            dlg.GetLayerParams(*layerSets);
            layerSets->ID = oldID;

            if(tabSets->GetActiveView() == FrTabSettingsDocObj::OrthoView){
                // TODO: implement update
            }
            
            *ptrToActiveID = layerSets->ID;
            mv->GetCurrentView()->UpdatePipeline(FRP_COLORMAP);
            result = true;
        }
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