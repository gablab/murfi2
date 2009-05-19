#include "FrTabSettingsDocObj.h"
#include "FrViewSettings.h"
#include "FrDocument.h"
#include "FrCommandController.h"
#include "FrLayerDocObj.h"
#include "RtDataID.h"

// Implementation
FrTabSettingsDocObj::FrTabSettingsDocObj(bool isDefault){
    m_ID = (unsigned long)((void*)this);
    m_Name = DEF_TBS_NAME;
//    m_Description = DEF_TBS_DESCR;
    m_IsCurrent = DEF_TBS_CURRENT;
    m_IsDefault = isDefault;

    m_ActiveView = DEF_TBS_VIEW;
    m_SliceViewSettings = new FrSliceViewSettings();
    m_MosaicViewSettings = new FrMosaicViewSettings();
    m_OrthoViewSettings = new FrOrthoViewSettings();

    //m_ImageLayer = new FrImageLayerSettings(RtDataID(),"empy name :(");
}

FrTabSettingsDocObj::~FrTabSettingsDocObj(){
    delete m_SliceViewSettings;
    delete m_MosaicViewSettings;
    delete m_OrthoViewSettings;

    //delete m_ImageLayer;
    this->ClearLayersInfo();
}

void FrTabSettingsDocObj::OnAdd(FrDocument* doc){    
    // We need to add tab here
    FrUpdateTabsCmd* cmd = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
    cmd->SetAction(FrUpdateTabsCmd::Add);
    cmd->SetTabSettingsDocObj(this);
    FrCommandController::Execute(cmd);
    delete cmd;
}

void FrTabSettingsDocObj::OnRemove(FrDocument* doc){    
    // We need to remove tab here
    FrUpdateTabsCmd* cmd = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
    cmd->SetAction(FrUpdateTabsCmd::Remove);
    cmd->SetTabSettingsDocObj(this);
    FrCommandController::Execute(cmd);
    delete cmd;
    
    // If no more tabs delete layers also
    // (except of ROI layers...)
    //std::vector<FrDocumentObj*> objects;
    //    
    //doc->GetObjectsByType(objects, FrDocumentObj::TabSettings);
    //if(objects.size() <= 0)
    //{
    //    doc->GetObjectsByType(objects, FrDocumentObj::LayerObject);

    //    std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
    //    for(it = objects.begin(); it != itEnd; ++it){
    //        FrLayerDocObj* layerDO = (FrLayerDocObj*) (*it);
    //        if(!layerDO->IsRoi())
    //        {
    //            doc->Remove(layerDO);
    //        }
    //    }
    //}
}

FrDocumentObj::ObjTypes FrTabSettingsDocObj::GetType(){
    return FrDocumentObj::TabSettings;
}

void FrTabSettingsDocObj::InitFrom(FrTabSettingsDocObj* docObj){
    // Do not need copy this
    // Have to be set by user manually
    //m_IsCurrent = docObj->m_IsCurrent;
    //m_IsDefault = docObj->m_IsDefault;
    m_ActiveView = docObj->m_ActiveView;

    // Init slice view settings
    FrSliceViewSettings* srcSlice = docObj->GetSliceViewSettings();
    FrSliceViewSettings* dstSlice = m_SliceViewSettings;
    dstSlice->CamSettings   = srcSlice->CamSettings;
    dstSlice->SetActiveLayerID(srcSlice->GetActiveLayerID());
    dstSlice->SliceNumber   = srcSlice->SliceNumber;
    
    // Init mosaic view settings
    FrMosaicViewSettings* srcMosaic = docObj->GetMosaicViewSettings();
    FrMosaicViewSettings* dstMosaic = m_MosaicViewSettings;
    dstMosaic->CamSettings   = srcMosaic->CamSettings;
    dstMosaic->SetActiveLayerID(srcMosaic->GetActiveLayerID());
        
    // Setup ortho view
    FrOrthoViewSettings* srcOrtho = docObj->GetOrthoViewSettings();
    FrOrthoViewSettings* dstOrtho = m_OrthoViewSettings;
    dstOrtho->SetActiveLayerID(srcOrtho->GetActiveLayerID());
    
    for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
        dstOrtho->SliceNumber[i] = srcOrtho->SliceNumber[i];
        dstOrtho->CamSettings[i] = srcOrtho->CamSettings[i];        
    }

    // Copy image layer
    //(*m_ImageLayer) = (*docObj->m_ImageLayer);

    // Copy other layers
    LayersCollection::iterator it, itEnd(docObj->m_Layers.end());
    for(it = docObj->m_Layers.begin(); it != itEnd; ++it){
        FrLayerSettings* src = (*it);
        FrLayerSettings* dst = FrLayerSettings::Create(src->GetType(),src->DataID, src->Name);
        if(dst){
            FrLayerSettings::CopySettings(src, dst);
            m_Layers.push_back(dst);
        }
    }
}


FrImageLayerSettings* FrTabSettingsDocObj::GetImageLayer(const RtDataID &id) {
  FrImageLayerSettings* result = NULL;
  
  LayersCollection::iterator it, itEnd(m_Layers.end());
  for(it = m_Layers.begin(); it != itEnd; ++it){
    if((*it)->GetType() == FrLayerSettings::LImage && (*it)->DataID == id) {
      result = (FrImageLayerSettings*) (*it);
      break;
    }
  }  

  return result;
}

void FrTabSettingsDocObj::ClearLayersInfo(){
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        delete (*it);
    }
    m_Layers.clear();
}
