#include "FrAddLayerCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrRemoveLayerCmd.h"
#include "FrTabSettingsDocObj.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"


FrRemoveLayerCmd::FrRemoveLayerCmd(){
}

bool FrRemoveLayerCmd::Execute(){
	// TODO: not finished
	// Check for safety
    if(!this->GetMainController()) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
	//LayerSettings layerParams = dlg.GetLayerParams();
	// getactivelayer();

	// we should update layer settings in all tabs
//	sets->RemoveLayer(layerParams);	// test
//	doc->SyncronyzeLayers(layerParams, 2);

	mv->GetSliceView()->RemoveLayer();				// test
	////mv->GetMosaicView()->AddLayer(layerParams);
	////mv->GetOrthoView()->AddLayer(layerParams);
		
	mv->GetCurrentView()->UpdatePipeline(FRP_LAYER);	// FRP_LAYER?

	return true;
}


///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrRemoveLayerCmd::CanUndo(){
    return false;
}

bool FrRemoveLayerCmd::Undo(){
    return false;
}

bool FrRemoveLayerCmd::Redo(){
    return false;
}