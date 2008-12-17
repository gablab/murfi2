#include "FrUserActionCmd.h"
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
: m_Action(FrUserActionCmd::Undefined), m_isID(false){
}

bool FrUserActionCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
        case FrUserActionCmd::Add: result = AddLayer();
            break;
        case FrUserActionCmd::Delete: result = DeleteLayer();
            break;
        case FrUserActionCmd::Undefined:
        default:
            // Do nothing here
            break;
    }
    return result;
}

bool FrUserActionCmd::AddLayer(){
    bool result = false;
    
    // create new layer doc object and get settings from dialog
    FrLayerDocObj* layerDO = new FrLayerDocObj(FrLayerSettings::LColormap);

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayerDialog dlg(mv, true);
    if(!dlg.SimpleExec()) return false;
    FrColormapLayerSettings* cmlSets = new FrColormapLayerSettings();
    dlg.GetLayerParams(*cmlSets);

    layerDO->SetSettings(cmlSets);

    // finally add new doc to MainDocument
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    doc->Add(layerDO);
    result = true;

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

    // find appropriate layer doc obj and remove it from main doc
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    if(layers.size() > 0){
        for (int i = 0; i < layers.size(); i++){
            FrLayerDocObj* layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID){
                doc->Remove(layerDO);
                break;
            }
        }
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

//bool FrUserActionCmd::IsRoiLayer(int id){
//    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
//    
//    std::vector<FrDocumentObj*> objects;
//    doc->GetObjectsByType(objects, FrDocumentObj::RoiObject);
//
//    // Iterate through all found rois
//    std::vector<FrDocumentObj*>::iterator itr, itrEnd(objects.end());
//    for(itr = objects.begin(); itr != itrEnd; ++itr){
//        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*itr);
//        if(id == roiDO->GetID()) return true;
//    }
//    return false;
//}
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
