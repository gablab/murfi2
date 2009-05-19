#ifndef FR_TABSETDOC_OBJ
#define FR_TABSETDOC_OBJ

class FrLayerSettings;
class FrImageLayerSettings;


// includes
#include "FrDocumentObj.h"
#include "FrMacro.h"
#include "FrViewSettings.h"

#include "RtDataID.h"

#include "Qt/qstring.h"
#include "Qt/qcolor.h"
#include <vector>

#define DEF_TBS_NAME    QString("Tab")
#define DEF_TBS_DESCR   QString("Description")
#define DEF_TBS_VIEW    OrthoView
#define DEF_TBS_CURRENT false

// This class represets settings of Tab
// Every tab may have its own:
// 1. ID, Name, Description
// 2. Settigns for each view
// 3. Set of layers (with own settings)
class FrTabSettingsDocObj : public FrDocumentObj {
public:
    FrTabSettingsDocObj(bool isDefault = false);
    virtual ~FrTabSettingsDocObj();

    // Base class overrides
    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();
    
    virtual FrImageLayerSettings* GetImageLayer(const RtDataID &id);

    // Initialization
    void InitFrom(FrTabSettingsDocObj* docObj);
    void ClearLayersInfo();

    // Properties
    FrPropMacro(unsigned long,ID);
    FrPropMacro(QString, Name);
    //FrPropMacro(QString, Description);
    FrPropMacro(bool, IsCurrent);
    FrGetPropMacro(bool, IsDefault)    

    // Views settings
    FrPropMacro(unsigned int, TimePoint);

    FrPropMacro(Views, ActiveView);
    FrGetPropMacro(FrSliceViewSettings*,  SliceViewSettings);
    FrGetPropMacro(FrMosaicViewSettings*, MosaicViewSettings);
    FrGetPropMacro(FrOrthoViewSettings*,  OrthoViewSettings);

    // Common layers for all views
    typedef std::vector<FrLayerSettings*> LayersCollection;
    FrGetRefPropMacro(LayersCollection, Layers);
    //FrGetPropMacro(FrImageLayerSettings*, ImageLayer);
};

#endif

