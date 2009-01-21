#ifndef FR_MAIN_DOCUMENT
#define FR_MAIN_DOCUMENT

// Forward declarations
class FrDataStore;
class FrDocumentObj;
class FrViewDocObj;
class FrTabSettingsDocObj;
class FrAppSettingsDocObj;
class FrLayerDocObj;
class FrGraphTabDocObj;
class FrGraphDocObj;
class FrPointsDocObj;
class RtData;

// Some includes
#include "FrDocument.h"
#include "FrMacro.h"
#include <vector>

class FrMainDocument: public FrDocument {
public:
    FrMainDocument();
    virtual ~FrMainDocument();

    // Some helper methods
    FrTabSettingsDocObj* GetCurrentTabSettings();
    FrGraphTabDocObj* GetCurrentGraphTab();
    
    FrViewDocObj* GetCurrentViewObject();
    FrPointsDocObj* GetCurrentPointsObject();

    FrAppSettingsDocObj* GetAppSettings();

    FrLayerDocObj* GetLayerDocObjByID(int id);
    FrGraphDocObj* GetGraphDocObjByID(int id);

    void AddDataToStore(RtData *data);

    // Properties
    friend class FrDataStore;
    FrGetPropMacro(FrDataStore*, DataStore);
};

#endif

