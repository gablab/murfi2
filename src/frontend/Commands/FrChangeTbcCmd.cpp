#include "FrChangeTbcCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrTabSettingsDocObj.h"
#include "FrUtils.h"


FrChangeTbcCmd::FrChangeTbcCmd()
: m_isThreshold(false), m_isBrightness(false), 
  m_isContrast(false) {
    
}

void FrChangeTbcCmd::SetThresholdDelta(double value){
    m_Threshold = value;
    m_isThreshold = true;
}

void FrChangeTbcCmd::SetBrightnessDelta(double value){
    m_Brightness = value;
    m_isBrightness = true;
}

void FrChangeTbcCmd::SetContrastDelta(double value){
    m_Contrast = value;
    m_isContrast = true;
}

bool FrChangeTbcCmd::Execute(){
    if(!this->GetMainController()) return false;
    
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView(); 
    
    int layerID = BAD_LAYER_ID;
    std::vector<FrLayerSettings*> layers;
    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
    
    switch(sets->GetActiveView()){
    case FrTabSettingsDocObj::SliceView:
    {
        layerID = sets->GetSliceViewSettings()->ActiveLayerID;
        GetLayerSettings(sets->GetSliceViewSettings(), layers);
        ChangeTbcByLayerID(layers, layerID);
        break;
    }
    case FrTabSettingsDocObj::MosaicView:
    {
        layerID = sets->GetSliceViewSettings()->ActiveLayerID;
        GetLayerSettings(sets->GetMosaicViewSettings(), layers);
        ChangeTbcByLayerID(layers, layerID);
        break;
    }
    case FrTabSettingsDocObj::OrthoView:
        // change for all 3 views
        layerID = sets->GetSliceViewSettings()->ActiveLayerID;
        for(int viewID=0; viewID < ORTHO_VIEW_NUM; ++viewID){
            GetLayerSettings(sets->GetOrthoViewSettings(), layers, viewID);
            ChangeTbcByLayerID(layers, layerID);
        }
        break;
    default:
        return false;
    }
    m_isThreshold = m_isBrightness = m_isContrast = false;
    FrBaseCmd::UpdatePipelineForID(ALL_LAYERS_ID, FRP_TBC);
    
    return true;
}

void FrChangeTbcCmd::ChangeTbcByLayerID(std::vector<FrLayerSettings*>& layers, int ID){
    // find layer by ID and change its settings
    if(ID == ALL_LAYERS_ID){
        // change settings for all layers
        std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
        for(it = layers.begin(); it != itEnd; ++it){
            SetupTbcSettings((*it)->TbcSettings);
        }
    }
    else if(ID != BAD_LAYER_ID){
        // find laye by ID and change settings
        std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
        for(it = layers.begin(); it != itEnd; ++it){
            if((*it)->ID == ID){
                SetupTbcSettings((*it)->TbcSettings);
                break;
            }
        }
    }
}

void FrChangeTbcCmd::SetupTbcSettings(FrTBCSettings& settings){ 

    if(m_isThreshold)  {
        settings.Threshold  += m_Threshold;
    }
    if(m_isBrightness) {
        settings.Brightness += m_Brightness;
    }
    if(m_isContrast) {
        settings.Contrast += m_Contrast;
    }
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrChangeTbcCmd::CanUndo(){
    return false;
}

bool FrChangeTbcCmd::Undo(){
    return false;
}

bool FrChangeTbcCmd::Redo(){
    return false;
}