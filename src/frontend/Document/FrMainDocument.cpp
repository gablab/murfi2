#include "FrMainDocument.h"


FrMainDocument::FrMainDocument() 
: FrDocument(){
}

FrMainDocument::~FrMainDocument(){
}

void FrMainDocument::GetAllImages(std::vector<FrDocumentObj*>& images){
    images.clear();
    GetObjectsByType(images, FrDocumentObj::ImageObject);
}