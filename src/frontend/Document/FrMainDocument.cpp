#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrTabSettingsDocObj.h"
#include "FrAppSettingsDocObj.h"
#include "FrDataStore.h"
#include "FrViewDocObj.h"


FrMainDocument::FrMainDocument() 
: FrDocument(), m_DataStore(0){
    m_DataStore = new FrDataStore(this);
}

FrMainDocument::~FrMainDocument(){
    std::vector<FrDocumentObj*> objects;
    std::vector<FrDocumentObj*>::iterator it, itEnd;

    // Remove tab objects
    this->GetObjectsByType(objects, FrDocumentObj::TabSettings);
    itEnd = objects.end();
    for(it = objects.begin(); it != itEnd; ++it){
        this->Remove( (*it) );
    }

    // Remove roi objects
    this->GetObjectsByType(objects, FrDocumentObj::RoiObject);
    itEnd = objects.end();
    for(it = objects.begin(); it != itEnd; ++it){
        // NOTE: Since this type of objects is not deleted do it manually
        FrDocumentObj* obj = (*it);
        this->Remove(obj);
        delete obj;
    }

    // Remove image objects
    this->GetObjectsByType(objects, FrDocumentObj::ImageObject);
    itEnd = objects.end();
    for(it = objects.begin(); it != itEnd; ++it){
        // NOTE: Since this type of objects is not deleted do it manually
        FrDocumentObj* obj = (*it);
        this->Remove(obj);
        delete obj;
    }

    // delete data store
    if(m_DataStore) delete m_DataStore;
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

void FrMainDocument::AddDataToStore(RtData *data){
    m_DataStore->AddData(data);
}

FrViewDocObj* FrMainDocument::GetCurrentViewObject(){
    // get all view objects
    DocObjCollection viewObjects;
    this->GetObjectsByType(viewObjects, FrDocumentObj::ViewObject);

    FrViewDocObj* result = 0;
    if(viewObjects.size() > 0){
        result = (FrViewDocObj*)viewObjects[0];
    }
    return result;
}

FrAppSettingsDocObj* FrMainDocument::GetAppSettings(){
    DocObjCollection settings;
    this->GetObjectsByType(settings, FrDocumentObj::AppSettings);

    FrAppSettingsDocObj* result = 0;
    if(settings.size() > 0){

        result = (FrAppSettingsDocObj*)settings[0];
    }
    return result;
}
