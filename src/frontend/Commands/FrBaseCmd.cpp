#include "FrBaseCmd.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrNotify.h"
#include "FrViewDocObj.h"

#include "FrMarshalling.h"

void FrBaseCmd::UpdatePipelineForID(unsigned long id, int point){

    if(id == BAD_LAYER_ID) return;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    // Set id for update
    unsigned long sliceID = viewDO->GetSliceViewSettings()->GetActiveLayerID();
    unsigned long mosaicID = viewDO->GetMosaicViewSettings()->GetActiveLayerID();
    unsigned long orthoID = viewDO->GetOrthoViewSettings()->GetActiveLayerID();

    viewDO->GetSliceViewSettings()->SetActiveLayerID(id);
    viewDO->GetMosaicViewSettings()->SetActiveLayerID(id);
    viewDO->GetOrthoViewSettings()->SetActiveLayerID(id);

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    mv->GetCurrentView()->UpdatePipeline(point);

    // Restore id
    viewDO->GetSliceViewSettings()->SetActiveLayerID(sliceID);
    viewDO->GetMosaicViewSettings()->SetActiveLayerID(mosaicID);
    viewDO->GetOrthoViewSettings()->SetActiveLayerID(orthoID);
}
