#ifndef FR_DOCUMENT
#define FR_DOCUMENT

// STL includes
#include <map>
#include <vector>
#include "FrDocumentObj.h"
#include "FrMacro.h"


class FrDocument {
public:
    // Types
    typedef std::vector<FrDocumentObj*> DocObjCollection;
    typedef std::map<FrDocumentObj::ObjTypes, DocObjCollection> DocObjMap;

public:
    FrDocument();
    virtual ~FrDocument();

    bool Add(FrDocumentObj* obj);
    bool Remove(FrDocumentObj* obj);
    void DeleteAll();

    // To be overloaded
    void GetObjectsByType(DocObjCollection& objects, 
                          FrDocumentObj::ObjTypes type);

    // Properties
    FrGetRefPropMacro(DocObjMap, Objects);
};

#endif
