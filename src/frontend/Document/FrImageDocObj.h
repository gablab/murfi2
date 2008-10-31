#ifndef FR_IMAGEDOC_OBJ
#define FR_IMAGEDOC_OBJ

// Forward declarations
class RtMRIImage;
class QString;

// Includes
#include "FrMacro.h"
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

    bool LoadFromFile(QString& fileName);
    bool LoadFromMRIImage(RtMRIImage* img);
    unsigned int GetMatrixSize();

    // Properties
    FrGetPropMacro(RtMRIImage*,Image);
};

#endif

