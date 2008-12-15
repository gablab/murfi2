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

FrUserActionCmd::FrUserActionCmd() 
: m_Action(FrUserActionCmd::Undefined), 
  m_DocObj(0), m_isID(false){
}

void FrUserActionCmd::SetID(int id){
    m_ID = id;
    m_isID = true;
}

bool FrUserActionCmd::Execute(){
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

bool FrUserActionCmd::AddLayer(){

    return result;
}

bool FrUserActionCmd::DeleteLayer(){
    // Get proper ID
    if(!m_isID){
        m_ID = GetActiveLayerID();
    }
    // if wrong ID is specified return
    // Default layer cannot be deleted
    if(m_ID < DEF_LAYER_ID) return false;
    if(m_ID == DEF_LAYER_ID){
        QMessageBox::critical(this->GetMainController()->GetMainView(), 
            "Delete Layer Command", "Can't delete default layer...");
        return false;
    }

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    
    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }

    // Delete layer doc obj
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    
    
    bool isFound = false;

    // delete layer from LayeredImage
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){
        layeredImage[i]->RemoveLayer(m_ID);
    }

    if(layers.size() > 0){
        // delete appropriate LayerDocObj
        for (int i = 0; i < layers.size(); i++){
            FrLayerDocObj* layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID){
                doc->Remove(layerDO);
                //delete layerDO;
                isFound = true;
                break;
            }
        }
    }    

    if(isFound){
        // Assume that there is good sync between layers 
        viewDO->GetSliceViewSettings()->ActiveLayerID = DEF_LAYER_ID;
        viewDO->GetMosaicViewSettings()->ActiveLayerID = DEF_LAYER_ID;
        viewDO->GetOrthoViewSettings()->ActiveLayerID = DEF_LAYER_ID;

        FrBaseCmd::UpdatePipelineForID(DEF_LAYER_ID, FRP_COLORMAP);
    }
    return true;
}

// delete active layer
int FrUserActionCmd::GetActiveLayerID(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }

    int result = BAD_LAYER_ID;
    switch(viewDO->GetActiveView()){
        case Views::SliceView: 
            result = viewDO->GetSliceViewSettings()->ActiveLayerID;
            break;
        case Views::MosaicView: 
            result = viewDO->GetMosaicViewSettings()->ActiveLayerID;
            break;
        case Views::OrthoView: 
            result = viewDO->GetOrthoViewSettings()->ActiveLayerID;
            break;
    }
    return result;
}

bool FrUserActionCmd::IsRoiLayer(int id){
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
bool FrUserActionCmd::CanUndo(){
    return false;
}

bool FrUserActionCmd::Undo(){
    return false;
}

bool FrUserActionCmd::Redo(){
    return false;
}
