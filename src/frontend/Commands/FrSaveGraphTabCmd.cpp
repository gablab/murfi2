#include "FrSaveGraphTabCmd.h"
#include "FrGraphTabDocObj.h"
#include "FrTabInfoDialog.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrLayerDocObj.h"
#include "FrViewDocObj.h"
#include "FrPointsDocObj.h"

#include "Qt/qstring.h"


FrSaveGraphTabCmd::FrSaveGraphTabCmd()
: m_IsDefault(false), m_IsCurrent(false), 
  m_Action(Undefined) {
}

bool FrSaveGraphTabCmd::Execute(){
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

bool FrSaveGraphTabCmd::SaveNewTab(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    FrTabInfoDialog dlg(mv, true);
    QString caption = "Save graph to tab";	
    dlg.SetCaption(caption);

    bool result = false;
    if(!m_IsDefault && dlg.SimpleExec()){
        // Create doc object & set params
        FrGraphTabDocObj* docObj = new FrGraphTabDocObj(false);
        docObj->SetIsCurrent(false);
        docObj->SetName(dlg.GetName());
        //docObj->SetDescription(dlg.GetDescription());
        // Use new instead
        this->InitDocObjFromActive(docObj);

        // add to main document
        result = doc->Add(docObj);
    }
    else if(m_IsDefault){
        // Create default doc object and set params
        // NOTE: Assume we never create two default tabs
        FrGraphTabDocObj* docObj = new FrGraphTabDocObj(true);
        docObj->SetIsCurrent(true);
        docObj->SetName(QString("Default"));
        //docObj->SetDescription(QString("Default tab"));

        // add to main document
        result = doc->Add(docObj);
    }
    return result;
}

bool FrSaveGraphTabCmd::SaveCurrentTab(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrGraphTabDocObj* gTabDO = doc->GetCurrentGraphTab();
    
    if (gTabDO)
        this->InitDocObjFromActive(gTabDO);
    
    return true;
}

void FrSaveGraphTabCmd::InitDocObjFromActive(FrGraphTabDocObj* docObj){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrPointsDocObj* pointsDO = doc->GetCurrentPointsObject();
    if(pointsDO != 0){
        docObj->SetPoints(pointsDO);
    }    

    FrDocument::DocObjCollection graphs;
    doc->GetObjectsByType(graphs, FrDocumentObj::GraphObject);

    // remove old graphs
    docObj->ClearGraphs();

    // add new graphs
    FrDocument::DocObjCollection::iterator it, itEnd(graphs.end());
    for(it = graphs.begin(); it != itEnd; ++it){
        FrGraphDocObj* graphDO = (FrGraphDocObj*)(*it);
        FrGraphDocObj* tmp = new FrGraphDocObj(graphDO);
        docObj->AddGraph(tmp);
    }
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrSaveGraphTabCmd::CanUndo(){
    return false;
}

bool FrSaveGraphTabCmd::Undo(){
    return false;
}

bool FrSaveGraphTabCmd::Redo(){
    return false;
}
