#ifndef FR_MAIN_DOCUMENT
#define FR_MAIN_DOCUMENT

// Forward declarations
class FrDataStore;
class FrDocumentObj;
class FrViewDocObj;
class FrTabSettingsDocObj;
class FrAppSettingsDocObj;
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
    
    FrViewDocObj* GetCurrentViewObject();

    FrAppSettingsDocObj* GetAppSettings();

    void AddDataToStore(RtData *data);

    // Properties
    friend class FrDataStore;
    FrGetPropMacro(FrDataStore*, DataStore);
};

#endif

