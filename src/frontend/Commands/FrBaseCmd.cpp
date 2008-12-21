#include "FrBaseCmd.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrNotify.h"
#include "FrViewDocObj.h"


void FrBaseCmd::UpdatePipelineForID(int id, int point){

    if(id == BAD_LAYER_ID) return;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    
    // Set id for update
    int sliceID = viewDO->GetSliceViewSettings()->ActiveLayerID;
    int mosaicID = viewDO->GetMosaicViewSettings()->ActiveLayerID;
    int orthoID = viewDO->GetOrthoViewSettings()->ActiveLayerID;

    viewDO->GetSliceViewSettings()->ActiveLayerID  = id;
    viewDO->GetMosaicViewSettings()->ActiveLayerID = id;
    viewDO->GetOrthoViewSettings()->ActiveLayerID  = id;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    mv->GetCurrentView()->UpdatePipeline(point);

    // Restore id
    viewDO->GetSliceViewSettings()->ActiveLayerID  = sliceID;
    viewDO->GetMosaicViewSettings()->ActiveLayerID = mosaicID;
    viewDO->GetOrthoViewSettings()->ActiveLayerID  = orthoID;
}
