#include "FrSaveTabSettingsCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrTabInfoDialog.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"

#include "Qt/QString.h"

FrSaveTabSettingsCmd::FrSaveTabSettingsCmd()
: m_IsDefault(false), m_IsCurrent(false) {
}

bool FrSaveTabSettingsCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    FrTabInfoDialog dlg(mv, true);
    dlg.SetCaption(QString("Save view to tab"));

    bool result = false;
    if(!m_IsDefault && dlg.SimpleExec()){
        // Create doc object & set params
        FrTabSettingsDocObj* docObj = new FrTabSettingsDocObj();
        docObj->SetIsCurrent(false);
        docObj->SetName(dlg.GetName());
        docObj->SetDescription(dlg.GetDescription());
        // Use new instead
        InitDocObjFromActive(docObj);

        // add to main document
        result = doc->Add(docObj);
    }
    else if(m_IsDefault){
        // Create default doc object and set params
        FrTabSettingsDocObj* docObj = new FrTabSettingsDocObj(true);
        docObj->SetIsCurrent(false);
        docObj->SetName(QString("Deafult"));
        docObj->SetDescription(QString("Default tab"));
        
        // add to main document
        result = doc->Add(docObj);
    }
    return result;
}

void FrSaveTabSettingsCmd::InitDocObjFromActive(FrTabSettingsDocObj* docObj){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* source = doc->GetCurrentTabSettings();
    docObj->InitFrom(source);
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrSaveTabSettingsCmd::CanUndo(){
    return false;
}

bool FrSaveTabSettingsCmd::Undo(){
    return false;
}

bool FrSaveTabSettingsCmd::Redo(){
    return false;
}