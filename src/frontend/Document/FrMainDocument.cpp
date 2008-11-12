#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrTabSettingsDocObj.h"
#include "FrTabSettingsDocObj.h"


FrMainDocument::FrMainDocument() 
: FrDocument(){
}

FrMainDocument::~FrMainDocument(){
    std::vector<FrDocumentObj*> objects;

    // Remove ROI objects
    this->GetObjectsByType(objects, FrDocumentObj::RoiObject);
    std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
    for(it = objects.begin(); it != itEnd; ++it){
        this->Remove( (*it) );
    }

    // Remove tab objects
    this->GetObjectsByType(objects, FrDocumentObj::TabSettings);
    itEnd = objects.end();
    for(it = objects.begin(); it != itEnd; ++it){
        this->Remove( (*it) );
    }
}

void FrMainDocument::GetAllImages(std::vector<FrDocumentObj*>& images){
    images.clear();
    GetObjectsByType(images, FrDocumentObj::ImageObject);
}

FrTabSettingsDocObj* FrMainDocument::GetCurrentTabSettings(){
    std::vector<FrDocumentObj*>& objects = this->GetObjects();

    FrTabSettingsDocObj* result = 0L;
    if(objects.size() > 0){
        std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
        for(it = objects.begin(); it != itEnd; ++it){
            // Check for type 
            if((*it)->GetType() != FrDocumentObj::TabSettings) continue;

            // returns first current
            FrTabSettingsDocObj* ts = (FrTabSettingsDocObj*)(*it);
            if(ts->GetIsCurrent()){
                result = ts;
                break;
            }
        }
    }
    return result;
}
