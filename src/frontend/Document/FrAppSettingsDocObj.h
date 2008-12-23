#ifndef FR_APPSETSDOC_OBJ
#define FR_APPSETSDOC_OBJ

// Forward declarations
#include "FrDocumentObj.h"
#include "FrMacro.h"

// This class represents settings used by app.
class FrAppSettingsDocObj : public FrDocumentObj {
public:
    FrAppSettingsDocObj();
    virtual ~FrAppSettingsDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

public:
    // Properties
    FrPropMacro(bool, LiveMode);
};

#endif
