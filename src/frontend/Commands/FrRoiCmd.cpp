#include "FrRoiCmd.h"
#include "FrMainWindow.h"
#include "FrLayeredImage.h"
#include "FrRoiDocObj.h"
#include "FrUtils.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"

#include "Qt/qmessagebox.h"


#define ALL_ITEMS_COUNT 5

FrRoiCmd::FrRoiCmd() 
: m_Action(FrRoiCmd::None), m_Roi(0){
}

bool FrRoiCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
        case FrRoiCmd::Add: result = this->AddRoi();
            break;
        case FrRoiCmd::Remove: result = RemoveRoi();
            break;
        default:
            // Do nothing here
            break;
    }
    return result;
}

bool FrRoiCmd::AddRoi(){
    if(m_Roi == 0) return false;
    
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);
    
    // NOTE: Assume that id is the same for all images
    int id = BAD_LAYER_ID; 
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){             // TODO: not finished
        id = layeredImage[i]->AddLayer(m_Roi->GetID(), FrLayeredImage::Roi);
        id = m_Roi->GetID();
    }
    
    FrBaseCmd::UpdatePipelineForID(id, FRP_READ);
    return true;
}

bool FrRoiCmd::RemoveRoi(){
    if(m_Roi == 0) return false;
    //// Get proper ID
    //if(!m_isID || m_ID == CUR_LAYER_ID){
    //    m_ID = GetActiveLayerID();
    //}
    //// if wrong ID is specified return
    //// Default layer cannot be deleted
    //if(m_ID < DEFAULT_LAYER_ID) return false;
    //if(m_ID == DEFAULT_LAYER_ID){
    //    QMessageBox::critical(this->GetMainController()->GetMainView(), 
    //        "Delete Layer Command", "Can't delete default layer...");
    //    return false;
    //}

    //// Init data
    //FrMainWindow* mv = this->GetMainController()->GetMainView();
    //FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    //layeredImage[0] = mv->GetSliceView()->GetImage();
    //layeredImage[1] = mv->GetMosaicView()->GetImage();
    //layeredImage[2] = mv->GetOrthoView()->GetImage(CORONAL_IMAGE);
    //layeredImage[3] = mv->GetOrthoView()->GetImage(SAGITAL_IMAGE);
    //layeredImage[4] = mv->GetOrthoView()->GetImage(AXIAL_IMAGE);

    //FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    //FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    //std::vector<FrLayerSettings*>* otherLayers[ALL_ITEMS_COUNT];
    //otherLayers[0] = &tabSets->GetSliceViewSettings()->OtherLayers;
    //otherLayers[1] = &tabSets->GetMosaicViewSettings()->OtherLayers;
    //otherLayers[2] = &tabSets->GetOrthoViewSettings()->OtherLayers[CORONAL_IMAGE];
    //otherLayers[3] = &tabSets->GetOrthoViewSettings()->OtherLayers[SAGITAL_IMAGE];
    //otherLayers[4] = &tabSets->GetOrthoViewSettings()->OtherLayers[AXIAL_IMAGE];

    //// Delete layer from all views
    //bool isFound = false;
    //for(int i=0; i < ALL_ITEMS_COUNT; ++i){
    //    if(layeredImage[i]->RemoveImageLayer(m_ID)){
    //        // Find layer
    //        LayerCollection::iterator it, itEnd(otherLayers[i]->end());
    //        for(it = otherLayers[i]->begin(); it != itEnd; ++it){
    //            if((*it)->ID == m_ID) break;
    //        }
    //        // remove it
    //        if(it != itEnd){
    //            delete (*it);
    //            otherLayers[i]->erase(it);
    //            isFound = true;
    //        }
    //    }
    //}

    //if(isFound){
    //    // Assume that there is good sync between layers 
    //    tabSets->GetSliceViewSettings()->ActiveLayerID--;
    //    tabSets->GetMosaicViewSettings()->ActiveLayerID--;
    //    tabSets->GetOrthoViewSettings()->ActiveLayerID--;

    //    FrBaseCmd::UpdatePipelineForID(0, FRP_COLORMAP);
    //}
    return false;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrRoiCmd::CanUndo(){
    return false;
}

bool FrRoiCmd::Undo(){
    return false;
}

bool FrRoiCmd::Redo(){
    return false;
}
