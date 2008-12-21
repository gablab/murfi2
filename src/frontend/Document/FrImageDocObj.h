#ifndef FR_IMAGEDOC_OBJ
#define FR_IMAGEDOC_OBJ

// Forward declarations
class RtMRIImage;
class QString;

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"

#include <vector>

// One have to provide valid type system
// All object types have to be registered here.
class FrImageDocObj : public FrDocumentObj {
public:
    FrImageDocObj();
    FrImageDocObj(RtMRIImage* img);
    virtual ~FrImageDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();
    
    unsigned int GetMatrixSize();

    // Series identifiers
    FrGetPropMacro(unsigned int, SeriesNumber);
    RtMRIImage* GetTimePointData(unsigned int timePoint);
    bool AddTimePointData(RtMRIImage* mriImage);
    unsigned int GetLastTimePoint();

    void ClearAll();
    
private:
    void NotifyAboutNewTimePointData();

private:
    typedef std::vector<RtMRIImage*> ImageCollection; 
    ImageCollection m_Images;
};

#endif

