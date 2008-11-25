#include "FrDocument.h"
#include <algorithm>


FrDocument::FrDocument(){
}

FrDocument::~FrDocument(){
    this->DeleteAll();
}

bool FrDocument::Add(FrDocumentObj* obj){
    DocObjMap::iterator it = m_Objects.find(obj->GetType());
    if(it != m_Objects.end()){
        it->second.push_back(obj);
        obj->OnAdd(this);
    }
    else {
        DocObjCollection newCollection;
        newCollection.push_back(obj);

        m_Objects[obj->GetType()] = newCollection;
        obj->OnAdd(this);
    }
    return true;
}

bool FrDocument::Remove(FrDocumentObj* obj){
    // find appropriate object and remove it
    DocObjMap::iterator it;
    it = m_Objects.find(obj->GetType());

    bool result = false;
    if(it != m_Objects.end()){
        
        DocObjCollection::iterator itr;
        itr = std::find(it->second.begin(), it->second.end(), obj);
        if(itr != it->second.end()){
            it->second.erase(itr);
            obj->OnRemove(this);
            delete obj;
        }
        result = true;
    }
    return result;
}

void FrDocument::DeleteAll(){
    DocObjMap::iterator it, itEnd(m_Objects.end());

    for(it = m_Objects.begin(); it != itEnd; ++it){
        while(it->second.size()){
            Remove( *(it->second.begin()) );
        }
    }
   m_Objects.clear();
}
    
void FrDocument::GetObjectsByType(std::vector<FrDocumentObj*>& objects, 
                                  FrDocumentObj::ObjTypes type){
    objects.clear();
    
    DocObjMap::iterator itr = m_Objects.find(type);
    if(itr != m_Objects.end()){
        objects.resize(itr->second.size());
        std::copy(itr->second.begin(), 
                  itr->second.end(), 
                  objects.begin()); 
    }
}