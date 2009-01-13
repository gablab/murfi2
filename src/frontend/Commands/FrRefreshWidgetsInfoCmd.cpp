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
    FrImageSettingsWidget* widget2 = mv->GetImageSettingsWidget();
    FrGraphPaneWidget* widget3 = mv->GetGraphPaneWidget();


    if (m_Target == All){
        widget->Update();
        widget2->Update();
        widget3->Update();
    }
    else{
        switch(m_Target){
            case GraphPane:
                widget3->Update();
                break;
            case ImageSettings:
                widget2->Update();
                break;
            case LayerList:
                widget->Update();
                break;
        }
    }

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
