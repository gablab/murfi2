#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrTabSettingsDocObj.h"
#include "FrTabSettingsDocObj.h"
#include "FrDataStore.h"


FrMainDocument::FrMainDocument() 
: FrDocument(), m_DataStore(0){
    m_DataStore = new FrDataStore(this);
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
    // delete data store
    if(m_DataStore) delete m_DataStore;
}

void FrMainDocument::GetAllImages(std::vector<FrDocumentObj*>& images){
    this->GetObjectsByType(images, FrDocumentObj::ImageObject);
}

FrTabSettingsDocObj* FrMainDocument::GetCurrentTabSettings(){
    // get all tab settings in document
    DocObjCollection tabSets;
    this->GetObjectsByType(tabSets, FrDocumentObj::TabSettings);

    FrTabSettingsDocObj* result = 0L;
    if(tabSets.size() > 0){

        DocObjCollection::iterator it, itEnd(tabSets.end());
        for(it = tabSets.begin(); it != itEnd; ++it){

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
