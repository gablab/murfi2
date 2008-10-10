#include "FrAddLayerCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrAddLayerDialog.h"
#include "FrTabSettingsDocObj.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"


FrAddLayerCmd::FrAddLayerCmd(){
}

bool FrAddLayerCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView();

	FrAddLayerDialog dlg(mv, true);
	if(!dlg.SimpleExec())
		return false;
	
    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
	LayerSettings layerParams = dlg.GetLayerParams();
	// we should update layer settings in all tabs
	sets->AddLayer(layerParams);	// test
	doc->SyncronyzeLayers(layerParams);

	mv->GetSliceView()->AddLayer(layerParams);	// check in each view variable to update pipeline? test
	//mv->GetMosaicView()->AddLayer(layerParams);
	//mv->GetOrthoView()->AddLayer(layerParams);
		
	mv->GetCurrentView()->UpdatePipeline(FRP_LAYER);	// FRP_LAYER?

	return true;
}


///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrAddLayerCmd::CanUndo(){
    return false;
}

bool FrAddLayerCmd::Undo(){
    return false;
}

bool FrAddLayerCmd::Redo(){
    return false;
}