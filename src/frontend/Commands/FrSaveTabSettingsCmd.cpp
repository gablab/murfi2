#include "FrSaveTabSettingsCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrTabInfoDialog.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrLayerDocObj.h"
#include "FrViewDocObj.h"

#include "Qt/qstring.h"


FrSaveTabSettingsCmd::FrSaveTabSettingsCmd()
: m_IsDefault(false), m_IsCurrent(false), 
  m_Action(Undefined) {
}

bool FrSaveTabSettingsCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
        case SaveNew:
            result = this->SaveNewTab();
            break;
        case SaveCurrent:
            result = this->SaveCurrentTab();
            break;
        // Not supported
        case Undefined:
        default: ;
    }

    return result;
}

bool FrSaveTabSettingsCmd::SaveNewTab(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    FrTabInfoDialog dlg(mv, true);
    QString caption = "Save view to tab";	
    dlg.SetCaption(caption);

    bool result = false;
    if(!m_IsDefault && dlg.SimpleExec()){
        // Create doc object & set params
        FrTabSettingsDocObj* docObj = new FrTabSettingsDocObj();
        docObj->SetIsCurrent(false);
        docObj->SetName(dlg.GetName());
        docObj->SetDescription(dlg.GetDescription());
        // Use new instead
        this->InitDocObjFromActive(docObj);

        // add to main document
        result = doc->Add(docObj);
    }
    else if(m_IsDefault){
        // Create default doc object and set params
        // NOTE: Assume we never create two default tabs
        FrTabSettingsDocObj* docObj = new FrTabSettingsDocObj(true);
        docObj->SetIsCurrent(true);
        docObj->SetName(QString("Default"));
        docObj->SetDescription(QString("Default tab"));

        // add to main document
        result = doc->Add(docObj);
    }
    return result;
}

bool FrSaveTabSettingsCmd::SaveCurrentTab(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabDO = doc->GetCurrentTabSettings();

    this->InitDocObjFromActive(tabDO);
    return true;
}

void FrSaveTabSettingsCmd::InitDocObjFromActive(FrTabSettingsDocObj* docObj){
    FrDocument::DocObjCollection views;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    if(viewDO != 0){
        viewDO->SaveSettingsTo(docObj);
    }    

    // And also we need info about layers from Document
    docObj->ClearLayersInfo();
    
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);

    FrDocument::DocObjCollection::iterator it, itEnd(layers.end());
    for(it = layers.begin(); it != itEnd; ++it){
        FrLayerDocObj* layer = (FrLayerDocObj*)(*it);
        if(layer->IsImage()){
            // Update image layer 
            // NOTE: (assume there may be only one)
            FrLayerSettings::CopySettings(
                layer->GetSettings(), 
                docObj->GetImageLayer());
        }
        else {
            FrLayerSettings* dst = FrLayerSettings::Create(layer->GetSettings()->GetType());
            FrLayerSettings::CopySettings(layer->GetSettings(), dst);
            docObj->GetLayers().push_back(dst);
        }
    }
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
