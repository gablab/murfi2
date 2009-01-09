#include "FrRefreshWidgetsInfoCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayerListWidget.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"
#include "FrViewDocObj.h"
#include "FrLayerDocObj.h"
#include "FrImageSettingsWidget.h"
#include "FrGraphPaneWidget.h"


FrRefreshWidgetsInfoCmd::FrRefreshWidgetsInfoCmd(){
}


bool FrRefreshWidgetsInfoCmd::Execute(){
    // For safty
    if(!this->GetMainController()) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    if(mv == 0) return false;
        
    FrLayerListWidget* widget = mv->GetLayerListWidget();
    widget->Update();

    FrImageSettingsWidget* widget2 = mv->GetImageSettingsWidget();
    widget2->Update();

    FrGraphPaneWidget* widget3 = mv->GetGraphPaneWidget();
    widget3->Update();

    return true;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrRefreshWidgetsInfoCmd::CanUndo(){
    return false;
}

bool FrRefreshWidgetsInfoCmd::Undo(){
    return false;
}

bool FrRefreshWidgetsInfoCmd::Redo(){
    return false;
}
