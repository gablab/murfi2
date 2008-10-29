#ifndef FR_ROIDOC_OBJ
#define FR_ROIDOC_OBJ

// Forward declarations
class vtkImageData;
class QString;

// Some includes
#include "FrDocumentObj.h"
#include "FrMacro.h"

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
    FrGetPropMacro(vtkImageData*,MaskImage);

    // Initialization
    bool LoadFromFile(QString& fileName);
    void CreateMaskImage(int x, int y, int z, int sx, int sy, int sz);
};

#endif
