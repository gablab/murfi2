#ifndef FR_LAYERDOC_OBJ
#define FR_LAYERDOC_OBJ

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrLayerSettings.h"

//#define BAD_LAYER_ID  0
//#define ALL_LAYER_ID  1
//#define SPEC_LAYRE_ID 2

// This class represents graph data object
// It contains data that application may use for plotting graphs.
class FrLayerDocObj : public FrDocumentObj {
public:
    FrLayerDocObj(FrLayerSettings::LTypes type);
    virtual ~FrLayerDocObj();

    // Overrides
    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    bool IsRoi();
    bool IsImage();
    bool IsColormap();

    // Properties
    FrPropMacro(FrLayerSettings*, Settings);
    FrPropMacro(unsigned int, ID);

    double GetOpacity();
    bool GetVisibility();

    //Methods
    bool CopySettings(FrLayerSettings* src);
};

#endif // FR_GRAPHDOC_OBJ

