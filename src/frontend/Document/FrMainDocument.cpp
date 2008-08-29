#include "FrMainDocument.h"
#include "FrTBCFilter.h"

FrMainDocument::FrMainDocument() 
: FrDocument(){
}

FrMainDocument::~FrMainDocument(){
}

void FrMainDocument::GetAllImages(std::vector<FrDocumentObj*>& images){
    images.clear();
    GetObjectsByType(images, FrDocumentObj::ImageObject);
}

void FrMainDocument::SetDefaultValues(){
    // Setup threshold/brightness/contrast values
    m_Threshold = DEF_THRESHOLD;
    m_Brightness = DEF_BRIGHTNESS;
    m_Contrast = DEF_CONTRAST;
}