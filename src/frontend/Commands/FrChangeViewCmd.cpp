#include "FrChangeViewCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrBaseView.h"
#include "FrActionManager.h"
#include "FrViewDocObj.h"

// Qt stuff
#include "Qt/qaction.h"

FrChangeViewCmd::FrChangeViewCmd()
: m_TargetView(FrChangeViewCmd::Unknown){
}

bool FrChangeViewCmd::Execute(){
    // check params
    if(this->GetMainController() == 0 ||
        m_TargetView == FrChangeViewCmd::Unknown) return false;
    
    bool result = false;
    Views viewType = (Views)GetActiveViewType();

    if(viewType != Views::Undefined){
        
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        //FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
        
        FrViewDocObj* viewDO = 0L;
        FrDocument::DocObjCollection views;
        doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
        if(views.size() > 0){
            viewDO = (FrViewDocObj*)views[0];
        }

        // Change active view
        viewDO->SetActiveView(viewType);
        
        mv->GetCurrentView()->RemoveRenderers();
        mv->SetCurrentView( GetTargetView(viewType) );
        mv->GetCurrentView()->SetupRenderers();
        FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_FULL);

        // Update UI
        UpdateViewUI();
        result = true;
    }
    return result;
}

int FrChangeViewCmd::GetActiveViewType(){
    // preset
    Views viewType = Views::Undefined;
        
    switch(m_TargetView){
        case FrChangeViewCmd::Slice:  
            viewType = Views::SliceView;
            break;
        case FrChangeViewCmd::Mosaic: 
            viewType = Views::MosaicView;
            break;
        case FrChangeViewCmd::Ortho:  
            viewType = Views::OrthoView;
            break;
        case FrChangeViewCmd::Synchronize:
            {
                // Get active view type from document
                FrMainDocument* doc = this->GetMainController()->GetMainDocument();

                FrViewDocObj* viewDO = 0L;
                FrDocument::DocObjCollection views;
                doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
                if(views.size() > 0){
                    viewDO = (FrViewDocObj*)views[0];
                }
                viewType = viewDO->GetActiveView();
                break;
            }
        default:
            break;
    }

    return viewType;
}

FrBaseView* FrChangeViewCmd::GetTargetView(int view){
    // Presets
    Views viewType = (Views)view;
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrBaseView* targetView = 0L;

    // Determine wich view to return
    switch(viewType){
        case Views::SliceView: 
            targetView = (FrBaseView*)mv->GetSliceView();
            break;
        case Views::MosaicView: 
            targetView = (FrBaseView*)mv->GetMosaicView();
            break;
        case Views::OrthoView:
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
    Views viewType = (Views)GetActiveViewType();
    switch(viewType){
        case Views::SliceView: action = actions[0];
            break;
        case Views::MosaicView: action = actions[1];
            break;
        case Views::OrthoView:  action = actions[2];
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
