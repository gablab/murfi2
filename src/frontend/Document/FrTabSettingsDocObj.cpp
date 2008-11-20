#include "FrTabSettingsDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"


#define DEF_VIEW FrTabSettingsDocObj::SliceView

// Implementation
FrTabSettingsDocObj::FrTabSettingsDocObj(bool isDefault){
    m_IsCurrent = false;
    m_IsDefault = isDefault;
    m_ActiveView = DEF_VIEW;

    m_SliceViewSettings = new FrSliceViewSettings();
    m_MosaicViewSettings = new FrMosaicViewSettings();
    m_OrthoViewSettings = new FrOrthoViewSettings();

    // Init slice view settings
    InitCameraDefault(&m_SliceViewSettings->CamSettings);
    InitLayerDefault(&m_SliceViewSettings->MainLayer);
    m_SliceViewSettings->MainLayer.ID = DEFAULT_LAYER_ID;
    m_SliceViewSettings->ActiveLayerID = DEFAULT_LAYER_ID;
    m_SliceViewSettings->SliceNumber = DEF_SLICE_NUMBER;
    m_SliceViewSettings->ResetNeeded = true;

    // Init mosaic view settings
    InitCameraDefault(&m_MosaicViewSettings->CamSettings);
    InitLayerDefault(&m_MosaicViewSettings->MainLayer);
    m_MosaicViewSettings->MainLayer.ID = DEFAULT_LAYER_ID;
    m_MosaicViewSettings->ActiveLayerID = DEFAULT_LAYER_ID;
    m_MosaicViewSettings->ResetNeeded = true;
    
    // Setup ortho view
    m_OrthoViewSettings->CoronalSlice = DEF_SLICE_NUMBER;
    m_OrthoViewSettings->SagitalSlice = DEF_SLICE_NUMBER;
    m_OrthoViewSettings->AxialSlice   = DEF_SLICE_NUMBER;
    m_OrthoViewSettings->ActiveLayerID = DEFAULT_LAYER_ID;
    m_OrthoViewSettings->ResetNeeded = true;

    for(int i=0; i < ORTHO_VIEW_NUM; ++i){
        InitCameraDefault(&m_OrthoViewSettings->CamSettings[i]);
        InitLayerDefault(&m_OrthoViewSettings->MainLayer[i]);
        m_OrthoViewSettings->MainLayer[i].ID = DEFAULT_LAYER_ID;
    }
}

FrTabSettingsDocObj::~FrTabSettingsDocObj(){
    delete m_SliceViewSettings;
    delete m_MosaicViewSettings;
    delete m_OrthoViewSettings;
}

void FrTabSettingsDocObj::OnAdd(FrDocument* doc){    
    // We need to add tab here
    FrUpdateTabsCmd* cmd = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
    cmd->SetAction(FrUpdateTabsCmd::Add);
    cmd->SetTabSettingsDocObj(this);
    cmd->Execute();
    delete cmd;
}

void FrTabSettingsDocObj::OnRemove(FrDocument* doc){    
    // We need to remove tab here
    FrUpdateTabsCmd* cmd = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
    cmd->SetAction(FrUpdateTabsCmd::Remove);
    cmd->SetTabSettingsDocObj(this);
    cmd->Execute();
    delete cmd;
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
    dstSlice->CamSettings  = srcSlice->CamSettings;
    dstSlice->MainLayer    = srcSlice->MainLayer;
    dstSlice->ActiveLayerID = srcSlice->ActiveLayerID;
    dstSlice->SliceNumber   = srcSlice->SliceNumber;
    CopyLayersInfo(dstSlice->OtherLayers, srcSlice->OtherLayers);
    dstSlice->ResetNeeded = srcSlice->ResetNeeded;
    
    // Init mosaic view settings
    FrMosaicViewSettings* srcMosaic = docObj->GetMosaicViewSettings();
    FrMosaicViewSettings* dstMosaic = m_MosaicViewSettings;
    dstMosaic->CamSettings  = srcMosaic->CamSettings;
    dstMosaic->MainLayer    = srcMosaic->MainLayer;
    dstMosaic->ActiveLayerID = srcMosaic->ActiveLayerID;
    CopyLayersInfo(dstMosaic->OtherLayers, srcMosaic->OtherLayers);
    dstMosaic->ResetNeeded = srcMosaic->ResetNeeded;
    
    // Setup ortho view
    FrOrthoViewSettings* srcOrtho = docObj->GetOrthoViewSettings();
    FrOrthoViewSettings* dstOrtho = m_OrthoViewSettings;
    dstOrtho->ActiveLayerID = srcOrtho->ActiveLayerID;
    dstOrtho->CoronalSlice = srcOrtho->CoronalSlice;
    dstOrtho->SagitalSlice = srcOrtho->SagitalSlice;
    dstOrtho->AxialSlice   = srcOrtho->AxialSlice;
    dstOrtho->ResetNeeded  = srcOrtho->ResetNeeded;

    for(int i=0; i < ORTHO_VIEW_NUM; ++i){
        dstOrtho->CamSettings[i] = srcOrtho->CamSettings[i];
        dstOrtho->MainLayer[i] = srcOrtho->MainLayer[i];
        CopyLayersInfo(dstOrtho->OtherLayers[i], srcOrtho->OtherLayers[i]);
    }   
}

void FrTabSettingsDocObj::CopyLayersInfo(LayerCollection& dst, LayerCollection& src){
    // Prepare dst collection
    while(dst.size() > 0){
        RemoveItemFromVector(dst, (*dst.begin()), true);
    }

    // Insert copies of items from src collection 
    LayerCollection::iterator it, itEnd(src.end());
    for(it = src.begin(); it != itEnd; ++it){
        FrLayerSettings& srcLayer = *(*it);
        FrLayerSettings* newLayer = new FrLayerSettings(srcLayer);
        dst.push_back(newLayer);
    }
}