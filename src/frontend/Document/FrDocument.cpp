#include "FrDocument.h"


FrDocument::FrDocument(){
}

FrDocument::~FrDocument(){
    DeleteAll();
}

bool FrDocument::Add(FrDocumentObj* obj){
    // NOTE: do not check for existance now!
    m_Objects.push_back(obj); 
    obj->OnAdd(this);
    return true;
}

bool FrDocument::Remove(FrDocumentObj* obj){
    // find appropriate object and renmove it
    std::vector<FrDocumentObj*>::iterator it, itEnd(m_Objects.end());

    for(it = m_Objects.begin(); it != itEnd; ++it){
        
        if(obj == (*it)){
            FrDocumentObj* docObj = (*it);
            m_Objects.erase(it);

            docObj->OnRemove(this);
            delete docObj;
            break;
        }
    }
    return true;
}

void FrDocument::DeleteAll(){
    std::vector<FrDocumentObj*>::iterator it, itEnd(m_Objects.end());

    for(it = m_Objects.begin(); it != itEnd; ++it){
        FrDocumentObj* docObj = (*it);        
        docObj->OnRemove(this);
        delete docObj;
    }
    m_Objects.clear();
}
    
void FrDocument::GetObjectsByType(std::vector<FrDocumentObj*>& objects, 
                                  FrDocumentObj::ObjType type){
    std::vector<FrDocumentObj*>::iterator it, itEnd(m_Objects.end());

    for(it = m_Objects.begin(); it != itEnd; ++it){
        if((*it)->GetType() == type){
            objects.push_back(*it);
        }
    }
}