#include "FrImageDocObj.h"
#include "FrDocument.h"


FrImageDocObj::FrImageDocObj(){
}

FrImageDocObj::~FrImageDocObj(){
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