#include "FrDocument.h"
#include <algorithm>



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
    // find appropriate object and remove it
    std::vector<FrDocumentObj*>::iterator it;
    it = std::find(m_Objects.begin(), m_Objects.end(), obj);

    bool result = false;
    if(it != m_Objects.end()){
        m_Objects.erase(it);
        obj->OnRemove(this);
        delete obj;

        result = true;
    }
    return result;
}

void FrDocument::DeleteAll(){
    std::vector<FrDocumentObj*>::iterator it, itEnd(m_Objects.end());

    while(m_Objects.size() > 0){
        Remove( (*m_Objects.begin()) );
    }
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