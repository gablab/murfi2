#include "FrLayerDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrLayerDocObj::FrLayerDocObj(FrLayerSettings::LTypes type, 
			     const RtDataID &id, QString name)
: m_Settings(0), m_ID (0){
  m_Settings = FrLayerSettings::Create(type,id,name);
    m_ID = (unsigned long)(void*)(this);
    //cout << "FrLayerDocObj::FrLayerDocObj(): creating layer doc obj with id " << m_ID << endl;
}

FrLayerDocObj::~FrLayerDocObj(){
    if(m_Settings) delete m_Settings;
}

void FrLayerDocObj::OnAdd(FrDocument* doc){
    FrLayerCmd* cmd1 = FrCommandController::CreateCmd<FrLayerCmd>();
    cmd1->SetAction(FrLayerCmd::Add);
    cmd1->SetDocObj(this);

    FrRefreshWidgetsInfoCmd* cmd2 = 
        FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd2->SetTarget(FrRefreshWidgetsInfoCmd::LayerList);

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    FrCommandController::Execute(cmd);

    delete cmd;
}

void FrLayerDocObj::OnRemove(FrDocument* doc){
    FrLayerCmd* cmd1 = FrCommandController::CreateCmd<FrLayerCmd>();
    cmd1->SetAction(FrLayerCmd::Delete);
    cmd1->SetDocObj(this);

    FrRefreshWidgetsInfoCmd* cmd2 = 
        FrCommandController::CreateCmd<FrRefreshWidgetsInfoCmd>();
    cmd2->SetTarget(FrRefreshWidgetsInfoCmd::LayerList);

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    FrCommandController::Execute(cmd);

    delete cmd;
}

FrDocumentObj::ObjTypes FrLayerDocObj::GetType(){
    return FrDocumentObj::LayerObject;
}

bool FrLayerDocObj::IsRoi(){
    return (m_Settings && 
        m_Settings->GetType() == FrLayerSettings::LRoi);
}

bool FrLayerDocObj::IsImage(){
    return (m_Settings && 
        m_Settings->GetType() == FrLayerSettings::LImage);
}

bool FrLayerDocObj::CopySettings(FrLayerSettings* src){
    return FrLayerSettings::CopySettings(src, m_Settings);
}

double FrLayerDocObj::GetOpacity(){
    return m_Settings->Opacity;
}

bool FrLayerDocObj::GetVisibility(){
    return m_Settings->Visibility;
}
