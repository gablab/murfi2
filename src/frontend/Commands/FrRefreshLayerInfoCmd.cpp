#include "FrRefreshLayerInfoCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayerListWidget.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"
#include "FrViewDocObj.h"
#include "FrLayerDocObj.h"

FrRefreshLayerInfoCmd::FrRefreshLayerInfoCmd(){
}


bool FrRefreshLayerInfoCmd::Execute(){
    // For safty
    if(!this->GetMainController()) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    if(mv == 0) return false;
        
    FrLayerListWidget* widget = mv->GetLayerListWidget();
    widget->Update();

    return true;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrRefreshLayerInfoCmd::CanUndo(){
    return false;
}

bool FrRefreshLayerInfoCmd::Undo(){
    return false;
}

bool FrRefreshLayerInfoCmd::Redo(){
    return false;
}
