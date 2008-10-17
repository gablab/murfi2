#include "FrUpdateTabsCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrBookmarkWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrMyLayeredImage.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"

#define BAD_TAB_ID -1
#define ITEMS_NUM  5


FrUpdateTabsCmd::FrUpdateTabsCmd()
: m_Action(FrUpdateTabsCmd::None), m_TabSettingsDocObj(0L), m_TabID(BAD_TAB_ID){

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

    std::vector<FrDocumentObj*> docTabs;
    doc->GetObjectsByType(docTabs, FrDocumentObj::TabSettings);
    
    std::vector<FrDocumentObj*>::iterator it, itEnd(docTabs.end());
    for(it = docTabs.begin(); it != itEnd; ++it){
        FrTabSettingsDocObj* tsDO = (FrTabSettingsDocObj*)(*it);
        tsDO->SetIsCurrent( (tsDO->GetID() == m_TabID) );
    }

    // Setup layers to images
    SetupImageLayers();

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
        case FrUpdateTabsCmd::Add: ;
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


void FrUpdateTabsCmd::SetupImageLayers(){
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    
    FrMyLayeredImage* image[ITEMS_NUM];
    image[0] = mv->GetSliceView()->GetImage();
    image[1] = mv->GetMosaicView()->GetImage();
    image[2] = mv->GetOrthoView()->GetImage(CORONAL_IMAGE);
    image[3] = mv->GetOrthoView()->GetImage(SAGITAL_IMAGE);
    image[4] = mv->GetOrthoView()->GetImage(AXIAL_IMAGE);
    
    LayerCollection* layers[ITEMS_NUM];
    layers[0] = &tabSets->GetSliceViewSettings()->OtherLayers;
    layers[1] = &tabSets->GetMosaicViewSettings()->OtherLayers;
    layers[2] = &tabSets->GetOrthoViewSettings()->OtherLayers[CORONAL_IMAGE];
    layers[3] = &tabSets->GetOrthoViewSettings()->OtherLayers[SAGITAL_IMAGE];
    layers[4] = &tabSets->GetOrthoViewSettings()->OtherLayers[AXIAL_IMAGE];
    
    for(int i=0; i < ITEMS_NUM; ++i){
        image[i]->RemoveLayers();

        LayerCollection::iterator it, itEnd(layers[i]->end());
        for(it = layers[i]->begin(); it != itEnd; ++it){
            int layerID = image[i]->AddLayer();
            (*it)->ID = layerID;
        }
    }
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