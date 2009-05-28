#ifndef FR_LAYERDOC_OBJ
#define FR_LAYERDOC_OBJ

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrLayerSettings.h"

#include "RtDataID.h"

//#define BAD_LAYER_ID  0
//#define ALL_LAYER_ID  1
//#define SPEC_LAYRE_ID 2

// This class represents graph data object
// It contains data that application may use for plotting graphs.
class FrLayerDocObj : public FrDocumentObj {
public:
  FrLayerDocObj(FrLayerSettings::LTypes type, 
		const RtDataID &id, QString name = "no name");
    virtual ~FrLayerDocObj();

    // Overrides
    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    bool IsRoi();
    bool IsImage();

    // Properties
    FrPropMacro(FrLayerSettings*, Settings);
    FrPropMacro(unsigned long, ID);
    //FrPropMacro(RtDataID, ID);
    FrPropMacro(unsigned int, Position);

    double GetOpacity();
    bool GetVisibility();

    //Methods
    bool CopySettings(FrLayerSettings* src);
};

#endif // FR_GRAPHDOC_OBJ

