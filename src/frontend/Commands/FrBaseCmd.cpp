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
    //FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

    //// Set id for update
    //int sliceID = tabSets->GetSliceViewSettings()->ActiveLayerID;
    //int mosaicID = tabSets->GetMosaicViewSettings()->ActiveLayerID;
    //int orthoID = tabSets->GetOrthoViewSettings()->ActiveLayerID;

    //tabSets->GetSliceViewSettings()->ActiveLayerID  = id;
    //tabSets->GetMosaicViewSettings()->ActiveLayerID = id;
    //tabSets->GetOrthoViewSettings()->ActiveLayerID  = id;
    
    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }
    
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
