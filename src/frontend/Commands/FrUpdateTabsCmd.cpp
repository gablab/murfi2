#include "FrUpdateTabsCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrBookmarkWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayerDocObj.h"
#include "FrViewDocObj.h"
#include "FrLayeredImage.h"
#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"

#define ITEMS_NUM  5


FrUpdateTabsCmd::FrUpdateTabsCmd()
: m_Action(FrUpdateTabsCmd::None),
  m_TabSettingsDocObj(0L), 
  m_TabID(BAD_TAB_ID) {

}

bool FrUpdateTabsCmd::Execute(){
    // Check params
    if(!this->GetMainController() || m_Action == FrUpdateTabsCmd::None) return false;

    bool result = false;
    if(m_Action == FrUpdateTabsCmd::SetCurrentTab){
        result = this->SetupTab();
    }
    else {
        result = this->UpdateView();
    }
    return result;
}

bool FrUpdateTabsCmd::SetupTab(){
    // check params
    if(m_TabID == BAD_TAB_ID) return false;

    // Update current tab in document!
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    if(m_TabID != CURRENT_TAB_ID){
        // First save all settings to prev tab if any
        std::vector<FrDocumentObj*> docTabs;
        doc->GetObjectsByType(docTabs, FrDocumentObj::TabSettings);

        std::vector<FrDocumentObj*>::iterator it, itEnd(docTabs.end());
        for(it = docTabs.begin(); it != itEnd; ++it){
            FrTabSettingsDocObj* tsDO = (FrTabSettingsDocObj*)(*it);
            tsDO->SetIsCurrent( (tsDO->GetID() == m_TabID) );
        }
    }

    // Setup Tab objects (layers, settings etc)
    this->SetupLayeredImage();
    this->SetupTabObjects();
    FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_FULL);

    return true;
}

bool FrUpdateTabsCmd::UpdateView(){
    // check params
    if(m_TabSettingsDocObj == 0L) return false;
    // Performe action in view
    FrBookmarkWidget* bmWidget = 
        this->GetMainController()->GetMainView()->GetBookmarkWidget();

    bool result = false;
    switch(m_Action){
        case FrUpdateTabsCmd::Add:
            result = bmWidget->AddBookmark(m_TabSettingsDocObj);
            break;
        case FrUpdateTabsCmd::Remove: 
            result = bmWidget->RemoveBookmark(m_TabSettingsDocObj);
            break;
        case FrUpdateTabsCmd::Update:
            // TODO: Implement !!!
            //result = bmWidget->UpdateTab(m_TabSettingsDocObj);
            break;
    }
    return result;
}


void FrUpdateTabsCmd::SetupTabObjects(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

    // Remove Image and Colormap layers
    FrDocument::DocObjCollection oldLayers;
    doc->GetObjectsByType(oldLayers, FrDocumentObj::LayerObject);
    
    FrDocument::DocObjCollection::iterator it, itEnd(oldLayers.end());
    for(it = oldLayers.begin(); it != itEnd; ++it){
        FrLayerDocObj* layerDO = (FrLayerDocObj*)(*it);
        // NOTE: do not touch ROI layers
        if(!layerDO->IsRoi()) 
            doc->Remove(layerDO);
    }

    // Remove view settings
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);
    for(it = views.begin(); it != views.end(); ++it){
        doc->Remove(*it);
    }

    ////////////////////////////
    // Add new
    FrViewDocObj* viewDO = new FrViewDocObj();
    viewDO->CopySettingsFrom(tabSets);
    doc->Add(viewDO);

    FrLayerDocObj* imgLayer = new FrLayerDocObj(tabSets->GetImageLayer()->GetType());
    imgLayer->CopySettings(tabSets->GetImageLayer());
    doc->Add(imgLayer);
    
    FrTabSettingsDocObj::LayersCollection& layers = tabSets->GetLayers();
    std::vector<FrLayerSettings*>::iterator itr, itrEnd(layers.end());
    for(itr = layers.begin(); itr != itrEnd; ++itr){
        // TODO: do not add roi(image) layers, may be just copy settings  ??
        if ((*itr)->GetType() != FrLayerSettings::LRoi){
            FrLayerDocObj* layer = new FrLayerDocObj((*itr)->GetType());
            layer->CopySettings((*itr));
            doc->Add(layer);
        }
    }   
}

void FrUpdateTabsCmd::SetupLayeredImage(){
    //// Init data
    //FrMainWindow* mv = this->GetMainController()->GetMainView();
    //FrMainDocument* doc = this->GetMainController()->GetMainDocument();
 
    //FrLayeredImage* layeredImage[ITEMS_NUM];
    //layeredImage[0] = mv->GetSliceView()->GetImage();
    //layeredImage[1] = mv->GetMosaicView()->GetImage();
    //layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    //layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    //layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);    

    //// remove all colormap layers from layered image in all views
    // for(int i = 0; i < ITEMS_NUM; i++){
    //    layeredImage[i]->RemoveColormapLayers();
    //}

    //FrDocument::DocObjCollection layers;
    //doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);
    //
    //// add only colormap layers
    //FrDocument::DocObjCollection::iterator it, itEnd(layers.end());
    //for(it = layers.begin(); it != itEnd; ++it){
    //    FrLayerDocObj* layerDO = (FrLayerDocObj*)(*it);

    //    if(layerDO->IsColormap()){
    //         for(int i = 0; i < ITEMS_NUM; i++){
    //             layeredImage[i]->AddLayer(layerDO->GetID(), FrLayeredImage::Colormap);
    //        }
    //    }
    //}
}


///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrUpdateTabsCmd::CanUndo(){
    return false;
}

bool FrUpdateTabsCmd::Undo(){
    return false;
}

bool FrUpdateTabsCmd::Redo(){
    return false;
}
