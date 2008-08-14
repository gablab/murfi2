#include "FrTabSettingsDocObj.h"
#include "FrDocument.h"


FrTabSettingsDocObj::FrTabSettingsDocObj(){
}

FrTabSettingsDocObj::~FrTabSettingsDocObj(){
}

void FrTabSettingsDocObj::OnAdd(FrDocument* doc){
    // NOTE : Do nothing in base class
}

void FrTabSettingsDocObj::OnRemove(FrDocument* doc){
    // NOTE : Do nothing in base class 
}

FrDocumentObj::ObjType FrTabSettingsDocObj::GetType(){
    return FrDocumentObj::TabSettings;
}