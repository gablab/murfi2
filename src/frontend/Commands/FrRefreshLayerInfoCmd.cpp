#include "FrRefreshLayerInfoCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayerListWidget.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"
#include "FrViewDocObj.h"
#include "FrLayerDocObj.h"

FrRefreshLayerInfoCmd::FrRefreshLayerInfoCmd(){
}


bool FrRefreshLayerInfoCmd::Execute(){
    // For safty
    if(!this->GetMainController()) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    int layerID = BAD_LAYER_ID;
 
    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }

    switch(viewDO->GetActiveView()){
        case Views::SliceView:
            //GetLayerSettings(tabSets->GetSliceViewSettings(), layers);
            layerID = viewDO->GetSliceViewSettings()->ActiveLayerID;
            break;
        case Views::MosaicView:
            //GetLayerSettings(tabSets->GetMosaicViewSettings(), layers);
            layerID = viewDO->GetMosaicViewSettings()->ActiveLayerID;
            break;
        case Views::OrthoView:
            // All views have to be synchronized so just take one
            //GetLayerSettings(tabSets->GetOrthoViewSettings(), layers, CORONAL_SLICE);
            layerID = viewDO->GetOrthoViewSettings()->ActiveLayerID;
            break;
    }

    bool result = false;
    
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    if(layers.size() > 0){
        FrLayerListWidget* widget = mv->GetLayerListWidget();
        widget->BlockSignals(true);

        widget->RemoveLayers();
        
        // Add all layers
        std::vector<FrDocumentObj*>::iterator it, itEnd(layers.end());
        for(it = layers.begin(); it != itEnd; ++it){
            FrLayerDocObj* layerDO = (FrLayerDocObj*) (*it);
            widget->AddLayer(layerDO);
        }

        //// add roi too
        //std::vector<FrDocumentObj*> objects;
        //doc->GetObjectsByType(objects, FrDocumentObj::RoiObject);
        //std::vector<FrDocumentObj*>::iterator itr, itrEnd(objects.end());
        //for(itr = objects.begin(); itr != itrEnd; ++itr){
        //    FrRoiDocObj* roiDO = (FrRoiDocObj*) (*itr);
        //    widget->AddLayer(roiDO);
        //}

        widget->SetSelectedLayer(layerID);
        widget->UpdateRoiList();
        widget->BlockSignals(false);
    }
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrRefreshLayerInfoCmd::CanUndo(){
    return false;
}

bool FrRefreshLayerInfoCmd::Undo(){
    return false;
}

bool FrRefreshLayerInfoCmd::Redo(){
    return false;
}
