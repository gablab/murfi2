#include "FrBaseCmd.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrNotify.h"

void FrBaseCmd::UpdatePipelineForID(int id, int point){
    if(id == BAD_LAYER_ID) return;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

    // Set id for update
    int sliceID = tabSets->GetSliceViewSettings()->ActiveLayerID;
    int mosaicID = tabSets->GetMosaicViewSettings()->ActiveLayerID;
    int orthoID = tabSets->GetOrthoViewSettings()->ActiveLayerID;

    tabSets->GetSliceViewSettings()->ActiveLayerID  = id;
    tabSets->GetMosaicViewSettings()->ActiveLayerID = id;
    tabSets->GetOrthoViewSettings()->ActiveLayerID  = id;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    mv->GetCurrentView()->UpdatePipeline(point);

    // Restore id
    tabSets->GetSliceViewSettings()->ActiveLayerID  = sliceID;
    tabSets->GetMosaicViewSettings()->ActiveLayerID = mosaicID;
    tabSets->GetOrthoViewSettings()->ActiveLayerID  = orthoID;
}