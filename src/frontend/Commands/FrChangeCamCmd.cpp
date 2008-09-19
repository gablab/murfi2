#include "FrChangeCamCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"

#include "FrTabSettingsDocObj.h"

FrChangeCamCmd::FrChangeCamCmd()
: m_isPosition(false), m_isFocalPoint(false), 
  m_isViewUp(false), m_isScale(false) {
    
}

bool FrChangeCamCmd::Execute(){
    if(!this->GetMainController()) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView(); 

    FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
        

    return true;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrChangeCamCmd::CanUndo(){
    return false;
}

bool FrChangeCamCmd::Undo(){
    return false;
}

bool FrChangeCamCmd::Redo(){
    return false;
}