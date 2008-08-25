#include "FrImageDocObj.h"
#include "FrDocument.h"

#include "vtkImageData.h"


FrImageDocObj::FrImageDocObj()
:m_origin(0), m_copy(0) {
}

FrImageDocObj::~FrImageDocObj(){
    if(m_origin) m_origin->Delete();
    if(m_copy) m_copy->Delete();
}

void FrImageDocObj::OnAdd(FrDocument* doc){
    // NOTE : Do nothing in base class
}

void FrImageDocObj::OnRemove(FrDocument* doc){
    // NOTE : Do nothing in base class 
}

FrDocumentObj::ObjType FrImageDocObj::GetType(){
    return FrDocumentObj::ImageObject;
}

void FrImageDocObj::UpdateObject(){    
    if(!m_origin || !m_copy) return;

    // Update m_copy object from m_origin
}