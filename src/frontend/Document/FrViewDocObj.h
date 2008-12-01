#ifndef FR_VIEWDOC_OBJ
#define FR_VIEWDOC_OBJ

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrViewSettings.h"

//#define BAD_LAYER_ID  0
//#define ALL_LAYER_ID  1
//#define SPEC_LAYRE_ID 2

// This class represents View settings
// currently used by the Application.
class FrViewDocObj : public FrDocumentObj {
public:
    FrViewDocObj();
    virtual ~FrViewDocObj();

    // Overrides
    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    // Type checking
    bool IsSliceView();
    bool IsMosaicView();
    bool IsOrthoView();

    // Methods
    void CopySettingsFrom(FrTabSettingsDocObj* src);
    void SaveSettingsTo(FrTabSettingsDocObj* dst);

    // Properties
    FrPropMacro(unsigned int, TimePoint); 

    FrPropMacro(Views, ActiveView);
    FrGetPropMacro(FrSliceViewSettings*,  SliceViewSettings);
    FrGetPropMacro(FrMosaicViewSettings*, MosaicViewSettings);
    FrGetPropMacro(FrOrthoViewSettings*,  OrthoViewSettings);
};

#endif // FR_GRAPHDOC_OBJ

