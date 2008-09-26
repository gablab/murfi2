#include "FrChangeTbcCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"

#include "FrTabSettingsDocObj.h"

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

    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
    switch(sets->GetActiveView()){
    case ActiveView::Slice:
        SetupTbcSettings(&sets->GetSliceViewSettings().TbcSetting);
        break;
    case ActiveView::Mosaic:
        SetupTbcSettings(&sets->GetMosaicViewSettings().TbcSetting);
        break;
    case ActiveView::Ortho:
        // change for all views
        SetupTbcSettings(&sets->GetOrthoViewSettings().TbcSettings[0]);
        SetupTbcSettings(&sets->GetOrthoViewSettings().TbcSettings[1]);
        SetupTbcSettings(&sets->GetOrthoViewSettings().TbcSettings[2]);
        break;
    default:
        return false;
    }
    mv->GetCurrentView()->UpdatePipeline(FRP_TBC);
    
    return true;
}

void FrChangeTbcCmd::SetupTbcSettings(void* settings){
    // Assume that settings can only be camera settings
    TBCSettings* tbcSettings = (TBCSettings*)settings;
    if(m_isThreshold)  {
        tbcSettings->Threshold  += m_Threshold;
        m_isThreshold = false;
    }
    if(m_isBrightness) {
        tbcSettings->Brightness += m_Brightness;
        m_isBrightness = false;;
    }
    if(m_isContrast) {
        tbcSettings->Contrast   += m_Contrast;
        m_isContrast = false;
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