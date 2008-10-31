#ifndef FR_TABSETDOC_OBJ
#define FR_TABSETDOC_OBJ

// includes
#include "FrDocumentObj.h"
#include "FrSettings.h"
#include "FrMacro.h"

#include "Qt/qstring.h"
#include "Qt/qcolor.h"
#include <vector>

// This class represets settings of Tab
// Every tab may have its own:
// 1. ID, Name, Description
// 2. Settigns for each view
// 3. Set of layers (with own settings)
class FrTabSettingsDocObj : public FrDocumentObj {
public:
    enum View { Undefined, SliceView, MosaicView, OrthoView };

public:
    FrTabSettingsDocObj(bool isDefault = false);
    virtual ~FrTabSettingsDocObj();

    // Base class overrides
    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjType GetType();
    
    // Initialization
    void InitFrom(FrTabSettingsDocObj* docObj);

    // Properties
    FrPropMacro(int,ID);
    FrPropMacro(QString, Name);
    FrPropMacro(QString, Description);

    FrPropMacro(bool,IsCurrent);
    FrGetPropMacro(bool,IsDefault)
    FrPropMacro(View, ActiveView);

    FrGetPropMacro(FrSliceViewSettings*,  SliceViewSettings);
    FrGetPropMacro(FrMosaicViewSettings*, MosaicViewSettings);
    FrGetPropMacro(FrOrthoViewSettings*,  OrthoViewSettings);

private:
    void CopyLayersInfo(LayerCollection& dst, LayerCollection& src);
};

#endif

