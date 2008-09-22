#ifndef FR_DOCUMENT
#define FR_DOCUMENT

// STL includes
#include <vector>
#include "FrDocumentObj.h"
#include "FrMacro.h"


class FrDocument
{
public:
    FrDocument();
    virtual ~FrDocument();
        
    bool Add(FrDocumentObj* obj);
    bool Remove(FrDocumentObj* obj);
    void DeleteAll();

    // To be overloaded
    void GetObjectsByType(std::vector<FrDocumentObj*>& objects, 
                          FrDocumentObj::ObjType type);

    // Properties
    FrGetRefPropMacro(std::vector< FrDocumentObj* >, Objects);
};

#endif
