#include "FrDeleteGraphTabCmd.h"
#include "FrGraphTabDocObj.h"
#include "FrDocumentObj.h"
#include "FrMainDocument.h"


#define BAD_TAB_ID -1

FrDeleteGraphTabCmd::FrDeleteGraphTabCmd()
: m_TabID(BAD_TAB_ID){
}

bool FrDeleteGraphTabCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    if(m_TabID != BAD_TAB_ID){
        // Get tab setting
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        std::vector<FrDocumentObj*> objects;
        doc->GetObjectsByType(objects, FrDocumentObj::GraphTab);

        // iterate throuh tab settings looking for appropriate tab
        std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
        for(it = objects.begin(); it != itEnd; ++it){
            FrGraphTabDocObj* ts = (FrGraphTabDocObj*)(*it);

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
bool FrDeleteGraphTabCmd::CanUndo(){
    return false;
}

bool FrDeleteGraphTabCmd::Undo(){
    return false;
}

bool FrDeleteGraphTabCmd::Redo(){
    return false;
}
