#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrTabSettingsDocObj.h"

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
	
	m_Slice = 0;
}

FrTabSettingsDocObj* FrMainDocument::GetCurrentTabSettings(){
    typedef std::vector<FrDocumentObj*> TSVector;
    TSVector tabSettings;
    GetObjectsByType(tabSettings, FrDocumentObj::TabSettings);
    
    FrTabSettingsDocObj* result = 0L;
    if(tabSettings.size() > 0){

        TSVector::iterator it, itEnd(tabSettings.end());
        for(it = tabSettings.begin(); it != itEnd; ++it){

            FrTabSettingsDocObj* ts = (FrTabSettingsDocObj*)(*it);
            if(ts->GetIsCurrent()){
                result = ts;
                break;
            }
        }
    }
    return result;
}