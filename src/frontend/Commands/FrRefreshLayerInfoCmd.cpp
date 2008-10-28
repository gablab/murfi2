#include "FrRefreshLayerInfoCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayerListWidget.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrUtils.h"


FrRefreshLayerInfoCmd::FrRefreshLayerInfoCmd(){
}


bool FrRefreshLayerInfoCmd::Execute(){
    // For safty
    if(!this->GetMainController()) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    int layerID = BAD_LAYER_ID;
    LayerCollection layers;
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    switch(tabSets->GetActiveView()){
        case FrTabSettingsDocObj::SliceView:
            GetLayerSettings(tabSets->GetSliceViewSettings(), layers);
            layerID = tabSets->GetSliceViewSettings()->ActiveLayerID;
            break;
        case FrTabSettingsDocObj::MosaicView:
            GetLayerSettings(tabSets->GetMosaicViewSettings(), layers);
            layerID = tabSets->GetMosaicViewSettings()->ActiveLayerID;
            break;
        case FrTabSettingsDocObj::OrthoView:
            // All views have to be synchronized so just take one
            GetLayerSettings(tabSets->GetOrthoViewSettings(), layers, CORONAL_SLICE);
            layerID = tabSets->GetOrthoViewSettings()->ActiveLayerID;
            break;
    }

    bool result = false;
    if(layers.size() > 0){
        FrLayerListWidget* widget = mv->GetLayerListWidget();
        widget->BlockSignals(true);

        widget->RemoveLayers();
        
        LayerCollection::iterator it, itEnd(layers.end());
        for(it = layers.begin(); it != itEnd; ++it){
            widget->AddLayer( (*it) );
        }

        widget->SetSelectedLayer(layerID);
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