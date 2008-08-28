#include "FrDocumentObj.h"
#include "FrDocument.h"


FrDocumentObj::FrDocumentObj()
: m_updateNeeded(false) {
}

FrDocumentObj::~FrDocumentObj(){
}

void FrDocumentObj::OnAdd(FrDocument* doc){
    // NOTE : Do nothing in base class
}

void FrDocumentObj::OnRemove(FrDocument* doc){
    // NOTE : Do nothing in base class 
}

FrDocumentObj::ObjType FrDocumentObj::GetType(){
    return FrDocumentObj::BaseObject;
}

void FrDocumentObj::UpdateObject(){
    // NOTE: do nothing in base class
}