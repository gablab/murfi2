#ifndef FR_ROIDOC_OBJ
#define FR_ROIDOC_OBJ

// Forward declarations
class FrImageDocObj;
class RtMaskImage;
class QString;

// Some includes
#include "FrDocumentObj.h"
#include "FrMacro.h"

#include "Qt/qstring.h"

// One have to provide valid type system
// All object types have to be registered here.
class FrRoiDocObj : public FrDocumentObj {
public:
    FrRoiDocObj();
    virtual ~FrRoiDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjType GetType();

    // Properties
    FrGetPropMacro(RtMaskImage*,MaskImage);
    FrPropMacro(int, ID);
    FrPropMacro(QString, Name);
    FrPropMacro(bool, Visibility);
    FrPropMacro(double, Opacity);

    // Initialization
    bool LoadFromFile(QString& fileName);
    void CreateMaskImage(FrImageDocObj* imgDO);
};

#endif

