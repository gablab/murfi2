#ifndef FR_IMAGEDOC_OBJ
#define FR_IMAGEDOC_OBJ

// includes
#include "FrDocumentObj.h"

// One have to provide valid type system
// All object types have to be registered here.
class FrImageDocObj : public FrDocumentObj {
public:
    FrImageDocObj();
    virtual ~FrImageDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);

    virtual ObjType GetType();

private:

};

#endif
