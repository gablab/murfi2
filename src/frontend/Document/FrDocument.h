#ifndef FR_DOCUMENT
#define FR_DOCUMENT

// STL includes
#include <set>
#include <map>
#include <vector>

#include "FrDocumentObj.h"

class FrDocument
{
public:
    FrDocument();
    virtual ~FrDocument();
    
    bool Add(FrDocumentObj* obj);
    bool Remove(FrDocumentObj* obj);
    void DeleteAll();
    
private:
    void GetObjectsByType(std::vector<FrDocumentObj*>& objects, 
                          FrDocumentObj::ObjType type);

private:
    // Objects in document
    std::vector< FrDocumentObj* > m_objects;
};

#endif
