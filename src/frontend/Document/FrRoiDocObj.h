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
    FrRoiDocObj(RtMaskImage* roi);
    virtual ~FrRoiDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    // Properties
    FrGetPropMacro(RtMaskImage*,MaskImage);
    FrPropMacro(unsigned long, ID);
    
    // Initialization    
    void CreateEmptyMaskImage(FrImageDocObj* imgDO);
    bool LoadFromFile(FrImageDocObj* imgDO, QString& fileName);
    void CreateByThresholdMaskImage(FrImageDocObj* imgDO, double threshold);
};

#endif

