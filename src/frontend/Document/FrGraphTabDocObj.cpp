#include "FrGraphTabDocObj.h"
#include "FrDocument.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrGraphTabDocObj::FrGraphTabDocObj(){
}

FrGraphTabDocObj::~FrGraphTabDocObj(){
}

void FrGraphTabDocObj::OnAdd(FrDocument* doc){
    // NOTE : Do nothing in base class
}

void FrGraphTabDocObj::OnRemove(FrDocument* doc){
    // NOTE : Do nothing in base class 
}

FrDocumentObj::ObjTypes FrGraphTabDocObj::GetType(){
    return FrDocumentObj::GraphTab;
}
