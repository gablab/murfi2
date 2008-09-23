#include "FrDeleteTabSettingsCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrDocumentObj.h"
#include "FrMainDocument.h"

#define BAD_TAB_ID -1

FrDeleteTabSettingsCmd::FrDeleteTabSettingsCmd()
: m_TabID(BAD_TAB_ID){
}

bool FrDeleteTabSettingsCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    if(m_TabID != BAD_TAB_ID){
        // Get tab setting
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        std::vector<FrDocumentObj*> objects;
        doc->GetObjectsByType(objects, FrDocumentObj::TabSettings);

        // iterate throuh tab settings looking forappropriate tab
        std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
        for(it = objects.begin(); it != itEnd; ++it){
            FrTabSettingsDocObj* ts = (FrTabSettingsDocObj*)(*it);

            // When tab is found and isn't default remove it
            if( ts->GetID() == m_TabID && !ts->GetIsDefault() ){
                doc->Remove(ts);
                result = true;
                break;
            }
        }
    }
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrDeleteTabSettingsCmd::CanUndo(){
    return false;
}

bool FrDeleteTabSettingsCmd::Undo(){
    return false;
}

bool FrDeleteTabSettingsCmd::Redo(){
    return false;
}