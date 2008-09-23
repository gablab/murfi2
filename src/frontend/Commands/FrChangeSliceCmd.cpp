#include "FrChangeSliceCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"

#include "FrTabSettingsDocObj.h"

FrChangeSliceCmd::FrChangeSliceCmd()
: m_isSlice(false) {
    
}

void FrChangeSliceCmd::SetSliceDelta(double value){
    m_Slice = value;
    m_isSlice = true;
}

bool FrChangeSliceCmd::Execute(){
    if(!this->GetMainController()) return false;
    
    if(m_isSlice){
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        FrMainWindow* mv = this->GetMainController()->GetMainView(); 

        FrTabSettingsDocObj* sets = doc->GetCurrentTabSettings();
        switch(sets->GetActiveView()){
        case ActiveView::Slice:
            {
                sets->GetSliceViewSettings().SliceNumber += m_Slice;
                break;
            }
        case ActiveView::Mosaic:
            {
                // DO we need this?
                sets->GetMosaicViewSettings().SliceNumber += m_Slice;
                break;
            }
        case ActiveView::Ortho:
            {
                // TODO: not implemented
            }
        default:
            return false;
        }
        mv->GetCurrentView()->UpdatePipeline(FRP_SLICE);
    }    
    return true;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrChangeSliceCmd::CanUndo(){
    return false;
}

bool FrChangeSliceCmd::Undo(){
    return false;
}

bool FrChangeSliceCmd::Redo(){
    return false;
}