#include "FrLayerDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrLayerDocObj::FrLayerDocObj(FrLayerSettings::LTypes type)
: m_Settings(0), m_ID (0){
    m_Settings = FrLayerSettings::Create(type);
    m_ID = (unsigned int)((void*)this);
}

FrLayerDocObj::~FrLayerDocObj(){
    if(m_Settings) delete m_Settings;
}

void FrLayerDocObj::OnAdd(FrDocument* doc){
    // TODO: implement
    FrLayerCmd* cmd = FrCommandController::CreateCmd<FrLayerCmd>();
    cmd->SetAction(FrLayerCmd::Add);
    cmd->SetDocObj(this);

//    FrUpdateLayerInfoCmd* cmd2 = 
//        FrCommandController::CreateCmd<FrUpdateLayerInfoCmd>();

//    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
//    cmd->AddCommand(cmd1);
//    cmd->AddCommand(cmd2);
    cmd->Execute();

    delete cmd;
}

void FrLayerDocObj::OnRemove(FrDocument* doc){
    // TODO: implement
    /*FrLayerCmd* cmd1 = FrCommandController::Create<FrLayerCmd>();
    cmd1->SetAction(FrLayerCmd::Remove);
    cmd1->SetLayer(this);

    FrUpdateLayerInfoCmd* cmd2 = 
        FrCommandController::Create<FrUpdateLayerInfoCmd>();

    FrMultiCmd* cmd = FrCommandController::Create<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->Execute();

    delete cmd;*/
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

bool FrLayerDocObj::IsColormap(){
    return (m_Settings && 
        m_Settings->GetType() == FrLayerSettings::LColormap);
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
