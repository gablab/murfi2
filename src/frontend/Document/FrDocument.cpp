#include "FrDocument.h"


FrDocument::FrDocument(){
}

FrDocument::~FrDocument(){
    DeleteAll();
}

bool FrDocument::Add(FrDocumentObj* obj){
    // NOTE: do not check for existance
    m_objects.push_back(obj);    
    return true;
}

bool FrDocument::Remove(FrDocumentObj* obj){
    std::vector<FrDocumentObj*>::iterator it, itEnd(m_objects.end());

    for(it = m_objects.begin(); it != itEnd; ++it){
        // same objects have same pointers
        if(obj == (*it)){
            FrDocumentObj* docObj = (*it);
            docObj->OnRemove(this);
            delete docObj;

            m_objects.erase(it);
            break;
        }
    }
    return true;
}

void FrDocument::DeleteAll(){
    std::vector<FrDocumentObj*>::iterator it, itEnd(m_objects.end());

    for(it = m_objects.begin(); it != itEnd; ++it){
        FrDocumentObj* docObj = (*it);
        docObj->OnRemove(this);
        delete docObj;

        m_objects.erase(it);
    }
}
    
void FrDocument::GetObjectsByType(std::vector<FrDocumentObj*>& objects, 
                                  FrDocumentObj::ObjType type){
    std::vector<FrDocumentObj*>::iterator it, itEnd(m_objects.end());

    for(it = m_objects.begin(); it != itEnd; ++it){
        if((*it)->GetType() == type){
            objects.push_back(*it);
        }
    }
}