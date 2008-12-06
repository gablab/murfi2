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

    int layerID = BAD_LAYER_ID;
    std::vector<FrLayerSettings*> layers;

    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }
        
    Views view = viewDO->GetActiveView();

    switch(view){
    case Views::SliceView:
    {
        layerID = viewDO->GetSliceViewSettings()->ActiveLayerID;
        //GetLayerSettings(sets->GetSliceViewSettings(), layers);
        ChangeTbcByLayerID(layers, layerID);
        break;
    }
    case Views::MosaicView:
    {
        layerID = viewDO->GetSliceViewSettings()->ActiveLayerID;
        //GetLayerSettings(sets->GetMosaicViewSettings(), layers);
        ChangeTbcByLayerID(layers, layerID);
        break;
    }
    case Views::OrthoView:
        // change for all 3 views
        layerID = viewDO->GetSliceViewSettings()->ActiveLayerID;
        for(int viewID=0; viewID < ORTHO_VIEWS_CNT; ++viewID){
            //GetLayerSettings(sets->GetOrthoViewSettings(), layers, viewID);
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

void FrChangeTbcCmd::ChangeTbcByLayerID(std::vector<FrLayerSettings*>& layers, int ID){
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

        //std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
        //for(it = layers.begin(); it != itEnd; ++it){
        //    SetupTbcSettings((*it)->TbcSettings);
        //}
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

        //std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
        //for(it = layers.begin(); it != itEnd; ++it){
        //    if((*it)->ID == ID){
        //        SetupTbcSettings((*it)->TbcSettings);
        //        break;
        //    }
        //}
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
