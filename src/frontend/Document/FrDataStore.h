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
    typedef struct dataItem {
        std::string ModuleID;
        unsigned long TimeSeries;

        std::vector<RtDataID> Items;
    } DataItem;

    struct MatchByID{
        std::string moduleID;
        unsigned long seriesNum;

        MatchByID(RtDataID dataID){
            moduleID = dataID.getModuleID();
            seriesNum = dataID.getSeriesNum();
        }
        bool operator()(DataItem& item){
            return (seriesNum == item.TimeSeries &&
                    moduleID == item.ModuleID);
        }
    };

public:
    FrDataStore(FrMainDocument* document);
    virtual ~FrDataStore();

    // Overrides
    virtual void notify(const RtDataID &dataID);
    void OnNotify(const RtDataID &dataID);

    // Properties
    FrGetPropMacro(RtDataStore*, Store);
    FrPropMacro(FrMainDocument*, Document);

    void AddData(RtData *data);
    void SetStore(RtDataStore* store);

    void GetStuff(std::vector<RtDataID>& data);
    void GetAvailableData(std::vector<DataItem>& data);     // get data sorted by timeserias

private:
    // helpers
    void AddImageToDocument(RtData* data);
    void AddRoiToDocument(RtData* data);

};

#endif
