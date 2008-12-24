#include "FrGraphDocObj.h"
#include "FrDocument.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrGraphDocObj::FrGraphDocObj(FrGraphSettings::GraphTypes type)
: m_Settings(0){
    m_Settings = FrGraphSettings::Create(type);
}

FrGraphDocObj::~FrGraphDocObj(){
    if(m_Settings) delete m_Settings;
}

void FrGraphDocObj::OnAdd(FrDocument* doc){
    // TODO: implement
}

void FrGraphDocObj::OnRemove(FrDocument* doc){
    // TODO: implement
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
