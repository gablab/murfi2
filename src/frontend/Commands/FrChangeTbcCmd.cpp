#include "FrChangeTbcCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"
#include "FrTabSettingsDocObj.h"
#include "FrUtils.h"
#include "FrViewDocObj.h"
#include "FrLayerDocObj.h"


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

    unsigned long layerID = BAD_LAYER_ID;
    std::vector<FrLayerSettings*> layers;

    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    switch(viewDO->GetActiveView()){
    case SliceView:
    {
      layerID = viewDO->GetSliceViewSettings()->GetActiveLayerID();
      ChangeTbcByLayerID(layers, layerID);
      break;
    }
    case MosaicView:
    {
      layerID = viewDO->GetSliceViewSettings()->GetActiveLayerID();
      ChangeTbcByLayerID(layers, layerID);
      break;
    }
    case OrthoView:
      // change for all 3 views
      layerID = viewDO->GetSliceViewSettings()->GetActiveLayerID();
      for(int viewID=0; viewID < ORTHO_VIEWS_CNT; ++viewID){
	ChangeTbcByLayerID(layers, layerID);
      }
      break;
    default:
        return false;
    }
    m_isThreshold = m_isBrightness = m_isContrast = false;
    FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_TBC);

    return true;
}

void FrChangeTbcCmd::ChangeTbcByLayerID(std::vector<FrLayerSettings*>& layers, unsigned long ID){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // find layer by ID and change its settings
    if(ID == ALL_LAYER_ID){
        // change settings for all layers
        // get all layer settings
        FrLayerDocObj* layerDO = 0L;
        FrDocument::DocObjCollection layers;
        doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    
 
        if(layers.size() > 0){
            for (int i = 0; i < layers.size(); i++){
                layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
                FrImageLayerSettings* imgLS = (FrImageLayerSettings*)layerDO->GetSettings();
                SetupTbcSettings(imgLS->TbcSettings);
            }
        }
    }
    else if(ID != BAD_LAYER_ID){
        // get all layer settings
        FrLayerDocObj* layerDO = 0L;
        FrDocument::DocObjCollection layers;
        doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

        // find laye by ID and change settings
        if(layers.size() > 0){

            for (int i = 0; i < layers.size(); i++){

                layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
                if (layerDO->GetID() == ID){

                    FrImageLayerSettings* imgLS = (FrImageLayerSettings*)layerDO->GetSettings();
                    SetupTbcSettings(imgLS->TbcSettings);
                }
            }
        }

    }
}

void FrChangeTbcCmd::SetupTbcSettings(FrTbcSettings& settings){ 

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
