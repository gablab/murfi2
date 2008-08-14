#ifndef FR_TABSETDOC_OBJ
#define FR_TABSETDOC_OBJ

// includes
#include "FrDocumentObj.h"

// One have to provide valid type system
// All object types have to be registered here.
class FrTabSettingsDocObj : public FrDocumentObj {
public:
    FrTabSettingsDocObj();
    virtual ~FrTabSettingsDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);

    virtual ObjType GetType();

private:

};

#endif
