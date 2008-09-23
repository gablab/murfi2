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
    ActiveView::View viewType = (ActiveView::View)GetActiveViewType();
    if(viewType != ActiveView::Unknown){
        
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();

        // Change active view
        tabSets->SetActiveView(viewType);
        
        mv->GetCurrentView()->RemoveRenderers();
        mv->SetCurrentView( GetTargetView(viewType) );
        mv->GetCurrentView()->SetupRenderers();
        mv->GetCurrentView()->UpdatePipeline(FRP_FULL);
        result = true;
    }
    return result;
}

int FrChangeViewCmd::GetActiveViewType(){
    
    ActiveView::View viewType = ActiveView::Unknown;
        
    switch(m_TargetView){
        case FrChangeViewCmd::Slice:  
            viewType = ActiveView::Slice;
            break;
        case FrChangeViewCmd::Mosaic: 
            viewType = ActiveView::Mosaic;
            break;
        case FrChangeViewCmd::Ortho:  
            viewType = ActiveView::Ortho;
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
    ActiveView::View viewType = (ActiveView::View)view;
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrBaseView* targetView = 0L;

    // Determine wich view to return
    switch(viewType){
        case ActiveView::Slice:
            targetView = (FrBaseView*)mv->GetSliceView();
            break;
        case ActiveView::Mosaic:
            targetView = (FrBaseView*)mv->GetMosaicView();
            break;
        case ActiveView::Ortho:
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