#ifndef FR_DATA_STORE
#define FR_DATA_STORE


// Forward declarations
class RtData;
class RtDataStore;
class FrMainDocument;

#include "FrMacro.h"
#include "RtOutput.h"


class FrDataStore : public RtOutput {
public:
    FrDataStore(FrMainDocument* document);
    virtual ~FrDataStore();

    // Overrides
    virtual void setData(RtData *data);

    // Properties
    FrPropMacro(RtDataStore*, Store);
    FrPropMacro(FrMainDocument*, Document);

    void AddData(RtData *data);

private:
    // helpers
    void AddImageToDocument(RtData* data);
    void AddRoiToDocument(RtData* data);

};

#endif
