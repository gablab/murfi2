#ifndef FR_DATA_STORE
#define FR_DATA_STORE


// Forward declarations
class RtDataStore;
class FrMainDocument;

#include "FrMacro.h"
#include "RtDataListener.h"
#include <vector>


class FrDataStore : public RtDataListener {
public:
    FrDataStore(FrMainDocument* document);
    virtual ~FrDataStore();

    // Overrides
    virtual void notify(const RtDataID &data);

    // Properties
    FrGetPropMacro(RtDataStore*, Store);
    FrPropMacro(FrMainDocument*, Document);

    void AddData(RtData *data);
    void SetStore(RtDataStore* store);

    void GetStuff(std::vector<RtDataID>& data);

private:
    // helpers
    void AddImageToDocument(RtData* data);
    void AddRoiToDocument(RtData* data);

};

#endif
