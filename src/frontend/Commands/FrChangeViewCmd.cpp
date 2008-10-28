#include "FrChangeViewCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrBaseView.h"
#include "FrActionManager.h"


// Qt stuff
#include "Qt/QAction.h"

FrChangeViewCmd::FrChangeViewCmd()
: m_TargetView(FrChangeViewCmd::Unknown){

}

bool FrChangeViewCmd::Execute(){
    // check params
    if(this->GetMainController() == 0 ||
        m_TargetView == FrChangeViewCmd::Unknown) return false;
    
    bool result = false;
    FrTabSettingsDocObj::View viewType = 
        (FrTabSettingsDocObj::View)GetActiveViewType();

    if(viewType != FrTabSettingsDocObj::Undefined){
        
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

        // Change active view
        tabSets->SetActiveView(viewType);
        
        mv->GetCurrentView()->RemoveRenderers();
        mv->SetCurrentView( GetTargetView(viewType) );
        mv->GetCurrentView()->SetupRenderers();
        FrBaseCmd::UpdatePipelineForID(ALL_LAYERS_ID, FRP_FULL);

        // Update UI
        UpdateViewUI();
        result = true;
    }
    return result;
}

int FrChangeViewCmd::GetActiveViewType(){
    // preset
    FrTabSettingsDocObj::View viewType = FrTabSettingsDocObj::Undefined;
        
    switch(m_TargetView){
        case FrChangeViewCmd::Slice:  
            viewType = FrTabSettingsDocObj::SliceView;
            break;
        case FrChangeViewCmd::Mosaic: 
            viewType = FrTabSettingsDocObj::MosaicView;
            break;
        case FrChangeViewCmd::Ortho:  
            viewType = FrTabSettingsDocObj::OrthoView;
            break;
        case FrChangeViewCmd::Synchronize:
            {
                // Get active view type from document
                FrMainDocument* doc = this->GetMainController()->GetMainDocument();
                FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
                viewType = tabSets->GetActiveView();
                break;
            }
        default:
            break;
    }

    return viewType;
}

FrBaseView* FrChangeViewCmd::GetTargetView(int view){
    // Presets
    FrTabSettingsDocObj::View viewType = (FrTabSettingsDocObj::View)view;
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrBaseView* targetView = 0L;

    // Determine wich view to return
    switch(viewType){
        case FrTabSettingsDocObj::SliceView: 
            targetView = (FrBaseView*)mv->GetSliceView();
            break;
        case FrTabSettingsDocObj::MosaicView: 
            targetView = (FrBaseView*)mv->GetMosaicView();
            break;
        case FrTabSettingsDocObj::OrthoView:
            targetView = (FrBaseView*)mv->GetOrthoView();
            break;
        default:
            break;
    }
    return targetView;
}

void FrChangeViewCmd::UpdateViewUI(){
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrActionManager* am = mv->GetActionManager();

    QAction* actions[3];
    actions[0] = am->GetViewSliceAction();
    actions[1] = am->GetViewMosaicAction();
    actions[2] = am->GetViewOrthoAction();
    
    QAction* action = 0L;
    FrTabSettingsDocObj::View viewType = (FrTabSettingsDocObj::View)GetActiveViewType();
    switch(viewType){
        case FrTabSettingsDocObj::SliceView: action = actions[0];
            break;
        case FrTabSettingsDocObj::MosaicView: action = actions[1];
            break;
        case FrTabSettingsDocObj::OrthoView:  action = actions[2];
            break;
        default:
            break;
    }

    // Do update UI
    if(action != 0L){
        for(int i=0; i < 3; ++i){
            actions[i]->blockSignals(true);
            actions[i]->setChecked( (actions[i] == action) );
            actions[i]->blockSignals(false);
        }
    }
}
///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrChangeViewCmd::CanUndo(){
    return false;
}

bool FrChangeViewCmd::Undo(){
    return false;
}

bool FrChangeViewCmd::Redo(){
    return false;
}