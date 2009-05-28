#include "FrGraphPaneDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"
#include "FrPointsDocObj.h"

#include "RtDataID.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrGraphPaneDocObj::FrGraphPaneDocObj(){
    m_ID = (unsigned long)((void*)this);

    m_Graphs.clear();
}

FrGraphPaneDocObj::~FrGraphPaneDocObj(){
    this->ClearGraphs();
}

void FrGraphPaneDocObj::OnAdd(FrDocument* doc){
    // add graph widget here
    FrGraphCmd* cmd1 = FrCommandController::CreateCmd<FrGraphCmd>();
    cmd1->SetAction(FrGraphCmd::AddGraphWidget);
    cmd1->SetDocObj(this);

    FrRefreshWidgetsInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd2->SetTarget(FrRefreshWidgetsInfoCmd::GraphPane);
    
    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    FrCommandController::Execute(cmd);
    delete cmd;
}

void FrGraphPaneDocObj::OnRemove(FrDocument* doc){
    // remove graph widget here
    FrGraphCmd* cmd1 = FrCommandController::CreateCmd<FrGraphCmd>();
    cmd1->SetAction(FrGraphCmd::DeleteGraphWidget);
    cmd1->SetDocObj(this);

    FrRefreshWidgetsInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd2->SetTarget(FrRefreshWidgetsInfoCmd::GraphPane);
    
    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    FrCommandController::Execute(cmd);
    delete cmd;
}

FrDocumentObj::ObjTypes FrGraphPaneDocObj::GetType(){
    return FrDocumentObj::GraphSet;
}

void FrGraphPaneDocObj::AddGraph(FrGraphDocObj* graph){
    m_Graphs.push_back(graph);
}

void FrGraphPaneDocObj::RemoveGraph(unsigned long id){
    // delete all graphs with specified timeseria id (at this moment every timeseria can have only one graph
    std::vector<FrGraphDocObj*>::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        if ((*it)->GetTimeSeria() == id) break;
    }

    if (it != itEnd){
        FrGraphDocObj* graphDO = (*it);
        m_Graphs.erase(it);
        delete graphDO;
    }
}

void FrGraphPaneDocObj::ClearGraphs(){
    std::vector<FrGraphDocObj*>::iterator it, itEnd(m_Graphs.end());
    for(it = m_Graphs.begin(); it != itEnd; ++it){
        delete (*it);
    }
    m_Graphs.clear();
}

FrGraphDocObj* FrGraphPaneDocObj::GetGraphDocObjByTS(unsigned long id){
    FrGraphDocObj* result = 0;
    if(m_Graphs.size() > 0){
        for (int i = 0; i < m_Graphs.size(); i++){
            result = dynamic_cast<FrGraphDocObj*>(m_Graphs[i]);
            // NOTE: we use timeseria number as id for graph
            if (result->GetTimeSeria() == id)
                break;
            else
                result = 0;
        }
    }

    return result;
}