#include "FrChangeViewCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrBaseView.h"

FrChangeViewCmd::FrChangeViewCmd()
: m_TargetView(FrChangeViewCmd::Unknown){

}

bool FrChangeViewCmd::Execute(){
    // check params
    if(this->GetMainController() == 0 ||
        m_TargetView == FrChangeViewCmd::Unknown) return false;
    
    bool result = false;
    FrBaseView* targetView = GetTargetView();
    if(targetView != 0){
        // Update current tab settings
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

        ActiveView::View viewType = ActiveView::Slice;
        switch(m_TargetView){
        case FrChangeViewCmd::Slice:  viewType = ActiveView::Slice;
            break;
        case FrChangeViewCmd::Mosaic: viewType = ActiveView::Mosaic;
            break;
        case FrChangeViewCmd::Ortho:  viewType = ActiveView::Ortho;
            break;
        default:
            break;
        }
        tabSets->SetActiveView(viewType);

        // Change active view
        mv->GetCurrentView()->RemoveRenderers();
        mv->SetCurrentView(targetView);
        mv->GetCurrentView()->SetupRenderers();
        mv->GetCurrentView()->UpdatePipeline(FRP_FULL);
        result = true;
    }
    return result;
}

FrBaseView* FrChangeViewCmd::GetTargetView(){
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrBaseView* targetView = 0L;
    switch(m_TargetView){
        case FrChangeViewCmd::Slice:
            targetView = (FrBaseView*)mv->GetSliceView();
            break;
        case FrChangeViewCmd::Mosaic:
            targetView = (FrBaseView*)mv->GetMosaicView();
            break;
        case FrChangeViewCmd::Ortho:
            targetView = (FrBaseView*)mv->GetOrthoView();
            break;
        default:
            break;
    }
    return targetView;
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