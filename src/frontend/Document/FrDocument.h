#ifndef FR_DOCUMENT
#define FR_DOCUMENT

// STL includes
#include <vector>
#include "FrDocumentObj.h"
#include "FrMacro.h"


class FrDocument {
public:
    FrDocument();
    virtual ~FrDocument();

    bool Add(FrDocumentObj* obj);
    bool Remove(FrDocumentObj* obj);
    void DeleteAll();

    // To be overloaded
    void GetObjectsByType(std::vector<FrDocumentObj*>& objects, 
                          FrDocumentObj::ObjTypes type);

    // Properties
    // TODO: we have to use here map of vectors with 
    // FrDocumentObj::ObjTypes as a key. This will make 
    // access to objects much faster
    FrGetRefPropMacro(std::vector< FrDocumentObj* >, Objects);
};

#endif
