#include "FrGraphDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrGraphDocObj::FrGraphDocObj(FrGraphSettings::GraphTypes type)
: m_Settings(0){
    m_Settings = FrGraphSettings::Create(type);
    m_ID = (unsigned int)((void*)this);
}

FrGraphDocObj::FrGraphDocObj(FrGraphDocObj* src){
    m_Settings = FrGraphSettings::Create(src->GetSettings()->GetType());
    FrGraphSettings::CopySettings(src->GetSettings(), m_Settings);
    m_ID = src->GetID();
    m_TimeSeria = src->GetTimeSeria();
}

FrGraphDocObj::~FrGraphDocObj(){
    if(m_Settings) delete m_Settings;
}

void FrGraphDocObj::OnAdd(FrDocument* doc){
    //FrGraphCmd* cmd1 = FrCommandController::CreateCmd<FrGraphCmd>();
    //cmd1->SetAction(FrGraphCmd::Add);
    //cmd1->SetDocObj(this);

    FrRefreshWidgetsInfoCmd* cmd = 
        FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd->SetTarget(FrRefreshWidgetsInfoCmd::GraphPane);

    //FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    //cmd->AddCommand(cmd1);
    //cmd->AddCommand(cmd2);
    FrCommandController::Execute(cmd);

    delete cmd;
}

void FrGraphDocObj::OnRemove(FrDocument* doc){
    //FrGraphCmd* cmd1 = FrCommandController::CreateCmd<FrGraphCmd>();
    //cmd1->SetAction(FrGraphCmd::Delete);
    //cmd1->SetDocObj(this);

    FrRefreshWidgetsInfoCmd* cmd = 
        FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd->SetTarget(FrRefreshWidgetsInfoCmd::GraphPane);

    //FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    //cmd->AddCommand(cmd1);
    //cmd->AddCommand(cmd2);
    FrCommandController::Execute(cmd);

    delete cmd;
}

FrDocumentObj::ObjTypes FrGraphDocObj::GetType(){
    return FrDocumentObj::GraphObject;
}

void FrGraphDocObj::GetGraphData(){
    // TODO: implement
}

void FrGraphDocObj::SetGraphData(){
    // TODO: implement
}
