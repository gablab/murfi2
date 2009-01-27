#include "FrGraphTabDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"
#include "FrPointsDocObj.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrGraphTabDocObj::FrGraphTabDocObj(bool isDefault){
    m_ID = (unsigned int)((void*)this);
    m_Name = DEF_TBS_NAME;
    m_Description = DEF_TBS_DESCR;
    m_IsCurrent = DEF_TBS_CURRENT;
    m_IsDefault = isDefault;

    m_Points = new FrPointsDocObj();
}

FrGraphTabDocObj::~FrGraphTabDocObj(){
    if (m_Points) delete m_Points;
    this->ClearGraphs();
}

void FrGraphTabDocObj::OnAdd(FrDocument* doc){
    // add tab here
    FrUpdateGraphTabsCmd* cmd1 = FrCommandController::CreateCmd<FrUpdateGraphTabsCmd>();
    cmd1->SetAction(FrUpdateGraphTabsCmd::Add);
    cmd1->SetGraphTabDocObj(this);

    FrRefreshWidgetsInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd2->SetTarget(FrRefreshWidgetsInfoCmd::GraphPane);
    
    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    FrCommandController::Execute(cmd);
    delete cmd;
}

void FrGraphTabDocObj::OnRemove(FrDocument* doc){
    // We need to remove tab here
    FrUpdateGraphTabsCmd* cmd = FrCommandController::CreateCmd<FrUpdateGraphTabsCmd>();
    cmd->SetAction(FrUpdateGraphTabsCmd::Remove);
    cmd->SetGraphTabDocObj(this);
    FrCommandController::Execute(cmd);
    delete cmd;
}

FrDocumentObj::ObjTypes FrGraphTabDocObj::GetType(){
    return FrDocumentObj::GraphTab;
}

void FrGraphTabDocObj::AddGraph(FrGraphDocObj* graph){
    m_Graphs.push_back(graph);
}

void FrGraphTabDocObj::ClearGraphs(){
    std::vector<FrGraphDocObj*>::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        delete (*it);
    }
    m_Graphs.clear();
}

void FrGraphTabDocObj::SetPoints(FrPointsDocObj * doc){
    //if (m_Points) delete m_Points;
    //m_Points = new FrPointsDocObj(doc);
    if (doc){
        m_Points->CopySettings(doc);
    }
}
