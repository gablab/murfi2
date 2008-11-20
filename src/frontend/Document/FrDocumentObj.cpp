#include "FrDocumentObj.h"
#include "FrDocument.h"


FrDocumentObj::FrDocumentObj(){
}

FrDocumentObj::~FrDocumentObj(){
}

void FrDocumentObj::OnAdd(FrDocument* doc){
    // NOTE : Do nothing in base class
}

void FrDocumentObj::OnRemove(FrDocument* doc){
    // NOTE : Do nothing in base class 
}

FrDocumentObj::ObjTypes FrDocumentObj::GetType(){
    return FrDocumentObj::BaseObject;
}